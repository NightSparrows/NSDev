#pragma once

#include <NS/SparrowEngine.h>

class Particle {
public:
	Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale);

protected:
	bool update(float delta);

private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	float m_gravityEffect;
	float m_lifeLength;
	float m_rotation;
	float m_scale;

	float m_elapsedTime;
};
