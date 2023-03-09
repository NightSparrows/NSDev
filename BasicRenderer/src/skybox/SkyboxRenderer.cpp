
#include <NS/renderer/skybox/SkyboxRenderer.h>
#include <NS/core/graphics/RenderAPI.h>

namespace ns {

	SkyboxRenderer::SkyboxRenderer(float size) : Renderer("skybox") {
		this->m_activeSkybox = false;
	}

	void SkyboxRenderer::init() {
		float size = this->m_size;
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
		this->m_cube = VertexArray::Create();
		this->m_cube->bind();
		this->m_cube->getIndicesBuffer()->bind();
		this->m_cube->getIndicesBuffer()->storeData(&indices[0], 36 * sizeof(uint32_t), Buffer::StaticDraw);
		this->m_cube->getDataBuffer()->bind();
		this->m_cube->getDataBuffer()->storeData(&vertices[0], 24 * sizeof(float), Buffer::StaticDraw);
		RenderAPI::AttributeFloatPointer(0, 3, false, 0, 0);
		this->m_cube->getDataBuffer()->unbind();
		this->m_cube->unbind();
	}

	void SkyboxRenderer::update(float delta) {

	}

	void SkyboxRenderer::render() {
		if (!this->m_activeSkybox)
			return;

		this->m_shader->bind();
		this->m_cube->bind();

		this->m_skyBox->m_texture->bind(0);
		RenderAPI::DrawIndexed(36, 0);

		this->m_cube->unbind();
		this->m_shader->unbind();
	}

	void SkyboxRenderer::clearBuffer() {

	}

	void SkyboxRenderer::cleanUp() {
		this->m_cube.reset();
	}
}
