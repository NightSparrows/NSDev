#pragma once

#include <map>

#include <NS/SparrowEngine.h>

#define BONE_TRANSFORMS_UNIFORM_BLOCK_UNIT (NSENGINE_UNIFORM_BLOCK_UNIT + 1)

class TestAnimatedRenderer : public ns::Renderer {

public:
	TestAnimatedRenderer();
	~TestAnimatedRenderer();

	void addEntity(ns::Entity* entity, ns::Ref<ns::ModelAnimator> animator);

	void init();

	void render();

	void clearBuffer();

	void cleanUp();

private:
	ns::Ref<ns::Shader> m_shader;
	ns::Uniform *m_transformationMatrix;
	ns::Uniform *m_boneOffsets;

	ns::Ref<ns::Buffer> m_boneTransformsBuffer;

	std::map<ns::Entity*, ns::Ref<ns::ModelAnimator>> m_entities;

};
