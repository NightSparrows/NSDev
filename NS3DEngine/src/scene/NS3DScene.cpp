
#include <NS/scene/NS3DScene.h>

namespace ns {

	const std::list<Ref<Terrain>>& NS3DScene::getTerrains() {
		return this->m_Terrains;
	}

	void NS3DScene::addTerrain(Ref<Terrain> terrain) {
		this->m_Terrains.push_back(terrain);
	}

	void NS3DScene::removeTerrain(Ref<Terrain> terrain) {
		this->m_Terrains.remove(terrain);
	}
}

