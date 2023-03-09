#pragma once

#include <NS/SparrowEngine.h>
#include "Particle.h"

class ParticleRenderer : public ns::Renderer {

public:
	ParticleRenderer();

	void init();

	void update(float delta) override;

	/// The rendering method
	void render();

	void clearBuffer() override;

	void cleanUp();

private:
	ns::Ref<ns::VertexArray> m_model;
	
	ns::Ref<ns::Shader> m_shader;

	std::vector<Particle*> m_particles;

	// shader uniform
	ns::Uniform *m_transformationMatrix;
};

