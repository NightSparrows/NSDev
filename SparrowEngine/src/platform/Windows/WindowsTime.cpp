//
// Created by ªô¤l·¶ on 2020/10/9.
//

#include <glfw/glfw3.h>
#include <NS/core/io/Time.h>

namespace ns {

	float Time::GetTime() {
		return (float) glfwGetTime();
	}

}

