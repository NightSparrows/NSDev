#pragma once

#include "LayerStack.h"

namespace ns {

	// You can create your own layer for each GameState
	// The function of layers just seperate the drawing stuff
	// likes GUI will always on the top of the layerstack
	class NS_API MasterLayer {
	public:
		MasterLayer();
		~MasterLayer();

		void onUpdate(Time delta);
		void onEvent(Event& event);

		void pushLayer(Layer *layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer *layer);
		void popOverlay(Layer *layer);

	private:
		Scope<LayerStack> m_Stack;
	};

}
