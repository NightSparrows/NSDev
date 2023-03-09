
#include <NS/core/utils/SmoothFloat.h>

namespace ns {

	SmoothFloat::SmoothFloat(float initialValue, float agility) :
		m_target(initialValue), m_actual(initialValue), m_agility(agility)
	{ }

	void SmoothFloat::update(float delta) {
		float offset = this->m_target - this->m_actual;
		float change = offset * delta * this->m_agility;
		this->m_actual += change;
	}

	void SmoothFloat::increaseTarget(float dT) {
		this->m_target += dT;
	}

	void SmoothFloat::setTarget(float target) {
		this->m_target = target;
	}

	void SmoothFloat::instantIncrease(float increase) {
		this->m_actual += increase;
	}

	float SmoothFloat::get() {
		return this->m_actual;
	}

	float SmoothFloat::getTarget() {
		return this->m_target;
	}
}

