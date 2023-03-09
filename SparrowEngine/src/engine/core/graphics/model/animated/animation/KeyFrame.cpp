#include <NS/core/graphics/model/animated/animation/KeyFrame.h>

namespace ns {

	KeyFrame::KeyFrame(float timeStamp) : m_timeStamp(timeStamp) {
	}

	KeyFrame::~KeyFrame() {
		for (auto it = this->m_pose.begin(); it != this->m_pose.end(); it++) {
			it->second.reset();
		}
		this->m_pose.clear();
	}

	const std::map <std::string, Ref<BoneTransform>> *KeyFrame::getPose() {
		return &this->m_pose;
	}

	void KeyFrame::addBoneTransform(const std::string &name, glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
		Ref<BoneTransform> boneTransform = CreateRef<BoneTransform>(position, rotation, scale);
		this->m_pose.emplace(std::pair< std::string, Ref<BoneTransform>>(name, boneTransform));
	}
}