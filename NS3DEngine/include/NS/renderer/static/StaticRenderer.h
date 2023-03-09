#pragma once

#include <map>

#include <NS/core/graphics/Renderer.h>
#include <NS/core/graphics/objects/Shader.h>
#include <NS/core/graphics/model/Model.h>
#include <NS/core/utils/Transform.h>

#define MAX_INSTANCES 10000

namespace ns {

	class StaticRenderer : public Renderer {

	public:
		StaticRenderer(Ref<Shader> shader);

		void addModel(const Transform& transform, const Ref<Model> model);

		void init();

		void update(float delta);

		void render();

		void clearBuffer();

		void cleanUp();
	private:
		Ref<Shader> m_shader;
		Ref<Buffer> m_transMatricesBuffer;

		std::map<Ref<Model>, std::vector<glm::mat4>*> m_entities;


	};

}
