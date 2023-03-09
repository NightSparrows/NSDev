#pragma once

#include "../Base.h"

namespace ns {

	class NS_API Time {
	public:
		static float GetTime();

		Time(float time = 0.0f) : m_Time(time) { }

		float asSecond() const { return this->m_Time; }
		float asMilliseconds() const { return this->m_Time * 1000.0f; }

		operator float() const { return this->m_Time; }

	private:
		float m_Time;
	};

}
