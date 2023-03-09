
#include <NS/core/layer/MasterLayer.h>

namespace ns {

	MasterLayer::MasterLayer() {
		this->m_Stack = CreateScope<LayerStack>();
	}

	MasterLayer::~MasterLayer() {
	}

	void MasterLayer::onUpdate(Time delta) {
		for (Layer *layer : *this->m_Stack) {
			layer->onUpdate(delta);
		}
	}

	void MasterLayer::onEvent(Event& e) {
		// 從最上面的開始
		for (auto it = this->m_Stack->end(); it != this->m_Stack->begin();) {
			(*--it)->onEvent(e);
			if (e.isHandled())
				break;
		}
	}

	void MasterLayer::pushLayer(Layer *layer) {
		this->m_Stack->pushLayer(layer);
		layer->onAttach();
	}

	void MasterLayer::pushOverlay(Layer *overlay) {
		this->m_Stack->pushOverlay(overlay);
		overlay->onAttach();
	}

	void MasterLayer::popLayer(Layer *layer) {
		this->m_Stack->popLayer(layer);
		layer->onDetach();
	}

	void MasterLayer::popOverlay(Layer *overlay) {
		this->m_Stack->popOverlay(overlay);
		overlay->onDetach();
	}
}
