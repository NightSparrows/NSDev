#pragma once

#include <string>

#include "../Base.h"
#include "../io/Time.h"
#include "../io/Event.h"

namespace ns {

	class NS_API Layer {

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() { };
		virtual void onDetach() { };
		virtual void onUpdate(Time time) { };
		virtual void onEvent(Event& event) { }

		virtual void onRender() { };

		inline const std::string& getName() const { return this->m_Name; }

	protected:
		std::string m_Name;
	};

}
