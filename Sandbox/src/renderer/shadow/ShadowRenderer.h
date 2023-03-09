#pragma once

#include "ShadowBox.h"

class ShadowRenderer {
public:
	ShadowRenderer(ns::Camera3D *camera, uint32_t shadowMapSize = 4096);
	~ShadowRenderer();

	void init();
	void cleanUp();

	void processEntity(ns::Entity * entity, ns::Model *model);
	void processAnimatedEntity(ns::Entity* entity, ns::Ref<ns::ModelAnimator> animator);


	void render(ns::Light *sun);

	glm::mat4 getToShadowMapSpaceMatrix();

	ns::Ref<ns::Texture> getShadowMapTexture();

private:
	ShadowBox *m_shadowBox;
	ns::Ref<ns::Shader> m_shader;
	ns::Ref<ns::FrameBuffer> m_shadowFbo;
	ns::Ref<ns::Buffer> m_boneTransformsBuffer;

	ns::Uniform *m_uniformTransformationMatrix;
	ns::Uniform *m_uniformBoneOffsets;
	ns::Uniform *m_uniformProjectionViewMatrix;
	ns::Uniform *m_uniformIsStatic;

	glm::mat4 m_projectionViewMatrix;

	std::map<ns::Model*, std::vector<ns::Entity*>*> m_entities;
	std::map<ns::Entity*, ns::Ref<ns::ModelAnimator>> m_animatedEntities;

	// property
	uint32_t m_shadowMapSize;
};


