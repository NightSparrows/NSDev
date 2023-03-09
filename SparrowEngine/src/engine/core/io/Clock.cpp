//
// Created by ªô¤l·¶ on 2020/10/9.
//

#include <NS/core/io/Clock.h>

namespace ns {

	Clock::Clock() : m_CurrentTime(Time::GetTime()) {
	}

	Time Clock::getElapsedTime() {
		float t = Time::GetTime();
		return t - this->m_CurrentTime;
	}

	Time Clock::restart() {
		float t = Time::GetTime();
		float elapsedTime = t - this->m_CurrentTime;
		this->m_CurrentTime = t;
		return elapsedTime;
	}

}