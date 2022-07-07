#include "./renderEngine.hpp"

#include <vector>
#include <string>
#include <cstring>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
extern "C" {
#include <logc/log.h>
}

#include "../definition/errorUtil.hpp"

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

RenderEngine::RenderEngine(RenderEngineInitConfig renderConfig) {
	if (renderConfig.enableValidationLayer) {
		if (renderConfig.validationLayerNames.size() == 0) {
			THROW_RT_ERR(
				"ValidationLayer activation is requested, "
				"but RenderEngineInitConfig.validationLayerNames is empty."
			);
		}

		if (!checkValidationLayerSupport(renderConfig.validationLayerNames)) {
			THROW_RT_ERR(
				"ValidationLayer activation is requested, "
				"but givin ValidationLayers are NOT supported.\n"
				"Make sure RenderEngineInitConfig.validationLayerNames is correct."
			);
		}

		log_info("ValidationLayer activated.");
	}

	supportInfo.isGlfwSupported = (glfwVulkanSupported() == GLFW_TRUE);
}