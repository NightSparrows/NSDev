#pragma once

#include <NS/core/scene/Camera3D.h>

class EnvMapCamera : public ns::Camera3D {

public:
	EnvMapCamera();

	void switchToFace(int faceIndex);

	void calcViewMatrix();

};
