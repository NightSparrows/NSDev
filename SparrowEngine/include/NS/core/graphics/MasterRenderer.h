#pragma once

#include "../scene/Scene.h"
#include "objects/Buffer.h"
#include "Renderer.h"
#include "../scene/Camera.h"
#include "../io/Time.h"

/**
 *
 * struct UniformBlock {
 * 		mat4 projectionMatrix;
 * 		mat4 viewMatrix;
 * 		vec3 fogColor;
 * 		float density, gradient;
 * 		Light lights[MAX_LIGHTS];
 * 	};
 */
#define NSENGINE_UNIFORM_BLOCK_UNIT 1

#define MAX_LIGHTS 8

namespace ns {


	// The main renderer for batch rendering to scene
	// Engine uniform buffer representation
	// the uniform buffer must be vec4 for each count
	// otherwise it looks wired
	// mat4 projectionMatrix;
	// mat4 viewMatrix;
	// vec4 fogColor; (w¬°«O¯d)
	// float fogDensity
	// float fogGradient
	// float lightCount
	// float NONE
	// Light[MAX_LIGHTS]
	class MasterRenderer {
	public:

		// Call by the engine. Application does not need to call again.
		static void Init();

		// Call by the engine. Application does not need to call again.
		static void Update(Time delta);

		// Call by the engine. Application does not need to call again.
		static void CleanUp();

		////////////////////////////////////////////////////////////////////
		/// Application method || Start
		/// 2021/23/22 Do by engine
		////////////////////////////////////////////////////////////////////
		static void BeginFrame();

		static void EndFrame();

		// Below method is for application
		// batching all stuff, not rendering just be prepared.
		// Clear the buffers and setup the master renderer uniform buffer
		static void PrepareScene(Ref<Camera> camera, Ref<Scene> scene);

		// load the camera to the master renderer uniform buffer
		static void PrepareCamera(Ref<Camera> camera, Ref<Scene> scene);

		// Render the screen that is already prepared. will not clear the buffer that is batched.
		// and not going to do the scene batching stuff.
		static void RenderScene();

		// Clearing all renderer batch, NOT OpenGL rendering buffers
		static void ClearBuffers();

		// \return
		// If conflict will return registered renderer. 
		static Ref<Renderer> Add(Ref<Renderer> renderer);

		static Ref<Renderer> Get(const std::string& name);

		////////////////////////////////////////////////////////////////////
		/// Application method || End
		////////////////////////////////////////////////////////////////////
	private:
		static Scope<std::vector<Ref<Renderer>>> s_Renderers;
		static Ref<Buffer> s_UniformBlockBuffer;
		static glm::vec3 s_fogColor;
	};

}
