

#include "ShadowRenderer.h"
#include <NS/core/deps/glm/ext.hpp>
#include <NS/core/deps/glm/gtx/transform.hpp>

ShadowRenderer::ShadowRenderer(ns::Camera3D *camera, uint32_t shadowMapSize) {
	this->m_shadowBox = new ShadowBox(camera);
	this->m_shadowMapSize = shadowMapSize;
}

void ShadowRenderer::init() {
	this->m_shader = ns::Shader::Create("shadow");
	this->m_shader->addShaderFromFile("res/shader/shadow/shader.vs", ns::Shader::Vertex);
	this->m_shader->addShaderFromFile("res/shader/shadow/shader.fs", ns::Shader::Fragment);
	this->m_shader->bindFragOutput(0, "out_Color");
	this->m_shader->bindAttribute(0, "in_position");
	this->m_shader->bindAttribute(1, "in_texCoord");
	this->m_shader->bindAttribute(2, "in_normal");
	this->m_shader->bindAttribute(3, "in_tangent");
	this->m_shader->bindAttribute(4, "in_bone");
	this->m_shader->bindAttribute(5, "in_weight");
	this->m_shader->link();
	this->m_uniformTransformationMatrix = this->m_shader->createUniform("u_transformationMatrix");
	this->m_uniformProjectionViewMatrix = this->m_shader->createUniform("u_projectionViewMatrix");
	this->m_uniformBoneOffsets = this->m_shader->createUniform("u_boneOffsets");
	this->m_uniformIsStatic = this->m_shader->createUniform("u_isStatic");
	this->m_shader->validate();
	this->m_shader->bind();
	this->m_shader->bindUniformBlock("engineUbo", NSENGINE_UNIFORM_BLOCK_UNIT);
	this->m_shader->bindUniformBlock("boneTransforms", 2);
	this->m_shader->unbind();

	// bone transform
	this->m_boneTransformsBuffer = ns::Buffer::Create(ns::Buffer::Type::UniformBuffer);
	this->m_boneTransformsBuffer->bind();
	this->m_boneTransformsBuffer->emptyData(650 * sizeof(glm::mat4), ns::Buffer::Usage::DynamicDraw);
	this->m_boneTransformsBuffer->unbind();

	// framebuffer
	this->m_shadowFbo = ns::FrameBuffer::Create(ns::FrameBuffer::Type::DepthTexture, this->m_shadowMapSize, this->m_shadowMapSize, 0);

}

glm::mat4 ShadowRenderer::getToShadowMapSpaceMatrix() {
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
	return biasMatrix * this->m_projectionViewMatrix;
}

ns::Ref<ns::Texture> ShadowRenderer::getShadowMapTexture() {
	return this->m_shadowFbo->getDepthTexture();
}

void ShadowRenderer::cleanUp() {
	this->m_shader.reset();
	this->m_shadowFbo.reset();
	this->m_boneTransformsBuffer.reset();
}

ShadowRenderer::~ShadowRenderer() {
	delete this->m_shadowBox;
}

void ShadowRenderer::processEntity(ns::Entity * entity, ns::Model *model) {

	auto it = this->m_entities.find(model);
	std::vector<ns::Entity*>* batchList;
	if (it == this->m_entities.end()) {
		batchList = new std::vector<ns::Entity*>();
		this->m_entities.emplace(std::pair<ns::Model*, std::vector<ns::Entity*>*>(model, batchList));
	}
	else {
		batchList = it->second;
	}
	batchList->push_back(entity);

}

