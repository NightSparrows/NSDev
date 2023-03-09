
#include <NS/core/graphics/GraphicsContext.h>

#ifdef NS_RENDER_API_OPENGL
#include <platform/OpenGL/OpenGLContext.h>

#elif NS_RENDER_API_VULKAN
#include <platform/Vulkan/VulkanContext.h>


#endif



namespace ns {

	Scope<GraphicsContext> GraphicsContext::Create(void *window) {
		
		// Just for OpenGL
#ifdef NS_RENDER_API_OPENGL
		return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));

#elif NS_RENDER_API_VULKAN
		return CreateScope<VulkanContext>(static_cast<GLFWwindow*>(window));
#endif
	}

}
