#pragma once


#include "Entity.h"

namespace ns {

	class NS_API Light {
	public:
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 attenuation = glm::vec3(1, 0, 0);
	};

}
