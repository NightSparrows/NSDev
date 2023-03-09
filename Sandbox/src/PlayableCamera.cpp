
#include "PlayableCamera.h"

#include <NS/core/io/Keyboard.h>

namespace ns {

	void PlayableCamera::update(float delta) {

		float speed = 10 * delta;

		if (ns::Keyboard::isKeyDown(ns::Key::W)) {
			this->position.z -= speed;
		}
		if (ns::Keyboard::isKeyDown(ns::Key::S)) {
			this->position.z += speed;
		}
		if (ns::Keyboard::isKeyDown(ns::Key::A)) {
			this->position.x -= speed;
		}
		if (ns::Keyboard::isKeyDown(ns::Key::D)) {
			this->position.x += speed;
		}
		if (ns::Keyboard::isKeyDown(ns::Key::LeftShift)) {
			this->position.y -= speed;
		}
		if (ns::Keyboard::isKeyDown(ns::Key::Space)) {
			this->position.y += speed;
		}
	}

	void PlayableCamera::render() {
		this->reCalculateViewMatrix();
	}

	PlayableCamera::PlayableCamera(glm::ivec2 size) : Camera3D(size) {

	}

}
