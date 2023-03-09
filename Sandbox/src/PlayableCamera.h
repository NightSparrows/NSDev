#pragma once

#include <NS/core/scene/Camera3D.h>

namespace ns {

	class PlayableCamera : public Camera3D {

	public:
		PlayableCamera(glm::ivec2 size);

		void update(float delta);

		void render();

	};

}
