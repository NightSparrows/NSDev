#pragma once

#include "../Base.h"
#include "Camera.h"

namespace ns {

	class NS_API Camera3D : public Camera {
	protected:
		void reCalculateViewMatrix();
	public:
		/*
			fov:
				以角度去算(not弧度)
		*/
		Camera3D(glm::ivec2 size, float fov = 70.f, float near = 0.1f, float far = 1000.f);

		void setPosition(const glm::vec3& position);

		void setRotation(const glm::vec3& rotation);

		void setScale(const glm::vec3& scale);

		const glm::vec3& getPosition();
		const glm::vec3& getRotation();
		const glm::vec3& getScale();

		glm::mat4 getProjectionMatrix() const override;
		glm::mat4 getViewMatrix() const override;

	public:
		float nearPlane;
		float farPlane;
		float fov;
		float aspectRatio;

	protected:
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::vec3 position;
		glm::vec3 rotation;						// In 角度
		glm::vec3 scale;
		float gamma = 2.2f;
	};

}
