#pragma once

#include <NS/core/Base.h>

#include <NS/core/graphics/model/animated/AnimatedModel.h>
#include <NS/core/graphics/model/animated/ModelAnimation.h>

namespace ns {

	class NS_API MMDLoader {
	public:
		static bool load(Ref<AnimatedModel> model, std::string& filePath, std::string& texturePath);

		static bool loadAnimation(Ref<ModelAnimation> animation, const char *filePath);

	};

}
