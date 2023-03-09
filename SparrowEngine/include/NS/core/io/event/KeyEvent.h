#pragma once

#include "../Event.h"

namespace ns {

	class KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(int key, bool repeat) : Event(KeyPressed, Keyboard) {
			this->key = key;
			this->repeat = repeat;
		}

		int key;
		bool repeat;
	};

	class KeyReleasedEvent : public Event {
	public:
		KeyReleasedEvent(int key) : Event(KeyReleased, Keyboard) {
			this->key = key;
		}

		int key;
	};

	class KeyTypedEvent : public Event {
	public:
		KeyTypedEvent(int keycode) : Event(KeyTyped, Keyboard) {
			this->keycode = keycode;
		}

		int keycode;
	};

}
