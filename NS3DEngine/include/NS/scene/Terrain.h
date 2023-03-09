#pragma once

#include <NS/core/Base.h>
#include <NS/core/graphics/objects/Texture.h>
#include <NS/core/graphics/model/Material.h>

namespace ns {

	class NS_API Terrain {


	private:
		int m_chunkX;
		int m_chunkZ;
		Ref<Texture> m_heightMap;
		
		Ref<Texture> m_blendMap;

		Ref<Material> m_rMaterial;
		Ref<Material> m_gMaterial;
		Ref<Material> m_bMaterial;
		Ref<Material> m_bgMaterial;

	};

}
