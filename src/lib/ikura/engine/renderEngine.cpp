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
	std::for_each(
		initConfig.validationLayerNames.begin(),
		initConfig.validationLayerNames.end(),
		[&](const std::string& s) {
			validationLayerNames.push_back(s.data());
		}
	);
	std::for_each(
		initConfig.extensionNames.begin(),
		initConfig.extensionNames.end(),
		[&](const std::string& s) {
			extensionNames.push_back(s.data());
		}
	);

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

	bool notFound = false;
	std::for_each(
		availableLayers.begin(),
		availableLayers.end(),
		[&notFound]() {
			if 
			notFound = true;
		}
	);

    // for (const char* layerName : validationLayers) {
    //     bool layerFound = false;

    //     for (const auto& layerProperties : availableLayers) {
    //         if (strcmp(layerName, layerProperties.layerName) == 0) {
    //             layerFound = true;
    //             break;
    //         }
    //     }

    //     if (!layerFound) {
    //         return false;
    //     }
    // }



    return true;
	// if (!checkValidationLayerSupport(validationLayerNames)) {
	// 	throw std::runtime_error(
	// 		"ValidationLayer activation is requested, "
	// 		"but givin ValidationLayers are NOT supported. "
	// 		"Make sure RenderEngineInitConfig.validationLayerNames is correct."
	// 	);
	// }

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

const char** stringVectorToCharPp(std::vector<std::string> vec) {

}