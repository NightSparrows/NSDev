#pragma once

#include <NS/core/graphics/Renderer.h>
#include <NS/core/scene/Scene.h>
#include <NS/core/graphics/model/Model.h>
#include <NS/core/graphics/objects/Shader.h>
#include <NS/core/graphics/objects/FrameBuffer.h>

namespace ns {

	// Is not a regular renderer to MasterRenderer
	// it must be out of the renderer of scene
	class NS_API DeferredShadingRenderer {
	public:
		DeferredShadingRenderer(Uint32 width, Uint32 height);

		// the shader path, contain shader.vs shader.fs
		void init(std::string shaderPath);

		// Render the scene and parse to the Framebuffer you chosen
		void render(Ref<Scene> scene, Ref<FrameBuffer> resolveTo = nullptr);

		void cleanUp();

	private:
		Ref<Model> m_quadModel;
		Ref<FrameBuffer> m_gBuffer;
		Ref<Shader> m_shader;
		Uint32 m_width, m_height;

		static float s_quad[];

	};


}
