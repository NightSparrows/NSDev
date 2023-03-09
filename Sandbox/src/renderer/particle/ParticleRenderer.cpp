
#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer() : ns::Renderer("terrain") {

}

void ParticleRenderer::init() {

	// shader initialization
	this->m_shader = ns::Shader::Create("terrain");
	this->m_shader->addShaderFromFile("res/shader/particle/shader.vs", ns::Shader::Vertex);
	this->m_shader->addShaderFromFile("res/shader/particle/shader.fs", ns::Shader::Fragment);
	this->m_shader->bindFragOutput(0, "out_Color");
	this->m_shader->bindAttribute(0, "in_position");
	this->m_shader->bindAttribute(1, "in_texCoord");
	this->m_shader->bindAttribute(2, "in_normal");
	this->m_shader->link();
	this->m_transformationMatrix = this->m_shader->createUniform("u_transformationMatrix");
	this->m_shader->validate();
	this->m_shader->bind();
	this->m_shader->bindUniformBlock("engineUbo", NSENGINE_UNIFORM_BLOCK_UNIT);	// the engine scene uniform buffer
	this->m_shader->unbind();

	// the standard model
	this->m_model = ns::VertexArray::Create();

	this->m_model->bind();
	this->m_model->unbind();

}

void ParticleRenderer::update(float delta) {

}

void ParticleRenderer::render() {
	this->m_shader->bind();
	this->m_model->bind();
	ns::RenderAPI::EnableVertexAttributes(1);
	ns::RenderAPI::DepthMask(false);
	ns::RenderAPI::Blend(true);

	// TODO

	ns::RenderAPI::DepthMask(true);
	ns::RenderAPI::Blend(false);
	ns::RenderAPI::DisableVertexAttributes(1);
	this->m_model->unbind();
	this->m_shader->unbind();
}

void ParticleRenderer::clearBuffer() {
}

void ParticleRenderer::cleanUp() {
	this->m_model.reset();
	this->m_shader.reset();
}
