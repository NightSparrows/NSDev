#pragma once

#include <string>

#include "../../Base.h"
#include "../../deps/glm/glm.hpp"

namespace ns {


	class NS_API Uniform {
		friend class Shader;

	public:
		Uniform(const char *variableName, int location);
		virtual ~Uniform() = default;

		virtual void load(float value) = 0;
		virtual void load(bool value) = 0;
		virtual void load(int value) = 0;
		virtual void load(glm::mat4 value) = 0;
		virtual void load(glm::vec3 value) = 0;
		virtual void load(glm::vec4 value) = 0;
		virtual void load(glm::ivec2 value) = 0;

	protected:
		static Uniform *Create(const char *variableName, int location);

	protected:
		int m_Location;
		std::string m_Name;
	};

	/**
	 *
	 * 	Creating a shader steps:
	 * 	1. Create a shader
	 * 	2. adding shader sources
	 * 	3. bind attributes
	 * 	4. link the program
	 * 	5. adding uniform, (or uniform blocks)
	 *
	 */
	class NS_API Shader {
	public:

		enum Type {
			Vertex, Fragment, Geometry,
			TessellationControl, TessellationEvaluation,
			Compute
		};

		// Creating / Getting uniform
		Uniform *createUniform(const char *variableName);
		Uniform *getUniform(const char *variableName);

		// Bind/Unbind the shader
		virtual void bind() = 0;
		virtual void unbind() = 0;

		// Add the shader source
		virtual void addShader(const char *shaderSource, Type type) = 0;
		void addShaderFromFile(const char *filePath, Type type);

		virtual void bindFragOutput(uint32_t colorNumber, const char *name) = 0;
		virtual void bindAttribute(uint32_t index, const char *name) = 0;

		virtual void bindUniformBlock(const char *variableName, uint32_t binding) = 0;

		// the linking method
		virtual void link() = 0;

		// validate
		virtual void validate() = 0;

		// Create the shader
		static Ref<Shader> Create(const char *shaderName);

		std::string getName() const;

		Shader(const char *shaderName);
		virtual ~Shader();

	protected:
		virtual int getLocation(const char *variableName) = 0;

		std::string m_Name;

		std::vector<Uniform*> m_Uniforms;

	};


}
