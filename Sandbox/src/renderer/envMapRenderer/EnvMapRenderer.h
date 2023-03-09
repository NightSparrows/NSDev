#pragma once

#include <NS/core/graphics/Renderer.h>
#include <NS/core/graphics/objects/Shader.h>
#include <NS/core/graphics/objects/FrameBuffer.h>
#include <NS/core/graphics/model/Model.h>
#include <NS/core/scene/Entity.h>
#include <NS/core/scene/Scene.h>

#include "EnvMapCamera.h"

class EnvMapRenderer : public ns::Renderer {
public:
	EnvMapRenderer();
	~EnvMapRenderer();

	void processEntity(ns::Model *model, ns::Entity *entity);

	void setCurrentCamera(glm::vec3 position);

	void init() override;

	void renderMap(ns::Ref<ns::Scene> scene);

	void render() override;

	void clearBuffer() override;

	void cleanUp() override;

	void setShadowMap(glm::mat4 shadowMapSpaceMatrix, ns::Ref<ns::Texture> shadowMapTexture);

private:
	ns::Ref<ns::Shader> m_shader;
	ns::Uniform *m_uniformCameraPosition;

	ns::Ref<ns::Texture> m_envMapTexture;		// texture cube map for rendering
	ns::Ref<ns::FrameBuffer> m_fbo;

	ns::Ref<EnvMapCamera> m_camera;

	glm::vec3 m_currentCameraPosition;

	ns::Uniform *m_uniformTransformationMatrix;

	ns::Model *m_model;
	ns::Entity *m_entity;

	// shadow
	ns::Uniform *m_uniformToShadowMapSpace;
	glm::mat4 m_shadowMapMatrix;
	ns::Ref<ns::Texture> m_shadowMapTexture;


};
