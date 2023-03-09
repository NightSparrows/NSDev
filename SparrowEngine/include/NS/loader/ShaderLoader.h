#pragma once

#include "../core/graphics/objects/Shader.h"

namespace ns {


#define NS_SHADER_TYPE_VERTEX		0
#define NS_SHADER_TYPE_FRAGMENT		1
#define NS_SHADER_TYPE_GEOMETRY		2

	class ShaderLoader {

	public:
		// TODO
		static bool load(Ref<Shader> shader, const std::string& shaderfolder);

	};


}
