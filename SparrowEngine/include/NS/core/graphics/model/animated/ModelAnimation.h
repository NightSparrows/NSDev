#pragma once

#include "animation/KeyFrame.h"

namespace ns {

	// Data of animation
	class NS_API ModelAnimation {
	public:
		ModelAnimation();
		~ModelAnimation();

		// the function call by the loader
		// don't use yourself if you don't know how it works
		void setDuration(float duration) { this->m_Duration = duration; }

		void addFrame(Ref<KeyFrame> frame);

		const std::vector<Ref<KeyFrame>> *getFrames();

		float getDuration() { return this->m_Duration; }

	private:
		float m_Duration;		// Animation time in second
		std::vector<Ref<KeyFrame>> m_frames;
	};

}
