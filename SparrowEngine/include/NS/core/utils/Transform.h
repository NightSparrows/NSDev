#pragma once

#include "../Base.h"
#include "../deps/glm/glm.hpp"

namespace ns {

	// The world transform
	class NS_API Transform {
	public:
		Transform();

		glm::mat4 matrix() const;

	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

}
