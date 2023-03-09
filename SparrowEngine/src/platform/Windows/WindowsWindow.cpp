#include "WindowsWindow.h"
#include <NS/core/Log.h>
#include <NS/core/io/Mouse.h>
#include <NS/core/io/event/MouseEvent.h>
#include <NS/core/io/event/WindowEvent.h>
#include <NS/core/io/event/KeyEvent.h>

namespace ns {

	bool WindowsWindow::s_GLFWInitialized = false;

	WindowsWindow::WindowsWindow(const WindowProperties & properties)
	{
		this->m_Data.width = properties.width;
		this->m_Data.height = properties.height;
		this->m_Data.title = properties.title;

		glfwSetErrorCallback(errorCallback);

		if (!s_GLFWInitialized) {
			if (glfwInit() != GLFW_TRUE) {
				NS_CORE_ERROR("Failed to initialize GLFW.");
			}
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_RESIZABLE, properties.resizable ? GLFW_TRUE : GLFW_FALSE);

		// test
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef NS_RENDER_API_VULKAN
		// Because GLFW is default to openGL
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

		this->m_Window = glfwCreateWindow(this->m_Data.width, this->m_Data.height, this->m_Data.title.c_str(), nullptr, nullptr);

		if (this->m_Window == GLFW_FALSE) {
			NS_CORE_ERROR("Failed to create window.");
		}

		// context initialize
		this->m_Context = GraphicsContext::Create(this->m_Window);
		this->m_Context->init();
		if (properties.vsync) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		// Events
		glfwSetWindowUserPointer(this->m_Window, &this->m_Data);

		// Mouse
		glfwSetCursorPosCallback(this->m_Window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			glm::vec2 currentPosition = { (float)x, (float)y };
			data.mouseDeltaPosition = currentPosition - data.lastMousePosition;
			data.lastMousePosition = currentPosition;

			MouseMovedEvent event((int)x, (int)y);
			data.callback(event);
		});

		glfwSetMouseButtonCallback(this->m_Window, [](GLFWwindow *window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button, false);
				data.callback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.callback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				MouseButtonPressedEvent event(button, true);
				data.callback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(this->m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.callback(event);
		});

		// Window
		glfwSetWindowCloseCallback(this->m_Window, [](GLFWwindow *window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			/// 雖然很醜，但GCC就是有問題= =
			WindowCloseEvent *event = new WindowCloseEvent();
			data.callback(*event);
			delete event;
		});

		glfwSetWindowSizeCallback(this->m_Window, [](GLFWwindow *window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.callback(event);
		});

		glfwSetWindowFocusCallback(this->m_Window, [](GLFWwindow *window, int focus) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowFocusEvent event(focus == GLFW_TRUE);
			data.callback(event);
		});

		glfwSetWindowPosCallback(this->m_Window, [](GLFWwindow *window, int x, int y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowMovedEvent event(x, y);
			data.callback(event);
		});

		// Keyboard
		glfwSetKeyCallback(this->m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false);
				data.callback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.callback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				data.callback(event);
				break;
			}
			}

		});

		glfwSetCharCallback(this->m_Window, [](GLFWwindow *window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.callback(event);
		});
	}

	WindowsWindow::~WindowsWindow() {
		this->m_Context->cleanUp();
		if (this->m_Window) {
			glfwDestroyWindow(this->m_Window);
		}
		if (s_GLFWInitialized) {
			glfwTerminate();
			s_GLFWInitialized = false;
		}
	}

	void WindowsWindow::update() {
		glfwPollEvents();
	}

	void WindowsWindow::display()
	{
		this->m_Context->swapBuffers();
	}

	uint32_t WindowsWindow::getWidth() const {
		return this->m_Data.width;
	}

	uint32_t WindowsWindow::getHeight() const {
		return this->m_Data.height;
	}

	const char *WindowsWindow::getTitle() const {
		return this->m_Data.title.c_str();
	}

	const void *WindowsWindow::getHandle() const {
		return this->m_Window;
	}

	const float WindowsWindow::getMouseDX() const {
		return this->m_Data.mouseDeltaPosition.x;
	}

	const float WindowsWindow::getMouseDY() const {
		return this->m_Data.mouseDeltaPosition.y;
	}

	void WindowsWindow::errorCallback(int error, const char *description) {
		NS_CORE_WARN("GLFW Error ({0}): {1}", error, description);
	}

}