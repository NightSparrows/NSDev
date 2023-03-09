#pragma once

#include <vector>
#include "Layer.h"

namespace ns {

	class NS_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer *layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer *layer);
		void popOverlay(Layer *layer);

		std::vector<Layer*>::iterator begin() { return this->m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return this->m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;

	};

}
