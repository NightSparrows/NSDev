#pragma once

#include <string>
#include <functional>
#include "Event.h"

namespace ns {

	/// Contain all properties when window is create.
	struct WindowProperties
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		bool resizable;

		bool vsync = true;

		WindowProperties(const std::string& title = "Sparrow Engine", uint32_t width = 1280, uint32_t height = 720, bool resizable = false) {
			this->title = title;
			this->width = width;
			this->height = height;
			this->resizable = resizable;
		}
	};

	class NS_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		/// Updating on every time
		virtual void update() = 0;

		/// render to display
		virtual void display() = 0;
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual const char *getTitle() const = 0;

		virtual const void *getHandle() const = 0;

		virtual const float getMouseDX() const = 0;
		virtual const float getMouseDY() const = 0;

		/// Create by each platform
		static Scope<Window> Create(const WindowProperties &properties);

	};

}

