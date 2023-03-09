#pragma once

#include <NS/core/graphics/model/animated/ModelAnimation.h>
#include <NS/core/graphics/model/animated/AnimatedModel.h>

namespace ns {

	class NS_API ModelLoader {
	public:

		static bool loadStatic(Ref<Model> model, const char *filePath);

		static bool loadStatic(Ref<Model> model, const char *filePath, const char *texturePath);

		static bool loadAnimated(Ref<AnimatedModel> model, const char *filePath, const char *texturePath);

		static bool loadModelAnimation(Ref<ModelAnimation> animation, const char *filePath);

		static bool loadVMDAnimation(Ref<ModelAnimation> animation, const char *filePath);

	};
}
