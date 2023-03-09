#pragma once

#include "../Base.h"

namespace ns {

	// for lerping the float 
	//	Gett smooth by time
	class NS_API SmoothFloat {
	public:
		SmoothFloat(float initialValue, float agility);

		void update(float delta);

		void increaseTarget(float dT);

		void setTarget(float target);

		void instantIncrease(float increase);

		float get();

		float getTarget();

	private:
		float m_agility;

		float m_target;
		float m_actual;

	};


}
