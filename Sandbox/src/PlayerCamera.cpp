
#include "PlayerCamera.h"

#include <NS/core/io/Keyboard.h>

void PlayerCamera::update(float delta) {

	this->m_angleAroundPlayer.update(delta);
	this->m_distanceFromPlayer.update(delta);

	if (this->m_isFirstPersonCamera) {
		this->position.x = this->m_player->transform.position.x;
		this->position.z = this->m_player->transform.position.z;
		this->position.y = this->m_player->transform.position.y + 5.f;
		this->rotation.y = 180.f - this->m_player->transform.rotation.y;
		this->rotation.y = this->rotation.y - ((float)((int)(this->rotation.y / 360.f)) * 360.f);
	}
	else {
		float verticDistance = this->m_distanceFromPlayer.get() * glm::sin(glm::radians(this->rotation.x));
		float horizDistance = this->m_distanceFromPlayer.get() * glm::cos(glm::radians(this->rotation.x));
		this->position.y = this->m_player->transform.position.y + verticDistance;

		float theta = this->m_player->transform.rotation.y + this->m_angleAroundPlayer.get();
		float offsetX = horizDistance * glm::sin(glm::radians(theta));
		float offsetZ = horizDistance * glm::cos(glm::radians(theta));
		this->position.x = this->m_player->transform.position.x - offsetX;
		this->position.z = this->m_player->transform.position.z - offsetZ;
		this->rotation.y = 180.f - (this->m_angleAroundPlayer.get() + this->m_player->transform.rotation.y);
		this->rotation.y = this->rotation.y - ((float)((int)(this->rotation.y / 360.f)) * 360.f);
	}
}

void PlayerCamera::event(ns::Event* evnet) {

	if (evnet->type == ns::Event::MouseMoved) {
		if (ns::Mouse::IsGrab()) {
			this->rotation.x += ns::Mouse::GetDY() * 0.1f;
			if (this->rotation.x <= -80.0f) {
				this->rotation.x = -80.0f;
			}
			else if (this->rotation.x >= 90.0f) {
				this->rotation.x = 90.0f;
			}
		}
		else {
			if (ns::Mouse::IsMouseButtonDown(ns::Mouse::Left)) {
				this->m_angleAroundPlayer.increaseTarget(-ns::Mouse::GetDX());
				this->rotation.x += ns::Mouse::GetDY();
			}
		}
	}
	else if (evnet->type == ns::Event::MouseScrolled) {
		ns::MouseScrolledEvent *scrollEvent = static_cast<ns::MouseScrolledEvent*>(evnet);
		float targetZoom = this->m_distanceFromPlayer.getTarget();
		float zoomLevel = scrollEvent->yOffset * 0.1f * targetZoom;
		targetZoom -= zoomLevel;
		if (targetZoom < 1.f) {
			targetZoom = 1.f;
		}
		this->m_distanceFromPlayer.setTarget(targetZoom);
	}
	else if (evnet->type == ns::Event::Type::KeyPressed) {
		ns::KeyPressedEvent *keyEvent = static_cast<ns::KeyPressedEvent*>(evnet);
		if (keyEvent->key == ns::Key::F) {
			if (this->m_isFirstPersonCamera) {
				this->m_isFirstPersonCamera = false;
			}
			else {
				this->m_isFirstPersonCamera = true;
			}
		}
		else if (keyEvent->key == ns::Key::W) {
			this->m_angleAroundPlayer.setTarget(0);
		}
	}
}

void PlayerCamera::render() {
	this->reCalculateViewMatrix();
}

PlayerCamera::PlayerCamera(glm::ivec2 size, Player *player) : ns::Camera3D(size), m_angleAroundPlayer(0, 10.f), m_distanceFromPlayer(20.f, 5) {
	this->m_player = player;
	this->m_isFirstPersonCamera = false;
}

bool PlayerCamera::isFirstPersonCamera() {
	return this->m_isFirstPersonCamera;
}
