

#include <NS/core/scene/Camera3D.h>
#include <NS/core/deps/glm/ext.hpp>
#include <NS/core/deps/glm/gtx/transform.hpp>

namespace ns {

	Camera3D::Camera3D(glm::ivec2 size, float fov, float near, float far)
	{
		this->fov = fov;
		this->nearPlane = near;
		this->farPlane = far;
		this->aspectRatio = (float)size.x / (float)size.y;
		this->projectionMatrix = glm::perspective(glm::radians(fov), this->aspectRatio, near, far);
		this->position = glm::vec3(0, 0, 0);
		this->rotation = glm::vec3(0);
		this->scale = glm::vec3(1);
		this->reCalculateViewMatrix();
	}

	void Camera3D::reCalculateViewMatrix() {
		glm::mat4 posMatrix = glm::translate(-this->position);
		glm::mat4 rotXMatrix = glm::rotate(glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(glm::radians(this->rotation.z), glm::vec3(0, 0, 1));
		glm::mat4 scaleMatrix = glm::scale(this->scale);

		glm::mat4 rotMatrix = rotXMatrix * rotYMatrix * rotZMatrix;

		this->viewMatrix = rotMatrix * scaleMatrix * posMatrix;
	}

	void Camera3D::setPosition(const glm::vec3& position) { 
		this->position = position; this->reCalculateViewMatrix(); 
	}

	void Camera3D::setRotation(const glm::vec3& rotation) { this->rotation = rotation; this->reCalculateViewMatrix(); }

	void Camera3D::setScale(const glm::vec3& scale) { this->scale = scale; this->reCalculateViewMatrix(); }

	const glm::vec3& Camera3D::getPosition() { return this->position; }
	const glm::vec3& Camera3D::getRotation() { return this->rotation; }
	const glm::vec3& Camera3D::getScale() { return this->scale; }

	glm::mat4 Camera3D::getProjectionMatrix() const
	{
		return this->projectionMatrix;
	}

	glm::mat4 Camera3D::getViewMatrix() const
	{
		return this->viewMatrix;
	}

}
