

#include <NS/core/Log.h>
#include <NS/core/GameEngine.h>
#include "VulkanContext.h"

#ifdef NS_DEBUG
const std::vector<const char*> validationLayer = {
	"VK_LAYER_KHRONOS_validation"
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}
#endif

namespace ns {

	VulkanContext::VulkanContext(GLFWwindow *window) :
		m_Handle(window)
	{
		NS_CORE_ASSERT((window != nullptr), "The window handle is null.");
	}

	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef NS_DEBUG
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

		return extensions;
	}

	void VulkanContext::init() {
		glfwMakeContextCurrent(this->m_Handle);

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = GameEngine::GetWindow()->getTitle();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "NS Game Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef NS_DEBUG

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		createInfo.enabledLayerCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledLayerNames = validationLayer.data();
		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

#else
		createInfo.enabledLayerCount = 0;		// 不知道是幹嘛用的，放著
#endif

		// create instance
		if (vkCreateInstance(&createInfo, nullptr, &this->m_instance) != VK_SUCCESS) {
			NS_CORE_ERROR("Failed to create vulkan context!");
			NS_CORE_ASSERT(false, "Failed to initialize Vulkan.");
		}
		// Create Debug Messenger
		this->setupDebugMessenger();

		// Graphic card support
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			NS_CORE_ERROR("Failed to support vulkan!");
			NS_CORE_ASSERT(false, "No Graphic card support vulkan");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, devices.data());

		// Just pick the first device to load ::TODO find greater card
		for (const auto device : devices) {
			if (isDeviceSuitable(device)) {
				physicalDevice = device;
				break;
			}
		}
		

		NS_CORE_INFO("Vulkan Info:");
		/*
		TODO
		NS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		NS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		NS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
		*/

	}

#ifdef NS_DEBUG

	// Debug callback
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		
		NS_CORE_ERROR("Validation layer: {0}", pCallbackData->pMessage);

		return VK_FALSE;
	}

	void VulkanContext::setupDebugMessenger() {
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(this->m_instance, &createInfo, nullptr, &this->m_debugMessenger) != VK_SUCCESS) {
			NS_CORE_ASSERT(false, "Failed to setup debug messenger!");
		}
	}

	void VulkanContext::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}
#endif

	bool VulkanContext::isDeviceSuitable(VkPhysicalDevice device) {
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if (!deviceFeatures.geometryShader) {
			return false;
		}

		return true;
	}

	void VulkanContext::swapBuffers() {
		glfwSwapBuffers(this->m_Handle);
	}

	void VulkanContext::cleanUp() {
		vkDestroyInstance(this->m_instance, nullptr);
	}

}