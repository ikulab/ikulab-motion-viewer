#include "./renderEngine.hpp"

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <easylogging++.h>

#include "../ikura.hpp"

bool checkValidationLayerSupport(const std::vector<std::string> layerNames) {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const std::string layerName : layerNames) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (layerName == layerProperties.layerName) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

RenderEngine::RenderEngine(RenderEngineInitConfig initConfig) {
	VkInstanceCreateInfo instanceCI{};

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
	validationLayerNames = initConfig.validationLayerNames;
	extensionNames = initConfig.extensionNames;
	{
		auto glfwReqExts = getGlfwRequiredExtensions();
		std::for_each(glfwReqExts.begin(), glfwReqExts.end(), [&](std::string ext) {
			if (std::find(extensionNames.begin(), extensionNames.end(), ext) == extensionNames.end()) {
				extensionNames.push_back(ext);
			}
		});
	}

	// support check ----------
	enableValidationLayers = initConfig.enableValidationLayers;
	if (enableValidationLayers) {
		checkValidationLayersSupport(validationLayerNames);
	}
	supportInfo.isGlfwSupported = (glfwVulkanSupported() == GLFW_TRUE);

	// ValidationLayer activation ----------
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers) {
		instanceCI.enabledLayerCount = static_cast<uint32_t>(validationLayerNames.size());
		instanceCI.ppEnabledLayerNames = validationLayerNames.data();

		// populateDebugMessengerCreateInfo(debugCreateInfo);
		instanceCI.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		instanceCI.enabledLayerCount = 0;
		instanceCI.pNext = nullptr;
	}
}

/**
 * @brief Check if all of givin ValidationLayers are supported.
 *
 * @exception std::runtime_error if ValidationLayer is not supported or paramater is invalid.
 */
void checkValidationLayersSupport(std::vector<std::string> validationLayerNames) {
	if (validationLayerNames.size() == 0) {
		throw std::runtime_error(
			"ValidationLayer activation is requested, "
			"but RenderEngineInitConfig.validationLayerNames is empty."
		);
	}

	if (!checkValidationLayerSupport(validationLayerNames)) {
		throw std::runtime_error(
			"ValidationLayer activation is requested, "
			"but givin ValidationLayers are NOT supported. "
			"Make sure RenderEngineInitConfig.validationLayerNames is correct."
		);
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