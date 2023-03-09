

#include <NS/renderer/terrain/TerrainRenderer.h>


namespace ns {

	TerrainRenderer::TerrainRenderer(Ref<Shader> shader) : Renderer("terrain") {
		this->m_shader = shader;
	}

	void TerrainRenderer::init() {
		this->m_terrainDataBuffer = Buffer::Create(Buffer::UniformBuffer);
	}

	void TerrainRenderer::update(float delta) {

	}

	void TerrainRenderer::render() {



	}

	void TerrainRenderer::clearBuffer() {

	}

	void TerrainRenderer::cleanUp() {
		this->m_terrainDataBuffer.reset();
	}


}

