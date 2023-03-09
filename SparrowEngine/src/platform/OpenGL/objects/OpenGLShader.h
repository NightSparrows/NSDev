#pragma once

#include <vector>

#include <glad/glad.h>
#include <NS/core/graphics/objects/Shader.h>

namespace ns {

	class NS_API OpenGLUniform : public Uniform {
	public:
		OpenGLUniform(const char *variableName, int location);
		~OpenGLUniform() override = default;

		void load(float value) override;

		void load(int value) override;
		void load(bool value) override;

		void load(glm::mat4 value) override;
		void load(glm::vec3 value) override;
		void load(glm::vec4 value) override;
		void load(glm::ivec2 value) override;
	};

	class NS_API OpenGLShader : public Shader {

	public:
		OpenGLShader(const char *shaderName);
		~OpenGLShader();

		void bind() override;
		void unbind() override;

		void addShader(const char *shaderSource, Type type) override;

		void bindFragOutput(uint32_t colorNumber, const char *name) override;
		void bindAttribute(uint32_t index, const char *name) override;

		void bindUniformBlock(const char *variableName, uint32_t binding) override;

		void link() override;
		void validate() override;

	protected:
		int getLocation(const char *variableName) override;

	private:
		GLint m_ID;
		std::vector<GLuint> m_Shaders;
	};

}
