#pragma once

#include <NS/core/scene/Camera3D.h>
#include <NS/core/utils/SmoothFloat.h>

#include "Player.h"

class PlayerCamera : public ns::Camera3D {

public:
	PlayerCamera(glm::ivec2 size, Player *player);

	void update(float delta);

	void event(ns::Event* evnet);

	void render();

	bool isFirstPersonCamera();

private:
	Player *m_player;

	ns::SmoothFloat m_distanceFromPlayer;
	ns::SmoothFloat m_angleAroundPlayer;

	bool m_isFirstPersonCamera;

};
