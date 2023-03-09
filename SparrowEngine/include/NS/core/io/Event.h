#pragma once

#include "../Base.h"

#define NS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace ns {

	class NS_API Event {
	public:
		enum Type {
			None = 0,
			WindowClose,
			WindowResize,
			WindowFocus,
			WindowLostFocus,
			WindowMoved,
			KeyPressed, KeyReleased, KeyTyped,
			MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
		};

		enum Category {
			Window,
			Mouse,
			Keyboard
		};

		Event(Type type, Category category) {
			this->type = type;
			this->category = category;
		}

		Type type;
		Category category;

		bool isHandled() { return this->m_Handled; }

		void handled() { this->m_Handled = true; }

	protected:
		bool m_Handled = false;

	};

}
