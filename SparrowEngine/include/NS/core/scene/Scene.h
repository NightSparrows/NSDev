#pragma once

#include "../Base.h"
#include "Camera.h"
#include "Light.h"
#include "Entity.h"

namespace ns {

	/**
		����
			contain all of the entity
			�t�dload/unload entities   <-- ?
	*/
	class NS_API Scene {
	public:
		Scene();
		~Scene();

		void update(float delta);

		// Just an abstract method that you can customize the rendering process
		virtual void render();

	public:
		glm::vec3 fogColor;
		float fogDensity;
		float fogGradient;

		std::vector<Ref<Light>> lights;
		std::vector <Ref<Entity>> entities;

	};

}
