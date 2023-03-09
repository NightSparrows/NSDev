#pragma once

#include <NS/SparrowEngine.h>

#include "testAnimatedRenderer/TestAnimatedRenderer.h"
#include <NS/core/deps/glm/glm.hpp>

class PlayerCamera;

class Player : public ns::Entity, std::enable_shared_from_this<Player> {

public:

	enum Status {
		Walk, idle
	};

	void update(float delta);

	void event(ns::Event* event);

	void render(PlayerCamera *camera);

	Player();
	
	ns::Ref<ns::ModelAnimator> getAnimator();


private:
	ns::Ref<ns::AnimatedModel> m_model;
	ns::Ref<ns::ModelAnimator> m_animator;

	ns::Ref<TestAnimatedRenderer> m_renderer;

	bool m_isOnGround;
	float m_groundVelocity;

	float m_currentSpeed;
	float m_sideSpeed;
	float m_walkSpeed = 50.f;
	Status m_status;



};
