#include "./renderEngine.hpp"

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <easylogging++.h>

#include "../../ikura.hpp"

// forward declearation of helper functions ----------
void checkValidationLayersSupport(std::vector<const char*> validationLayerNames);
std::vector<std::string> getGlfwRequiredExtensions();


/**
 * @brief Populates CreateInfo, checks supports, and creates VulkanInstance.
 */
void RenderEngine::createInstance(RenderEngineInitConfig initConfig) {
	VkInstanceCreateInfo instanceCI{};
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	// application info ----------
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = initConfig.applicationName;
	appInfo.applicationVersion = initConfig.applicationVersion;
	appInfo.pEngineName = IKURA_APP_INFO_ENGINE_NAME;
	appInfo.engineVersion = IKURA_APP_INFO_ENGINE_VER;
	appInfo.apiVersion = IKURA_APP_INFO_API_VER;
	instanceCI.pApplicationInfo = &appInfo;

	// set ValidationLayer / Extension names ----------
	for (const auto& name : initConfig.validationLayerNames) {
		validationLayerNames.push_back(name.data());
	}
	for (const auto& name : initConfig.extensionNames) {
		extensionNames.push_back(name.data());
	}
	// for GLFW
	{
		auto glfwReqExts = getGlfwRequiredExtensions();
		std::for_each(
			glfwReqExts.begin(),
			glfwReqExts.end(),
			[&](std::string ext) {
				auto f = std::find(
					extensionNames.begin(),
					extensionNames.end(),
					ext
				);
				if (f == extensionNames.end()) {
					extensionNames.push_back(ext.data());
				}
			}
		);
	}

	// ValidationLayer / Extension info population ----------
	instanceCI.enabledLayerCount = static_cast<uint32_t>(validationLayerNames.size());
	instanceCI.ppEnabledLayerNames = validationLayerNames.data();
	instanceCI.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size());
	instanceCI.ppEnabledExtensionNames = extensionNames.data();

	// support check ----------
	// NOTE: Extensions support is depends on device. Check later.
	// ValidationLayers
	enableValidationLayers = initConfig.enableValidationLayers;
	if (enableValidationLayers) {
		checkValidationLayersSupport(validationLayerNames);
	}
	// Glfw
	// terminate program when GlfwNativeWindow creation is requested.
	supportInfo.isGlfwSupported = (glfwVulkanSupported() == GLFW_TRUE);
	if (supportInfo.isGlfwSupported) {
		LOG(INFO) << "Glfw is supported.";
	}
	else {
		LOG(WARNING) << "Glfw is NOT supported.";
	}

	// ValidationLayer activation setting ----------
	if (enableValidationLayers) {
		instanceCI.enabledLayerCount = static_cast<uint32_t>(validationLayerNames.size());
		instanceCI.ppEnabledLayerNames = validationLayerNames.data();
		VkDebugUtilsMessengerCreateInfoEXT debugCI = getDebugUtilsMessengerCI();

		instanceCI.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCI;
	}
	else {
		instanceCI.enabledLayerCount = 0;
		instanceCI.pNext = nullptr;
	}

	CheckError(
		vkCreateInstance(&instanceCI, nullptr, &instance),
		"Failed to create Vulkan Instance."
	);
}

/**
 * @brief Check if all of givin ValidationLayers are supported.
 * Prints INFO log and can throw runtime_error.
 *
 * @exception std::runtime_error if ValidationLayer is not supported or paramater is invalid.
 */
void checkValidationLayersSupport(std::vector<const char*> validationLayerNames) {
	if (validationLayerNames.size() == 0) {
		throw std::runtime_error(
			"ValidationLayer activation is requested, "
			"but RenderEngineInitConfig.validationLayerNames is empty."
		);
	}

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	bool supported = true;
	for (const auto& layerName : validationLayerNames) {
		supported = std::any_of(
			availableLayers.begin(),
			availableLayers.end(),
			[&layerName](const VkLayerProperties& prop) {
				return std::strcmp(layerName, prop.layerName) == 0;
			}
		);

		if (!supported) {
			std::string msg;
			msg += "ValidationLayer activation is requested, but '";
			msg += layerName;
			msg += "' is NOT supported.";
			msg += "Make sure ValidationLayer name is correct.";
			throw std::runtime_error(msg);
		}
	}

	LOG(INFO) << "ValidationLayer is supported.";
}

/**
 * @brief Returns list of extension names that Glfw requires.
 */
std::vector<std::string> getGlfwRequiredExtensions() {
	uint32_t extCount;
	const char** exts = glfwGetRequiredInstanceExtensions(&extCount);

	std::vector<std::string> result;
	for (size_t i = 0; i < extCount; i++) {
		result.push_back(exts[i]);
	}

	return result;
}
