#pragma once

#include "Skybox.h"

class SkyboxRenderer : public ns::Renderer {
public:
	SkyboxRenderer();

	void init() override;

	void render() override;

	void cleanUp() override;

	void setSkybox(ns::Ref<Skybox> skybox);

private:
	ns::Ref<ns::VertexArray> m_model;
	ns::Ref<ns::Shader> m_shader;

	ns::Ref<Skybox> m_skybox;

};

