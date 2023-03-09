#pragma once

#include <NS/SparrowEngine.h>

#define SHADOW_DISTANCE 100.0f
#define SHADOW_OFFSET 10.f;

class ShadowBox {

public:
	ShadowBox(ns::Camera3D *camera);

	void update(glm::vec3 lightDirection);

	glm::mat4 getProjectionMatrix();
	glm::mat4 getLightViewMatrix();

private:

	glm::mat4 calculateCameraRotationMatrix();

private:

	float m_offset;		// offset of the box

	glm::mat4 m_lightViewMatrix;
	glm::mat4 m_projectionMatrix;

	ns::Camera3D *m_camera;

};
