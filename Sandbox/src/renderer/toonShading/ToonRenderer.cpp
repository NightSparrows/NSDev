
#include "ToonRenderer.h"

ToonRenderer::ToonRenderer() : Renderer("toon") {


}

void ToonRenderer::init() {

	this->m_shader = ns::Shader::Create("toon");
	this->m_shader->addShaderFromFile("res/shader/toonShading/shader.vs", ns::Shader::Vertex);
	this->m_shader->addShaderFromFile("res/shader/toonShading/shader.fs", ns::Shader::Fragment);
	this->m_shader->bindFragOutput(0, "out_Color");
	this->m_shader->bindAttribute(0, "in_position");
	this->m_shader->bindAttribute(1, "in_texCoord");
	this->m_shader->bindAttribute(2, "in_normal");
	this->m_shader->bindAttribute(3, "in_tangent");
	this->m_shader->bindAttribute(4, "in_bone");
	this->m_shader->bindAttribute(5, "in_weight");
	this->m_shader->link();
	this->m_transformationMatrix = this->m_shader->createUniform("u_transformationMatrix");
	ns::Uniform *texSampler = this->m_shader->createUniform("textureSampler");
	this->m_boneOffsets = this->m_shader->createUniform("u_boneOffsets");
	this->m_uniformHasDiffuseTexture = this->m_shader->createUniform("u_hasDiffuseTexture");
	this->m_uniformDiffuseColor = this->m_shader->createUniform("u_diffuseColor");
	this->m_uniformToShadowMapSpace = this->m_shader->createUniform("u_toShadowMapSpace");
	ns::Uniform *shadowMap = this->m_shader->createUniform("shadowMap");
	this->m_shader->validate();
	this->m_shader->bind();
	texSampler->load(0);
	shadowMap->load(3);
	this->m_shader->bindUniformBlock("engineUbo", NSENGINE_UNIFORM_BLOCK_UNIT);
	this->m_shader->bindUniformBlock("boneTransforms", BONE_TRANSFORMS_UNIFORM_BLOCK_UNIT);
	this->m_shader->unbind();

	// bone transform
	this->m_boneTransformsBuffer = ns::Buffer::Create(ns::Buffer::Type::UniformBuffer);
	this->m_boneTransformsBuffer->bind();
	this->m_boneTransformsBuffer->emptyData(650 * sizeof(glm::mat4), ns::Buffer::Usage::StreamDraw);
	this->m_boneTransformsBuffer->unbind();
}

void ToonRenderer::render() {

	ns::RenderAPI::CullFace(false);
	this->m_shader->bind();

	// shadow matrix
	this->m_uniformToShadowMapSpace->load(this->m_shadowMapMatrix);
	this->m_shadowMapTexture->bind(3);

	for (auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
		ns::Entity* entity = it->first;
		ns::Ref<ns::ModelAnimator> animator = it->second;
		ns::Ref<ns::AnimatedModel> model = animator->getModel();

		model->bind();
		ns::RenderAPI::EnableVertexAttributes(6);

		this->m_transformationMatrix->load(entity->transform.matrix());

		auto boneOffsetIt = model->getBoneOffsets()->begin();

		for (auto meshIt = model->getMeshes().begin(); meshIt != model->getMeshes().end(); meshIt++) {


			uint32_t boneOffset = boneOffsetIt->first;
			uint32_t boneCount = boneOffsetIt->second;
			this->m_boneOffsets->load({ boneOffset, boneCount });
			boneOffsetIt++;

			/// Mapping
			this->m_boneTransformsBuffer->bind();
			void *mapPtr = this->m_boneTransformsBuffer->startMapping();
			void *matrixPtr = (void*)animator->getBoneTransformMatrix();
			matrixPtr = (char*)matrixPtr + sizeof(glm::mat4) * boneOffset;
			NS_ASSERT((boneCount <= 650), "Failed to load bone transformations overflow!");
			memcpy(mapPtr, matrixPtr, boneCount * sizeof(glm::mat4));
			this->m_boneTransformsBuffer->stopMapping();
			this->m_boneTransformsBuffer->unbind();
			this->m_boneTransformsBuffer->bindBase(BONE_TRANSFORMS_UNIFORM_BLOCK_UNIT);
			/// end mapping

			ns::TexturedMesh *mesh = *meshIt;

			// material!!
			ns::Material *material = mesh->getMaterial();
			this->m_uniformHasDiffuseTexture->load(material->hasDiffuseTexture());
			if (material->hasDiffuseTexture()) {
				material->getDiffues()->bind(0);
			}
			else {
				this->m_uniformDiffuseColor->load(material->getDiffuseColor());
			}
			

			
			ns::RenderAPI::DrawIndexed(mesh->getIndicesCount(), mesh->getOffset());
		}

		ns::RenderAPI::DisableVertexAttributes(6);
		model->unbind();
	}


	this->m_shader->unbind();
	ns::RenderAPI::CullFace(true);

}

void ToonRenderer::clearBuffer() {
	this->m_entities.clear();
}

void ToonRenderer::cleanUp() {
	this->m_transformationMatrix = nullptr;
	this->m_boneOffsets = nullptr;
	this->m_boneTransformsBuffer.reset();
	this->m_shader.reset();
}

ToonRenderer::~ToonRenderer() {
}

void ToonRenderer::addEntity(ns::Entity *entity, ns::Ref<ns::ModelAnimator> animator) {
	this->m_entities.emplace(std::pair<ns::Entity*, ns::Ref<ns::ModelAnimator>>(entity, animator));
}

void ToonRenderer::setShadowMap(glm::mat4 shadowMapSpaceMatrix, ns::Ref<ns::Texture> shadowMapTexture) {
	this->m_shadowMapMatrix = shadowMapSpaceMatrix;
	this->m_shadowMapTexture = shadowMapTexture;
}
