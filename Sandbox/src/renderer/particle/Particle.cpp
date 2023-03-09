
#include "Particle.h"


Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale) :
	m_position(position), m_velocity(velocity), m_gravityEffect(gravityEffect), m_lifeLength(lifeLength), m_rotation(rotation), m_scale(scale)
{

}

bool Particle::update(float delta) {
	this->m_velocity += (-90.f)/* Gravity */ * this->m_gravityEffect * delta;

	this->m_elapsedTime += delta;
	if (this->m_lifeLength <= this->m_elapsedTime) {
		return false;
	}

	return true;
}
