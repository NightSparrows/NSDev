
#include <NS/core/Log.h>
#include <NS/core/graphics/MasterRenderer.h>
#include <NS/core/graphics/RenderAPI.h>

namespace ns {

	Scope<std::vector<Ref<Renderer>>> MasterRenderer::s_Renderers;
	Ref<Buffer> MasterRenderer::s_UniformBlockBuffer;
	glm::vec3 MasterRenderer::s_fogColor;

	void MasterRenderer::PrepareScene(Ref<Camera> camera, Ref<Scene> scene) {

		// push entities to renderers for batch rendering
		scene->render();
		s_fogColor = scene->fogColor;

		PrepareCamera(camera, scene);
	}

	void MasterRenderer::PrepareCamera(Ref<Camera> camera, Ref<Scene> scene) {
		// Preparing for engine uniform buffer object
		s_UniformBlockBuffer->bind();
		void *mapPtr = s_UniformBlockBuffer->startMapping();
		memcpy(mapPtr, &camera->getProjectionMatrix()[0][0], sizeof(glm::mat4));
		mapPtr = (char*)mapPtr + sizeof(glm::mat4);
		memcpy(mapPtr, &camera->getViewMatrix()[0][0], sizeof(glm::mat4));
		mapPtr = (char*)mapPtr + sizeof(glm::mat4);
		// Scene environment
		memcpy(mapPtr, &scene->fogColor, sizeof(glm::vec3));
		mapPtr = (char*)mapPtr + sizeof(glm::vec4);
		memcpy(mapPtr, &scene->fogDensity, sizeof(float));
		mapPtr = (char*)mapPtr + sizeof(float);
		memcpy(mapPtr, &scene->fogGradient, sizeof(float));
		mapPtr = (char*)mapPtr + sizeof(float);
		float numberOfLight = (float)scene->lights.size();
		memcpy(mapPtr, &numberOfLight, sizeof(float));
		mapPtr = (char*)mapPtr + sizeof(glm::vec2);
		// lights
		// memcpy(mapPtr, &scene->lights[0], sizeof(Light) * scene->lights.size());
		glm::vec4 defVec4(0);
		glm::vec4 defAtt(1, 0, 0, 0);
		for (size_t i = 0; i < MAX_LIGHTS; i++) {
			if (scene->lights.size() > i) {
				Ref<Light> light = scene->lights[i];
				memcpy(mapPtr, &light->position, sizeof(glm::vec3));
				mapPtr = (char*)mapPtr + sizeof(glm::vec4);
				memcpy(mapPtr, &light->color, sizeof(glm::vec3));
				mapPtr = (char*)mapPtr + sizeof(glm::vec4);
				memcpy(mapPtr, &light->attenuation, sizeof(glm::vec3));
				mapPtr = (char*)mapPtr + sizeof(glm::vec4);
			}
			else {
				memcpy(mapPtr, &defVec4[0], sizeof(glm::vec4));
				mapPtr = (char*)mapPtr + sizeof(glm::vec4);
				memcpy(mapPtr, &defVec4[0], sizeof(glm::vec4));
				mapPtr = (char*)mapPtr + sizeof(glm::vec4);
				memcpy(mapPtr, &defAtt[0], sizeof(glm::vec4));
				mapPtr = (char*)mapPtr + sizeof(glm::vec4);
			}
		}
		s_UniformBlockBuffer->stopMapping();
		s_UniformBlockBuffer->unbind();
		// Binding the Uniform buffer object
		s_UniformBlockBuffer->bindBase(NSENGINE_UNIFORM_BLOCK_UNIT);

	}

	void MasterRenderer::RenderScene() {
		RenderAPI::Clear();
		RenderAPI::ClearColor({ s_fogColor, 1 });

		// render scene
		for (Ref<Renderer> renderer : *s_Renderers) {
			renderer->render();
		}

	}

	void MasterRenderer::BeginFrame() {

		// TODO: Future planning
	}

	void MasterRenderer::EndFrame() {
		// clear the renderers buffer to do with next frame
		ClearBuffers();
	}

	void MasterRenderer::ClearBuffers() {
		for (Ref<Renderer> renderer : *s_Renderers) {
			renderer->clearBuffer();
		}
	}

	void MasterRenderer::Update(Time delta) {
		for (Ref<Renderer> renderer : *s_Renderers) {
			renderer->update(delta);
		}
	}

	void MasterRenderer::Init() {

		// Initialize uniform buffer for global scene.
		s_UniformBlockBuffer = Buffer::Create(Buffer::UniformBuffer);
		s_UniformBlockBuffer->bind();
		s_UniformBlockBuffer->emptyData(544, Buffer::DynamicDraw);
		s_UniformBlockBuffer->unbind();

		s_Renderers = CreateScope<std::vector<Ref<Renderer>>>();
	}

	void MasterRenderer::CleanUp() {
		for (Ref<Renderer> renderer : *s_Renderers) {
			renderer->cleanUp();
		}
		s_Renderers->clear();
		s_UniformBlockBuffer.reset();			// release the static buffer
		s_Renderers.release();
	}

	Ref<Renderer> MasterRenderer::Add(Ref<Renderer> renderer) {
		for (Ref<Renderer> rend : *s_Renderers) {
			if (rend->getName() == renderer->getName()) {
				NS_CORE_WARN("Renderer name conflict!{0}", renderer->getName());
				return rend;
			}
		}

		renderer->init();

		s_Renderers->push_back(renderer);
		return renderer;
	}

	Ref<Renderer> MasterRenderer::Get(const std::string& name) {
		for (Ref<Renderer> renderer : *s_Renderers) {
			if (renderer->getName() == name) {
				return renderer;		// found!
			}
		}
#ifdef NS_DEBUG
		NS_CORE_WARN("Renderer {0} not found.", name);
#endif // NS_DEBUG
		return nullptr;
	}

}
