
#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer() : Renderer("skybox") {

}

void SkyboxRenderer::init() {
	float size = 550.f;

	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 1, 0, 0, 5, 4,
		2, 6, 7, 7, 3, 2,
		4, 5, 7, 7, 6, 4,
		0, 3, 7, 7, 5, 0,
		1, 4, 2, 2, 4, 6
	};
	float vertices[] = {
		-size, size, -size,			// 0
		-size, -size, -size,		// 1
		size, -size, -size,			// 2
		size,  size, -size,			// 3

		-size, -size,  size,		// 4
		-size,  size,  size,		// 5

		 size, -size,  size,		// 6
		 size,  size,  size,		// 7
	};
	/*
	uint32_t indices[] = {
		//Top
		2, 6, 7,
		2, 3, 7,

		//Bottom
		0, 4, 5,
		0, 1, 5,

		//Left
		0, 2, 6,
		0, 4, 6,

		//Right
		1, 3, 7,
		1, 5, 7,

		//Front
		0, 2, 3,
		0, 1, 3,

		//Back
		4, 6, 7,
		4, 5, 7
	};


	float vertices[] = {
		-size, -size,  size, //0
		 size, -size,  size, //1
		-size,  size,  size, //2
		 size,  size,  size, //3
		-size, -size, -size, //4
		 size, -size, -size, //5
		-size,  size, -size, //6
		 size,  size, -size  //7
	};
	*/
	this->m_model = ns::VertexArray::Create();
	this->m_model->bind();
	this->m_model->getIndicesBuffer()->bind();
	this->m_model->getIndicesBuffer()->storeData(&indices[0], 36 * sizeof(uint32_t), ns::Buffer::StaticDraw);
	this->m_model->getDataBuffer()->bind();
	this->m_model->getDataBuffer()->storeData(&vertices[0], 24 * sizeof(float), ns::Buffer::StaticDraw);
	//this->m_model->getDataBuffer()->storeData(&VERTICES[0], sizeof(VERTICES), ns::Buffer::StaticDraw);
	ns::RenderAPI::AttributeFloatPointer(0, 3, false, 0, 0);
	this->m_model->getDataBuffer()->unbind();
	this->m_model->unbind();

	// shader
	this->m_shader = ns::Shader::Create("skybox");
	this->m_shader->addShaderFromFile("res/shader/skybox/shader.vs", ns::Shader::Vertex);
	this->m_shader->addShaderFromFile("res/shader/skybox/shader.fs", ns::Shader::Fragment);
	this->m_shader->bindFragOutput(0, "out_Color");
	this->m_shader->bindAttribute(0, "in_position");
	this->m_shader->link();
	ns::Uniform* textureSampler = this->m_shader->createUniform("skyboxTexture");
	this->m_shader->validate();
	this->m_shader->bind();
	textureSampler->load(0);
	this->m_shader->bindUniformBlock("engineUbo", NSENGINE_UNIFORM_BLOCK_UNIT);	// the engine scene uniform buffer
	this->m_shader->unbind();
}

void SkyboxRenderer::render() {
	if (this->m_skybox == NULL)
		return;
	this->m_shader->bind();
	this->m_model->bind();
	ns::RenderAPI::EnableVertexAttributes(1);

	this->m_skybox->m_texture->bind(0);
	ns::RenderAPI::DrawIndexed(36, 0);
	
	ns::RenderAPI::DisableVertexAttributes(1);
	this->m_model->unbind();
	this->m_shader->unbind();
}

void SkyboxRenderer::cleanUp() {
	this->m_model.reset();
	this->m_shader.reset();
}

void SkyboxRenderer::setSkybox(ns::Ref<Skybox> skybox) {
	this->m_skybox = skybox;
}