void ShadowRenderer::render(ns::Light *sun) {
	this->m_shadowBox->update(-sun->position);

	this->m_projectionViewMatrix = this->m_shadowBox->getProjectionMatrix() * this->m_shadowBox->getLightViewMatrix();
	this->m_shadowFbo->bind();
	ns::RenderAPI::Clear();
	ns::RenderAPI::DepthTest(true);
	this->m_shader->bind();
	this->m_uniformProjectionViewMatrix->load(this->m_projectionViewMatrix);

	// render entities

	// static
	this->m_uniformIsStatic->load(1.f);
	// reset bone uniform
	this->m_boneTransformsBuffer->bind();
	void *mapPtr = this->m_boneTransformsBuffer->startMapping();
	glm::mat4 identityMatrix = glm::mat4(1.f);
	for (int i = 0; i < 650; i++) {
		memcpy(mapPtr, &identityMatrix[0][0], sizeof(glm::mat4));
		mapPtr = (char*)mapPtr + sizeof(glm::mat4);
	}
	this->m_boneTransformsBuffer->stopMapping();
	this->m_boneTransformsBuffer->unbind();
	this->m_boneTransformsBuffer->bindBase(2);
	this->m_uniformBoneOffsets->load({ 0, 0 });
	for (auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
		ns::Model *model = it->first;
		std::vector<ns::Entity*>* list = it->second;

		model->bind();
		ns::RenderAPI::EnableVertexAttributes(1);
		for (ns::Entity *entity : *list) {

			this->m_uniformTransformationMatrix->load(entity->transform.matrix());
			for (ns::TexturedMesh *mesh : model->getMeshes()) {
				ns::RenderAPI::DrawIndexed(mesh->getIndicesCount(), mesh->getOffset());
			}
		}
		
		ns::RenderAPI::DisableVertexAttributes(1);
		model->unbind();

	}

	// animated
	this->m_uniformIsStatic->load(0.f);
	for (auto it = this->m_animatedEntities.begin(); it != this->m_animatedEntities.end(); it++) {
		ns::Entity* entity = it->first;
		ns::Ref<ns::ModelAnimator> animator = it->second;
		ns::Ref<ns::AnimatedModel> model = animator->getModel();

		model->bind();
		ns::RenderAPI::EnableVertexAttributes(6);

		this->m_uniformTransformationMatrix->load(entity->transform.matrix());

		auto boneOffsetIt = model->getBoneOffsets()->begin();

		for (auto meshIt = model->getMeshes().begin(); meshIt != model->getMeshes().end(); meshIt++) {


			uint32_t boneOffset = boneOffsetIt->first;
			uint32_t boneCount = boneOffsetIt->second;
			NS_ASSERT((boneCount <= 650), "Failed to load bone transformations overflow!");
			this->m_uniformBoneOffsets->load({ boneOffset, boneCount });
			boneOffsetIt++;

			/// Mapping
			this->m_boneTransformsBuffer->bind();
			void *mapPtr = this->m_boneTransformsBuffer->startMapping();
			void *matrixPtr = (void*)animator->getBoneTransformMatrix();
			matrixPtr = (char*)matrixPtr + (sizeof(glm::mat4) * boneOffset);
			memcpy(mapPtr, matrixPtr, boneCount * sizeof(glm::mat4));
			this->m_boneTransformsBuffer->stopMapping();
			this->m_boneTransformsBuffer->unbind();
			this->m_boneTransformsBuffer->bindBase(2);
			/// end mapping

			ns::TexturedMesh *mesh = *meshIt;
			ns::RenderAPI::DrawIndexed(mesh->getIndicesCount(), mesh->getOffset());
		}

		ns::RenderAPI::DisableVertexAttributes(6);
		model->unbind();
	}



	this->m_shader->unbind();
	this->m_shadowFbo->unbind();

	// clear the buffer
	for (auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
		delete it->second;	// delete the list
	}
	this->m_entities.clear();
	this->m_animatedEntities.clear();
}


void ShadowRenderer::processAnimatedEntity(ns::Entity* entity, ns::Ref<ns::ModelAnimator> animator) {
	this->m_animatedEntities.emplace(std::pair<ns::Entity*, ns::Ref<ns::ModelAnimator>>(entity, animator));
}