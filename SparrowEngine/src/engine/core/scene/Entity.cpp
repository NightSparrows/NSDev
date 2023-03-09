
#include <NS/core/Log.h>
#include <NS/core/scene/Entity.h>

namespace ns {

	void Entity::addComponent(EntityComponent *component) {
		component->parent = this;
		component->init();
		this->m_components.push_back(component);
	}

	void Entity::removeComponent(EntityComponent *component) {
		if (component->parent != this) {
#ifdef NS_DEBUG
			NS_CORE_WARN("Component's parent is not this Entity!");
#endif // NS_DEBUG
			return;
		}
		component->cleanUp();
		for (auto it = this->m_components.begin(); it != this->m_components.end(); it++) {
			if (*it == component) {
				this->m_components.erase(it);
				delete component;
				return;
			}
		}
	}

	void Entity::update(float delta) {
		for (EntityComponent *component : this->m_components) {
			component->update(delta);
		}
	}

	void Entity::render() {
		for (EntityComponent *component : this->m_components) {
			component->render();
		}
	}

	Entity::Entity() {
	}

	Entity::~Entity() {
		for (EntityComponent *component : this->m_components) {
			delete component;
		}
	}
}
