#pragma once

#include "../core/layer/Layer.h"

namespace ns {

	class NS_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate(Time time) override;
		void onEvent(Event& event) override;

		// Just Test
		void renderGUI();

		// Start to draw GUI stuff
		void BeginFrame();

		// Render the stuff you put in.
		void EndFrame();

	private:
		float m_DeltaTime;

	};

}
