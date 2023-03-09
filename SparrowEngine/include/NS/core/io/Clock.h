#pragma once

#include "Time.h"

namespace ns {

	class NS_API Clock {
	public:
		Clock();

		/// Getting the elapsed time not resetting the last time.
		Time getElapsedTime();

		/// \return
		/// 	the elapsed time
		Time restart();

	private:
		float m_CurrentTime;
	};

}
