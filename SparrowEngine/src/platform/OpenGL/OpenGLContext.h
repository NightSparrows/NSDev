#pragma once

#include <GLFW/glfw3.h>
#include <NS/core/graphics/GraphicsContext.h>

namespace ns {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow *window);

		void init() override;
		void swapBuffers() override;
	private:
		GLFWwindow *m_Handle;
	};

}

