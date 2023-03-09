#pragma once

#include <NS/core/graphics/objects/Texture.h>

namespace ns {

	class Skybox {
		friend class SkyboxRenderer;
	public:

	private:
		Ref<Texture> m_texture;
	};

}
