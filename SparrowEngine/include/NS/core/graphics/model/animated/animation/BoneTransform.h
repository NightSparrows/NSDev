#pragma once

#include "../../../../Base.h"
#include "../../../../deps/glm/glm.hpp"
#include "../../../../deps/glm/gtc/quaternion.hpp"

namespace ns {

	// one bone transformation
	class NS_API BoneTransform {
	public:
		BoneTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

		static Ref<BoneTransform> interpolate(Ref<BoneTransform> a, Ref<BoneTransform> b, float progression);

		glm::mat4 getLocalTransform();

	private:
		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;

	};

}

