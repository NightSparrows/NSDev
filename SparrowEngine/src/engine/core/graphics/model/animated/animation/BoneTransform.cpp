#include <NS/core/graphics/model/animated/animation/BoneTransform.h>

#include <NS/core/deps/glm/gtx/matrix_decompose.hpp>
#include <NS/core/deps/glm/gtx/quaternion.hpp>

namespace ns {




	BoneTransform::BoneTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) : m_position(position), m_rotation(rotation), m_scale(scale) {
		
	}

	Ref<BoneTransform> BoneTransform::interpolate(Ref<BoneTransform> a, Ref<BoneTransform> b, float progression)
	{
		glm::vec3 pos = glm::mix(a->m_position, b->m_position, progression);
		glm::quat rot = slerp(a->m_rotation, b->m_rotation, progression);	// Don't know
		glm::vec3 scl = glm::mix(a->m_scale, b->m_scale, progression);
		return CreateRef<BoneTransform>(pos, rot, scl);
	}

	glm::mat4 BoneTransform::getLocalTransform() {
		glm::mat4 matrix = glm::mat4(1);
		matrix = glm::translate(matrix, this->m_position);
		matrix = matrix * glm::toMat4(this->m_rotation);
		matrix = glm::scale(matrix, this->m_scale);
		return matrix;
	}

}