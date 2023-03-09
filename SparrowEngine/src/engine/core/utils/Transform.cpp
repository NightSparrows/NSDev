
#include <NS/core/utils/Transform.h>
#include <NS/core/deps/glm/gtx/transform.hpp>

namespace ns {

	Transform::Transform() : position(0), rotation(0), scale(1) {

	}

	glm::mat4 Transform::matrix() const {

		glm::mat4 posMatrix = glm::translate(this->position);
		glm::mat4 rotXMatrix = glm::rotate(glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(glm::radians(this->rotation.z), glm::vec3(0, 0, 1));
		glm::mat4 scaleMatrix = glm::scale(this->scale);

		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * rotMatrix * scaleMatrix;
	}

}
