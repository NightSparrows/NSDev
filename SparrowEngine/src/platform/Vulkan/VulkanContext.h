#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <NS/core/graphics/GraphicsContext.h>

namespace ns {

	class NS_API VulkanContext : public GraphicsContext {
	public:
		VulkanContext(GLFWwindow *window);

		void init() override;
		void swapBuffers() override;
		void cleanUp() override;

	private:
		bool isDeviceSuitable(VkPhysicalDevice device);

#ifdef NS_DEBUG
		void setupDebugMessenger();
#endif

	private:
		GLFWwindow *m_Handle;
		VkInstance m_instance;

#ifdef NS_DEBUG
		VkDebugUtilsMessengerEXT m_debugMessenger;
#endif
	};

}
