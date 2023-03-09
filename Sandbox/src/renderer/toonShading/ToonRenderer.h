#pragma once

#include <map>

#include <NS/SparrowEngine.h>

#define BONE_TRANSFORMS_UNIFORM_BLOCK_UNIT (NSENGINE_UNIFORM_BLOCK_UNIT + 1)

class ToonRenderer : public ns::Renderer {

public:
	ToonRenderer();
	~ToonRenderer();

	void addEntity(ns::Entity* entity, ns::Ref<ns::ModelAnimator> animator);

	void init();

	void render();

	void clearBuffer();

	void cleanUp();

	void setShadowMap(glm::mat4 shadowMapSpaceMatrix, ns::Ref<ns::Texture> shadowMapTexture);

private:
	ns::Ref<ns::Shader> m_shader;
	ns::Uniform *m_transformationMatrix;
	ns::Uniform *m_boneOffsets;
	ns::Uniform *m_uniformHasDiffuseTexture;
	ns::Uniform *m_uniformDiffuseColor;

	// shadow
	ns::Uniform *m_uniformToShadowMapSpace;
	glm::mat4 m_shadowMapMatrix;
	ns::Ref<ns::Texture> m_shadowMapTexture;

	ns::Ref<ns::Buffer> m_boneTransformsBuffer;

	std::map<ns::Entity*, ns::Ref<ns::ModelAnimator>> m_entities;

};
