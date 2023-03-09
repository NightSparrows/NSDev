
#include <fstream>

#include <NS/core/Log.h>
#include <NS/core/graphics/objects/Shader.h>

namespace ns {

	Shader::Shader(const char *shaderName) {
		this->m_Name = shaderName;
	}

	Shader::~Shader() {
		for (Uniform *uniform : this->m_Uniforms) {
			delete uniform;
		}
	}

	Uniform *Shader::createUniform(const char *variableName) {
		Uniform *uniform = Uniform::Create(variableName, this->getLocation(variableName));
		this->m_Uniforms.push_back(uniform);
		return uniform;
	}

	Uniform *Shader::getUniform(const char *variableName) {
		for (Uniform *uniform : this->m_Uniforms) {
			if (uniform->m_Name.compare(variableName) == 0) {
				return uniform;
			}
		}
		// no uniform found
#ifdef NS_DEBUG
		NS_CORE_WARN("Failed to find uniform:{0}, create one.", variableName);
#endif	// NS_DEBUG

		// then try to create one
		return this->createUniform(variableName);
	}

	void Shader::addShaderFromFile(const char *filePath, Shader::Type type) {
		std::ifstream file(filePath);
		if (file.fail()) {
			NS_CORE_WARN("Failed to load shader file:{0}", filePath);
			NS_CORE_ASSERT(!file.fail(), "Loading shader file.");
			return;
		}
		std::string shaderSource = "";
		std::string line;

		while (std::getline(file, line)) {
			shaderSource += line + "\n";
		}

		file.close();
		this->addShader(shaderSource.c_str(), type);

	}


	std::string Shader::getName() const {
		return this->m_Name;
	}

	Uniform::Uniform(const char *variableName, int location) :
		m_Location(location), m_Name(variableName)
	{ }

}