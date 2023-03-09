
#include <NS/core/graphics/model/animated/ModelAnimation.h>


namespace ns {

	void ModelAnimation::addFrame(Ref<KeyFrame> frame) {
		this->m_frames.push_back(frame);
	}

	const std::vector<Ref<KeyFrame>> *ModelAnimation::getFrames() {
		return &this->m_frames;
	}

	ModelAnimation::ModelAnimation() {
	}

	ModelAnimation::~ModelAnimation() {
		for (Ref<KeyFrame> frame : this->m_frames) {
			frame.reset();
		}
		this->m_frames.clear();
	}

}
