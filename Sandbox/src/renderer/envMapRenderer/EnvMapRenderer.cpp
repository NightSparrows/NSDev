
#include "EnvMapRenderer.h"

#include <NS/core/graphics/RenderAPI.h>
#include <NS/core/graphics/MasterRenderer.h>
#include <NS/loader/TextureLoader.h>


EnvMapRenderer::EnvMapRenderer() : ns::Renderer("envMap") {
	this->m_camera = ns::CreateRef<EnvMapCamera>();
}

EnvMapRenderer::~EnvMapRenderer() {

}

void EnvMapRenderer::processEntity(ns::Model *model, ns::Entity *entity) {
	this->m_model = model;
	this->m_entity = entity;
	glm::vec3 pos = entity->transform.position;
	pos.y += 5.f;
	this->m_camera->setPosition(pos);
}

void EnvMapRenderer::renderMap(ns::Ref<ns::Scene> scene) {
	this->m_fbo->bind();
	this->m_fbo->drawAttachment(0);
	for (int i = 0; i < 6; i++) {
		this->m_fbo->attachCubeMapTexture(this->m_envMapTexture, i);
		this->m_camera->switchToFace(i);
		// TODO switch low poly
		ns::MasterRenderer::PrepareCamera(this->m_camera, scene);
		ns::MasterRenderer::RenderScene();
	}
	this->m_fbo->unbind();

}

void EnvMapRenderer::setCurrentCamera(glm::vec3 position) {
	this->m_currentCameraPosition = position;
}

void EnvMapRenderer::render() {

	this->m_shader->bind();

	this->m_uniformCameraPosition->load(this->m_currentCameraPosition);

	// shadow matrix
	this->m_uniformToShadowMapSpace->load(this->m_shadowMapMatrix);
	this->m_shadowMapTexture->bind(3);

	// env map texture bind
	this->m_envMapTexture->bind(1);

	this->m_model->bind();
	ns::RenderAPI::EnableVertexAttributes(4);
	this->m_uniformTransformationMatrix->load(this->m_entity->transform.matrix());

	for (auto meshIt = this->m_model->getMeshes().begin(); meshIt != this->m_model->getMeshes().end(); meshIt++) {
		ns::TexturedMesh *mesh = *meshIt;
		ns::RenderAPI::DrawIndexed(mesh->getIndicesCount(), mesh->getOffset());
	}

	ns::RenderAPI::DisableVertexAttributes(4);
	this->m_model->unbind();

	this->m_shader->unbind();

}


void EnvMapRenderer::clearBuffer() {
	
}

void EnvMapRenderer::init() {

	this->m_envMapTexture = ns::Texture::Create(ns::Texture::textureCube);
	ns::Loader::loadEmptyCube(this->m_envMapTexture, 128);

	this->m_fbo = ns::FrameBuffer::Create(ns::FrameBuffer::DepthRenderBuffer, 128, 128, 0);

	this->m_shader = ns::Shader::Create("envMap");
	this->m_shader->addShaderFromFile("res/shader/envMapShader/shader.vs", ns::Shader::Vertex);
	this->m_shader->addShaderFromFile("res/shader/envMapShader/shader.fs", ns::Shader::Fragment);
	this->m_shader->bindFragOutput(0, "out_Color");
	this->m_shader->bindAttribute(0, "in_position");
	this->m_shader->bindAttribute(1, "in_texCoord");
	this->m_shader->bindAttribute(2, "in_normal");
	this->m_shader->bindAttribute(3, "in_tangent");
	this->m_shader->link();
	ns::Uniform *envMapTexture = this->m_shader->createUniform("u_envMapSampler");
	this->m_uniformTransformationMatrix = this->m_shader->createUniform("u_transformationMatrix");
	this->m_uniformToShadowMapSpace = this->m_shader->createUniform("u_toShadowMapSpace");
	ns::Uniform *shadowMap = this->m_shader->createUniform("shadowMap");
	this->m_uniformCameraPosition = this->m_shader->createUniform("cameraPosition");
	this->m_shader->validate();
	this->m_shader->bind();
	envMapTexture->load(1);
	shadowMap->load(3);
	this->m_shader->bindUniformBlock("engineUbo", NSENGINE_UNIFORM_BLOCK_UNIT);
	this->m_shader->unbind();

}

void EnvMapRenderer::cleanUp() {
	this->m_shader.reset();
}

void EnvMapRenderer::setShadowMap(glm::mat4 shadowMapSpaceMatrix, ns::Ref<ns::Texture> shadowMapTexture) {
	this->m_shadowMapMatrix = shadowMapSpaceMatrix;
	this->m_shadowMapTexture = shadowMapTexture;
}

