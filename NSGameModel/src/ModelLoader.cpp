
#include <NS/core/Log.h>
#include <NS/ModelLoader.h>

#include "MMD/MMDLoader.h"
#include "assimp/AssimpLoader.h"

namespace ns {

	bool ModelLoader::loadStatic(Ref<Model> model, const char *filePath, const char *texturePath) {

		std::string filePathText;
		filePathText = filePath;

		std::string extName = filePathText.substr(filePathText.find(".") + 1, filePathText.length());

		return AssimpLoader::loadStatic(model, filePathText, std::string(texturePath));
	}

	bool ModelLoader::loadStatic(Ref<Model> model, const char *filePath) {
		return loadStatic(model, filePath, filePath);
	}

	bool ModelLoader::loadAnimated(Ref<AnimatedModel> model, const char *filePath, const char *texturePath) {
		
		std::string filePathText;
		filePathText = filePath;

		std::string extName = filePathText.substr(filePathText.find(".") + 1, filePathText.length());

		return AssimpLoader::loadAnimated(model, filePathText, std::string(texturePath));

		/*
		if (extName == "pmx") {
			// return MMDLoader::load(model, filePathText, std::string(texturePath));
			// TODO
			return MMDLoader::load(model, filePathText, std::string(texturePath));
		}
		else {
			return AssimpLoader::loadAnimated(model, filePathText, std::string(texturePath));
		}*/
	}

	bool ModelLoader::loadModelAnimation(Ref<ModelAnimation> animation, const char *filePath) {
		return AssimpLoader::loadModelAnimation(animation, filePath);
	}

	bool ModelLoader::loadVMDAnimation(Ref<ModelAnimation> animation, const char *filePath) {
		return MMDLoader::loadAnimation(animation, filePath);
	}
}
