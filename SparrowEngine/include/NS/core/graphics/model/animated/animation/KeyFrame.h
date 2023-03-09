#pragma once

#include <map>

#include "BoneTransform.h"

namespace ns {

	class NS_API KeyFrame {
	public:
		KeyFrame(float timeStamp);
		~KeyFrame();

		void addBoneTransform(const std::string &name, glm::vec3 position, glm::quat rotation, glm::vec3 scale);

		const std::map <std::string, Ref<BoneTransform>> *getPose();

		float getTimeStamp() { return this->m_timeStamp; }

	private:
		float m_timeStamp;		// where is the frame at

		std::map<std::string, Ref<BoneTransform>> m_pose;

	};

}
