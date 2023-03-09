
#include "StaticRenderer.h"


StaticRenderer::StaticRenderer() : ns::Renderer("static") {

}

StaticRenderer::~StaticRenderer() {

}

void StaticRenderer::processEntity(ns::Model *model, ns::Entity *entity) {
	auto it = this->m_entities.find(model);

	std::vector<ns::Entity*>* list;
	if (it == this->m_entities.end()) {
		list = new std::vector<ns::Entity*>();
		this->m_entities.emplace(std::pair<ns::Model*, std::vector<ns::Entity*>*>(model, list));
	}
	else {
		list = it->second;
	}
	list->push_back(entity);
}

void StaticRenderer::render() {

	ns::RenderAPI::DepthTest(true);
	this->m_shader->bind();

	// shadow matrix
	this->m_uniformToShadowMapSpace->load(this->m_shadowMapMatrix);
	this->m_shadowMapTexture->bind(3);

	for (auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
		ns::Model *model = it->first;
		std::vector<ns::Entity*> *list = it->second;

		this->m_instancesTransformBuffer->bind();
		void *mapPtr = this->m_instancesTransformBuffer->startMapping();
		for (ns::Entity *entity : *list) {
			glm::mat4 matrix = entity->transform.matrix();
			memcpy(mapPtr, &matrix[0][0], sizeof(glm::mat4));
			mapPtr = (char*)mapPtr + sizeof(glm::mat4);
		}
		this->m_instancesTransformBuffer->stopMapping();
		this->m_instancesTransformBuffer->unbind();
		this->m_instancesTransformBuffer->bindBase(NSENGINE_UNIFORM_BLOCK_UNIT + 1);

		// bind model
		model->bind();
		ns::RenderAPI::EnableVertexAttributes(4);

		for (auto meshIt = model->getMeshes().begin(); meshIt != model->getMeshes().end(); meshIt++) {

			ns::TexturedMesh *mesh = *meshIt;
			mesh->getMaterial()->getDiffues()->bind(0);
			this->m_uniformHarNormalMap->load(mesh->getMaterial()->hasNormalTexture() ? 1.f : 0.f);
			if (mesh->getMaterial()->hasNormalTexture()) {
				mesh->getMaterial()->getNormalTexture()->bind(1);
			}
			this->m_uniformHarExtraInfoMap->load(mesh->getMaterial()->hasExtraInfoTexture() ? 1.f : 0.f);
			if (mesh->getMaterial()->hasExtraInfoTexture()) {
				mesh->getMaterial()->getExtraInfoTexture()->bind(2);
			}
			ns::RenderAPI::DrawIndexInstanced(mesh->getIndicesCount(), mesh->getOffset(), (uint32_t)list->size());
		}
		
		ns::RenderAPI::DisableVertexAttributes(4);
		model->unbind();
	}


	this->m_shader->unbind();

}


void StaticRenderer::clearBuffer() {
	for (auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
		delete it->second;
	}
	this->m_entities.clear();
}

void StaticRenderer::init() {

	this->m_instancesTransformBuffer = ns::Buffer::Create(ns::Buffer::Type::UniformBuffer);
	this->m_instancesTransformBuffer->bind();
	this->m_instancesTransformBuffer->emptyData(600 * sizeof(glm::mat4), ns::Buffer::Usage::DynamicDraw);
	this->m_instancesTransformBuffer->unbind();

	this->m_shader = ns::Shader::Create("static");
	this->m_shader->addShaderFromFile("res/shader/static/shader.vs", ns::Shader::Vertex);
	this->m_shader->addShaderFromFile("res/shader/static/shader.fs", ns::Shader::Fragment);
	this->m_shader->bindFragOutput(0, "out_Color");
	this->m_shader->bindAttribute(0, "in_position");
	this->m_shader->bindAttribute(1, "in_texCoord");
	this->m_shader->bindAttribute(2, "in_normal");
	this->m_shader->bindAttribute(3, "in_tangent");
	this->m_shader->link();
	ns::Uniform *texSampler = this->m_shader->createUniform("textureSampler");
	ns::Uniform *normalSampler = this->m_shader->createUniform("normalSampler");
	this->m_uniformHarNormalMap = this->m_shader->createUniform("hasNormalMap");
	this->m_uniformHarExtraInfoMap = this->m_shader->createUniform("hasExtraInfoMap");
	this->m_uniformToShadowMapSpace = this->m_shader->createUniform("u_toShadowMapSpace");
	ns::Uniform *shadowMap = this->m_shader->createUniform("shadowMap");
	ns::Uniform *extraInfoMap = this->m_shader->createUniform("u_extraInfoMap");
	this->m_shader->validate();
	this->m_shader->bind();
	texSampler->load(0);
	normalSampler->load(1);
	extraInfoMap->load(2);
	shadowMap->load(3);
	this->m_shader->bindUniformBlock("engineUbo", NSENGINE_UNIFORM_BLOCK_UNIT);
	this->m_shader->bindUniformBlock("instanceTransforms", NSENGINE_UNIFORM_BLOCK_UNIT + 1);
	this->m_shader->unbind();

}

void StaticRenderer::cleanUp() {
	this->m_shader.reset();
	this->m_instancesTransformBuffer.reset();
}

void StaticRenderer::setShadowMap(glm::mat4 shadowMapSpaceMatrix, ns::Ref<ns::Texture> shadowMapTexture) {
	this->m_shadowMapMatrix = shadowMapSpaceMatrix;
	this->m_shadowMapTexture = shadowMapTexture;
}
