
#include <NS/core/scene/Scene.h>

namespace ns {

	Scene::Scene() {
		this->fogColor = { 0.5f, 0.5f, 0.5f };
		this->fogDensity = 0.0035f;
		this->fogGradient = 3.f;
	}

	void Scene::update(float delta) {
		for (Ref<Entity> entity : this->entities) {
			entity->update(delta);
		}
	}

	void Scene::render() {
		for (Ref<Entity> entity : this->entities) {
			entity->render();
		}
	}

	Scene::~Scene() {
		for (Ref<Light> light : this->lights) {
			light.reset();
		}
	}

}
