#pragma once

#include "../core/Base.h"
#include "../core/graphics/objects/Texture.h"

namespace ns {

	namespace Loader {

		// Loading images to texture2D
		// \return
		//		true mean successful.
		bool NS_API loadTexture2D(Ref<Texture> &texture, const std::string& filePath, const TextureConfig config = TextureConfig());

		// 
		bool NS_API loadTextureCube(Ref<Texture> texture, const std::vector<std::string>* filesPath, const TextureConfig config = TextureConfig());
		
		void NS_API loadEmptyCube(Ref<Texture> texture, Uint32 size);

	}

}
