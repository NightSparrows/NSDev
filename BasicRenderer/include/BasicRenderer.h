#pragma once

#include <map>
#include <list>

#include <NS/core/graphics/Renderer.h>
#include <NS/core/scene/Entity.h>
#include <NS/core/graphics/model/Model.h>
#include <NS/core/graphics/objects/Shader.h>

namespace ns {

	class BasicRenderer : public Renderer {
	public:
		BasicRenderer(Ref<Shader> shader);

		// Important
		void addEntity(Ref<Entity> entity, Ref<Model> model);


		void init() override;

		void update(float delta) override;

		void render() override;

		void clearBuffer() override;

		void cleanUp() override;

	private:
		std::map<Ref<Model>, std::list<Ref<Entity>>*>* m_map;

		Ref<Shader> m_shader;
		Uniform *m_transMatrix;
	};

}
