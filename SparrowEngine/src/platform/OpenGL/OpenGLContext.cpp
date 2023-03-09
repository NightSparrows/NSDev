
#include <glad/glad.h>
#include <NS/core/Log.h>
#include "OpenGLContext.h"

namespace ns {

	OpenGLContext::OpenGLContext(GLFWwindow *window) :
		m_Handle(window)
	{
		NS_CORE_ASSERT((window != nullptr), "The window handle is null.");
	}

	void OpenGLContext::init() {
		glfwMakeContextCurrent(this->m_Handle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NS_CORE_ASSERT(status, "Failed to initialize Glad.");

#ifdef NS_DEBUG
		NS_CORE_INFO("OpenGL Info:");
		NS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		NS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		NS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
#endif
		//NS_CORE_ASSERT((GLVersion.major > 4 || (GLVersion.major >= 4 && GLVersion.minor >= 5)), "SparrowEngine requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(this->m_Handle);
	}
}
