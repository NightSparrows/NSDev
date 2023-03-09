
#include <GLFW/glfw3.h>
#include <NS/core/GameEngine.h>
#include <NS/core/io/Mouse.h>

namespace ns {

	bool Mouse::IsMouseButtonDown(Button button) {
		GLFWwindow *window = (GLFWwindow*)(GameEngine::GetWindow()->getHandle());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	glm::vec2 Mouse::GetPosition() {
		GLFWwindow *window = (GLFWwindow*)(GameEngine::GetWindow()->getHandle());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	void Mouse::SetPosition(int x, int y) {
		GLFWwindow *window = (GLFWwindow*)(GameEngine::GetWindow()->getHandle());
		glfwSetCursorPos(window, x, y);
	}

	float Mouse::GetDX() {
		return GameEngine::GetWindow()->getMouseDX();
	}

	float Mouse::GetDY() {
		return GameEngine::GetWindow()->getMouseDY();
	}

	void Mouse::Grab(bool grab) {
		GLFWwindow *window = (GLFWwindow*)(GameEngine::GetWindow()->getHandle());
		glfwSetInputMode(window, GLFW_CURSOR, grab ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	bool Mouse::IsGrab() {
		GLFWwindow *window = (GLFWwindow*)(GameEngine::GetWindow()->getHandle());
		return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
	}
}
