#pragma once

#include <NS/core/graphics/Renderer.h>
#include <NS/core/graphics/objects/Shader.h>
#include <NS/core/graphics/objects/Buffer.h>

namespace ns {


	class NS_API TerrainRenderer : public Renderer {

	public:
		TerrainRenderer(Ref<Shader> shader);

		void init();

		void update(float delta);

		void render();

		void clearBuffer();

		void cleanUp();
	private:
		Ref<Shader> m_shader;
		Ref<Buffer> m_terrainDataBuffer;
	};

}
