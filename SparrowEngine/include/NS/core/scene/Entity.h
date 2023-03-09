#pragma once

#include <list>

#include "../utils/Transform.h"
#include "components/EntityComponent.h"

namespace ns {


	/**

		The instance class of the entity that render in screen
		if you add the component, the component will bind to this entity
		and you don't need to delete a bound component

	*/
	class NS_API Entity {
	public:
		Entity();
		~Entity();

		void addComponent(EntityComponent *component);

		void removeComponent(EntityComponent *component);

		void update(float delta);

		void render();

	public:
		Transform transform;

	private:
		std::list<EntityComponent*> m_components;

	};

}
