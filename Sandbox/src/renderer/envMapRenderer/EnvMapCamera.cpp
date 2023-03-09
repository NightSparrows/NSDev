
#include "EnvMapCamera.h"
#include <NS/core/deps/glm/ext.hpp>
#include <NS/core/deps/glm/gtx/transform.hpp>


EnvMapCamera::EnvMapCamera() : Camera3D({ 1.f, 1.f }, 90.f) {
	this->rotation.z = 180;
}

void EnvMapCamera::switchToFace(int faceIndex) {
	switch (faceIndex)
	{
	case 0:
		this->rotation.x = 0.f;
		this->rotation.y = 90.f;
		break;
	case 1:
		this->rotation.x = 0.f;
		this->rotation.y = -90.f;
		break;
	case 2:
		this->rotation.x = -90.f;
		this->rotation.y = 180.f;
		break;
	case 3:
		this->rotation.x = 90.f;
		this->rotation.y = 180.f;
		break;
	case 4:
		this->rotation.x = 0.f;
		this->rotation.y = 180.f;
		break;
	case 5:
		this->rotation.x = 0.f;
		this->rotation.y = 0.f;
		break;
	default:
		break;
	}
	this->calcViewMatrix();
}

void EnvMapCamera::calcViewMatrix() {
	glm::mat4 posMatrix = glm::translate(-this->position);
	glm::mat4 rotZMatrix = glm::rotate(glm::radians(this->rotation.z), glm::vec3(0, 0, 1));
	glm::mat4 rotXMatrix = glm::rotate(glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
	glm::mat4 rotYMatrix = glm::rotate(glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
	glm::mat4 scaleMatrix = glm::scale(this->scale);

	glm::mat4 rotMatrix = rotZMatrix * rotXMatrix * rotYMatrix;

	this->viewMatrix = rotMatrix * scaleMatrix * posMatrix;
}