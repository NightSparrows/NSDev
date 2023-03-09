
#include <NS/core/Log.h>

#include "OpenGLShader.h"

namespace ns {

	OpenGLShader::OpenGLShader(const char *shaderName) : Shader(shaderName) {
		this->m_ID = glCreateProgram();
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(this->m_ID);
	}

	void OpenGLShader::bind() {
		glUseProgram(this->m_ID);
	}

	void OpenGLShader::unbind() {
		glUseProgram(0);
	}

	void OpenGLShader::addShader(const char *shaderSource, Shader::Type type) {
		GLuint shader;
		GLenum shaderType = -1;

		switch (type) {
		case Shader::Vertex:
			shaderType = GL_VERTEX_SHADER;
			break;
		case Shader::Fragment:
			shaderType = GL_FRAGMENT_SHADER;
			break;
		case Shader::Geometry:
			shaderType = GL_GEOMETRY_SHADER;
			break;
		case Shader::Compute:
			shaderType = GL_COMPUTE_SHADER;
			break;
		case Shader::TessellationControl:
			shaderType = GL_TESS_CONTROL_SHADER;
			break;
		case Shader::TessellationEvaluation:
			shaderType = GL_TESS_EVALUATION_SHADER;
			break;
		default:
			NS_CORE_ASSERT(false, "Unknown shader type.");
			break;
		}
		shader = glCreateShader(shaderType);

		const GLchar *source = shaderSource;
		glShaderSource(shader, 1, &source, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			NS_CORE_ERROR("Failed to compile shader.:{0}", shaderSource);
			NS_CORE_ASSERT(false, "{0}", infoLog.data());

			return;
		}

		// Attach to program
		glAttachShader(this->m_ID, shader);

		this->m_Shaders.push_back(shader);
	}

	void OpenGLShader::bindFragOutput(uint32_t colorNumber, const char *name) {
		glBindFragDataLocation(this->m_ID, colorNumber, name);
	}

	void OpenGLShader::bindAttribute(uint32_t index, const char *name) {
		glBindAttribLocation(this->m_ID, index, name);
	}

	void OpenGLShader::bindUniformBlock(const char *variableName, uint32_t binding) {
		GLuint index = glGetUniformBlockIndex(this->m_ID, variableName);
		if (index == GL_INVALID_INDEX) {
			NS_CORE_WARN("UniformBlock:{0} not found.", variableName);
		}
		else {
			glUniformBlockBinding(this->m_ID, index, binding);
		}
	}

	void OpenGLShader::link() {
		glLinkProgram(this->m_ID);

		GLint isLinked = -1;
		glGetProgramiv(this->m_ID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(this->m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(this->m_ID, maxLength, &maxLength, &infoLog[0]);

			NS_CORE_ERROR("Failed to link program.");
			NS_CORE_ASSERT(false, "{0}", &infoLog[0]);

			return;
		}

		// link success
		for (GLuint shader : this->m_Shaders) {
			glDetachShader(this->m_ID, shader);
			glDeleteShader(shader);
		}
		this->m_Shaders.clear();
	}

	void OpenGLShader::validate() {
		glValidateProgram(this->m_ID);
	}

	int OpenGLShader::getLocation(const char *variableName) {
		GLint index = glGetUniformLocation(this->m_ID, variableName);
		if (index == -1) {
			NS_CORE_WARN("Uniform {0} not found in {1}", variableName, this->m_Name);
		}
		return index;
	}

	/// Uniform
	OpenGLUniform::OpenGLUniform(const char *variableName, int location) :
		Uniform(variableName, location)
	{}

	void OpenGLUniform::load(float value) {
		glUniform1f(this->m_Location, value);
	}

	void OpenGLUniform::load(int value) {
		glUniform1i(this->m_Location, value);
	}

	void OpenGLUniform::load(bool value) {
		glUniform1f(this->m_Location, value ? 1.f : 0.f);
	}

	void OpenGLUniform::load(glm::mat4 value) {
		glUniformMatrix4fv(this->m_Location, 1, GL_FALSE, &value[0][0]);
	}

	void OpenGLUniform::load(glm::vec3 value) {
		glUniform3f(this->m_Location, value.x, value.y, value.z);
	}

	void OpenGLUniform::load(glm::vec4 value) {
		glUniform4f(this->m_Location, value.x, value.y, value.z, value.w);
	}

	void OpenGLUniform::load(glm::ivec2 value) {
		glUniform2i(this->m_Location, value.x, value.y);
	}
	/// Object creation

	// Shader creation
	Ref<Shader> Shader::Create(const char *shaderName) {
		return CreateRef<OpenGLShader>(shaderName);
	}

	// Uniform
	Uniform *Uniform::Create(const char *variableName, int location) {
		return new OpenGLUniform(variableName, location);
	}
}