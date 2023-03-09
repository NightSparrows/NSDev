#pragma once

#include "../Event.h"

namespace ns {

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() : Event(Event::WindowClose, Event::Window) { }
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height) : Event(WindowResize, Window) {
			this->width = width;
			this->height = height;
		}

		int width, height;
	};

	class WindowFocusEvent : public Event {
	public:
		WindowFocusEvent(bool focus) : Event(focus ? WindowFocus : WindowLostFocus, Window) { }
	};

	class WindowMovedEvent : public Event {
	public:
		WindowMovedEvent(int x, int y) : Event(WindowMoved, Window) {
			this->x = x;
			this->y = y;
		}

		int x, y;
	};

}
