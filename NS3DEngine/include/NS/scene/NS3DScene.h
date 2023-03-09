#pragma once

#include <list>

#include <NS/core/scene/Scene.h>

#include "Terrain.h"

namespace ns {

	class NS_API NS3DScene : public Scene {

	public:

		const std::list<Ref<Terrain>>& getTerrains();

		void addTerrain(Ref<Terrain> terrain);

		void removeTerrain(Ref<Terrain> terrain);

	private:
		std::list<Ref<Terrain>> m_Terrains;
	};

}
