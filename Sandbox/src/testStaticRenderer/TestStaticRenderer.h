#pragma once

#include <map>

#include <NS/SparrowEngine.h>

#define BONE_TRANSFORMS_UNIFORM_BLOCK_UNIT (NSENGINE_UNIFORM_BLOCK_UNIT + 1)

namespace ns {

	class TestStaticRenderer : public Renderer {

	public:
		TestStaticRenderer();
		~TestStaticRenderer();

		void addEntity(Ref<Model> model, Ref<Entity>entity);

		void init();

		void render();

		void clearBuffer();

		void cleanUp();

	private:
		Ref<Shader> m_shader;
		Ref<Buffer> m_instancesUniformBuffer;



		std::map<Ref<Model>, Ref<std::list<Transform*>>> m_entities;

	};

}

