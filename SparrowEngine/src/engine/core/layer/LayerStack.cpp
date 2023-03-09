
#include <NS/core/layer/LayerStack.h>

namespace ns {

	LayerStack::LayerStack() {
		this->m_LayerInsert = this->m_Layers.begin();
	}

	LayerStack::~LayerStack() {
		for (Layer *layer : this->m_Layers)
			delete layer;
	}

	void LayerStack::pushLayer(Layer *layer) {
		this->m_LayerInsert = this->m_Layers.emplace(this->m_LayerInsert, layer);
	}

	void LayerStack::pushOverlay(Layer *overlay) {
		this->m_Layers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Layer *layer) {
		auto it = std::find(this->m_Layers.begin(), this->m_Layers.end(), layer);
		if (it != this->m_Layers.end()) {
			this->m_Layers.erase(it);
			this->m_LayerInsert--;
		}
	}

	void LayerStack::popOverlay(Layer *overlay) {
		auto it = std::find(this->m_Layers.begin(), this->m_Layers.end(), overlay);
		if (it != this->m_Layers.end()) {
			this->m_Layers.erase(it);
		}
	}
}
