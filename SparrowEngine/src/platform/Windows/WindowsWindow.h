#pragma once

#include <NS/core/io/Window.h>

#include <NS/core/graphics/GraphicsContext.h>
#include <GLFW/glfw3.h>
#include <NS/core/deps/glm/glm.hpp>

namespace ns {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& properties);
		~WindowsWindow() override;

		void update() override;

		/// render to display
		void display() override;

		void setEventCallback(const EventCallbackFn& callback) override { m_Data.callback = callback; };

		uint32_t getWidth() const override;
		uint32_t getHeight() const override;

		const char *getTitle() const override;

		const void *getHandle() const override;

		const float getMouseDX() const override;
		const float getMouseDY() const override;
	protected:

		static void errorCallback(int error, const char *description);

	private:
		GLFWwindow *m_Window;

		Scope<GraphicsContext> m_Context;

		struct WindowData {
			int width;
			int height;
			std::string title;
			glm::vec2 lastMousePosition;
			glm::vec2 mouseDeltaPosition;
			EventCallbackFn callback;
		};

		WindowData m_Data;

		static bool s_GLFWInitialized;


	};

}