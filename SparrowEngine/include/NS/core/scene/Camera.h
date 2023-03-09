#pragma once

#include "../deps/glm/glm.hpp"

namespace ns {

	// Use for MasterRenderer::Render method giving the transformation of camera view
	class Camera {

	public:
		virtual glm::mat4 getProjectionMatrix() const = 0;
		virtual glm::mat4 getViewMatrix() const = 0;

	};

}
