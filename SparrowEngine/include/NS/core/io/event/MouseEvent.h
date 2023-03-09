#pragma once

#include "../Event.h"

namespace ns {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(int x, int y) : Event(MouseMoved, Mouse) {
			this->x = x;
			this->y = y;
		}

		int x, y;
	};

	class MouseButtonPressedEvent : public Event {
	public:
		MouseButtonPressedEvent(int button, bool repeat) : Event(MouseButtonPressed, Mouse) {
			this->button = button;
			this->repeat = repeat;
		}

		int button;
		bool repeat;
	};

	class MouseButtonReleasedEvent : public Event {
	public:
		MouseButtonReleasedEvent(int button) : Event(MouseButtonReleased, Mouse) {
			this->button = button;
		}

		int button;
	};

	// Scrolling
	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : Event(MouseScrolled, Mouse) {
			this->xOffset = xOffset;
			this->yOffset = yOffset;
		}

		float xOffset, yOffset;
	};

}
