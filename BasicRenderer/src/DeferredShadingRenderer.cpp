
#include <DeferredShadingRenderer.h>
#include <NS/core/graphics/MasterRenderer.h>
#include <NS/core/graphics/RenderAPI.h>

namespace ns {

	float DeferredShadingRenderer::s_quad[] = { -1, 1, -1, -1, 1, 1, 1, -1 };

	DeferredShadingRenderer::DeferredShadingRenderer(Uint32 width, Uint32 height) {
		this->m_width = width;
		this->m_height = height;
	}

	void DeferredShadingRenderer::init(std::string shaderPath) {

		this->m_quadModel = CreateRef<Model>();
		this->m_quadModel->bind();
		this->m_quadModel->getDataBuffer()->bind();
		this->m_quadModel->getDataBuffer()->storeData(&s_quad, 32, Buffer::Usage::StaticDraw);
		RenderAPI::AttributeFloatPointer(0, 2, false, 0, 0);
		this->m_quadModel->getDataBuffer()->unbind();
		this->m_quadModel->unbind();

		this->m_gBuffer = FrameBuffer::Create(FrameBuffer::Type::DepthTexture, this->m_width, this->m_height, 4);
		this->m_shader = Shader::Create("deffered");
		std::string vertexShaderFile = shaderPath + "/shader.vs";
		std::string fragmentShaderFile = shaderPath + "/shader.fs";
		this->m_shader->addShaderFromFile(vertexShaderFile.c_str(), Shader::Type::Vertex);
		this->m_shader->addShaderFromFile(fragmentShaderFile.c_str(), Shader::Type::Fragment);
		this->m_shader->bindFragOutput(0, "out_Color");
		this->m_shader->bindAttribute(0, "in_position");
		this->m_shader->link();
		Uniform *uniformGPos = this->m_shader->createUniform("tex_gPosition");
		Uniform *uniformGNor = this->m_shader->createUniform("tex_gNormal");
		Uniform *uniformGCol = this->m_shader->createUniform("tex_gColor");
		this->m_shader->validate();
		this->m_shader->bind();
		uniformGPos->load(0);
		uniformGNor->load(1);
		uniformGCol->load(2);
		this->m_shader->bindUniformBlock("engineUbo", NSENGINE_UNIFORM_BLOCK_UNIT);
		this->m_shader->unbind();
	}

	void DeferredShadingRenderer::render(Ref<Scene> scene, Ref<FrameBuffer> resolveTo) {
		this->m_gBuffer->bind();

		MasterRenderer::RenderScene();

		this->m_gBuffer->unbind();

		this->m_shader->bind();
		this->m_gBuffer->getColorTexture(0)->bind(0);		// pos
		this->m_gBuffer->getColorTexture(1)->bind(1);		// nor
		this->m_gBuffer->getColorTexture(2)->bind(2);		// col

		this->m_quadModel->bind();
		RenderAPI::EnableVertexAttributes(1);

		// render to quad
		if (resolveTo != nullptr) {
			resolveTo->bind();
		}
		RenderAPI::Clear();
		RenderAPI::DrawArrays(RenderAPI::Shape::TriangleStrip, 0, 4);
		if (resolveTo != nullptr) {
			resolveTo->unbind();
		}

		RenderAPI::DisableVertexAttributes(1);
		this->m_quadModel->unbind();

		this->m_gBuffer->unbind();
	}

	void DeferredShadingRenderer::cleanUp() {
		this->m_quadModel.reset();
		this->m_gBuffer.reset();
		this->m_shader.reset();
	}
}
