
#include <GLFW/glfw3.h>
#include <NS/core/GameEngine.h>
#include <NS/core/io/Keyboard.h>

namespace ns {

	bool Keyboard::isKeyDown(const KeyCode code) {
		GLFWwindow *window = (GLFWwindow*)(GameEngine::GetWindow()->getHandle());
		auto state = glfwGetKey(window, static_cast<int32_t>(code));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

}
