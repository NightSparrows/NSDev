
#include <fstream>
#include <NS/core/Log.h>
#include <NS/loader/ShaderLoader.h>

#include <yaml-cpp/yaml.h>

namespace ns {

	bool ShaderLoader::load(Ref<Shader> shader, const std::string& shaderfolder) {

		std::string shaderConfigFile = shaderfolder + "/shader.yaml";

		std::ifstream stream(shaderConfigFile);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Shader"]) {
			NS_CORE_WARN("Settings is not a shader config file.");
			return false;
		}

		auto shaderFiles = data["GLSL"];
		if (shaderFiles) {
			for (auto file : shaderFiles) {

				Uint32 type = file["Type"].as<Uint32>();
				std::string filePath = shaderfolder + file["filePath"].as<std::string>();

				switch (type)
				{
				case NS_SHADER_TYPE_VERTEX:
					shader->addShaderFromFile(filePath.c_str(), Shader::Vertex);
					break;
				case NS_SHADER_TYPE_FRAGMENT:
					shader->addShaderFromFile(filePath.c_str(), Shader::Fragment);
					break;
				case NS_SHADER_TYPE_GEOMETRY:
					shader->addShaderFromFile(filePath.c_str(), Shader::Geometry);
					break;
				default:
					break;
				}
			}
		}
		else {
			NS_CORE_WARN("No shader files!");
			return false;
		}

		// Binding


		shader->link();

		// Uniform

		return true;
	}

}
