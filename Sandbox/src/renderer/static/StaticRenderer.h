#pragma once

#include <NS/SparrowEngine.h>

class StaticRenderer : public ns::Renderer {

public:
	StaticRenderer();
	~StaticRenderer();

	void processEntity(ns::Model *model, ns::Entity *entity);

	void init() override;

	void render() override;

	void clearBuffer() override;

	void cleanUp() override;

	void setShadowMap(glm::mat4 shadowMapSpaceMatrix, ns::Ref<ns::Texture> shadowMapTexture);

private:
	ns::Ref<ns::Shader> m_shader;
	ns::Uniform *m_uniformHarNormalMap;
	ns::Uniform *m_uniformHarExtraInfoMap;
	ns::Ref<ns::Buffer> m_instancesTransformBuffer;

	std::map<ns::Model*, std::vector<ns::Entity*>*> m_entities;

	// shadow
	ns::Uniform *m_uniformToShadowMapSpace;
	glm::mat4 m_shadowMapMatrix;
	ns::Ref<ns::Texture> m_shadowMapTexture;

};
