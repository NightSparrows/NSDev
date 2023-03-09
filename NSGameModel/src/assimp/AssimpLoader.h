#pragma once

#include <NS/core/graphics/model/Model.h>
#include <NS/core/graphics/model/animated/ModelAnimation.h>
#include <NS/core/graphics/model/animated/AnimatedModel.h>

namespace ns {


	class NS_API AssimpLoader {

	public:
		static bool loadStatic(Ref<Model> model, std::string& filePath, std::string& texturePath);

		// for stable don't chamge the name Armature to your bone
		// I'm just Anchor the bone of the name "Armature"
		static bool loadAnimated(Ref<AnimatedModel> model, std::string& filePath, std::string& texturePath);
		
		static bool loadModelAnimation(Ref<ModelAnimation> animation, const char *filePath);
	};

}

