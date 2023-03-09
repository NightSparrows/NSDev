#pragma once

#include <NS/SparrowEngine.h>
#include "Terrain.h"

class TerrainRenderer : public ns::Renderer {

public:
	TerrainRenderer();

	void init();

	void processTerrain(ns::Ref<Terrain> terrain);

	/// The rendering method
	void render();

	void clearBuffer() override;

	void cleanUp();

	void setShadowMap(glm::mat4 shadowMapSpaceMatrix, ns::Ref<ns::Texture> shadowMapTexture);

private:
	ns::Ref<ns::VertexArray> m_model;
	float m_size = 800.f;
	int m_vertexCount = 128;

	std::vector<ns::Ref<Terrain>> m_entities;
	ns::Ref<ns::Shader> m_shader;

	// shader uniform
	ns::Uniform *m_transformationMatrix;
	ns::Uniform *m_uniformToShadowMapSpace;

	glm::mat4 m_shadowMapMatrix;
	ns::Ref<ns::Texture> m_shadowMapTexture;

};

