#pragma once

#include <NS/core/graphics/Renderer.h>
#include <NS/core/graphics/objects/VertexArray.h>
#include <NS/core/graphics/objects/Shader.h>
#include "Skybox.h"

namespace ns {

	class SkyboxRenderer : public Renderer {

	public:
		SkyboxRenderer(float size);

		void init() override;

		void update(float delta) override;

		void render() override;

		void clearBuffer() override;

		void cleanUp() override;

	private:
		float m_size;
		Ref<VertexArray> m_cube;
		Ref<Shader> m_shader;
		Ref<Skybox> m_skyBox;
		bool m_activeSkybox;
	};

}
