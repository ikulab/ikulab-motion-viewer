#include "./renderEngine.hpp"

#include <vulkan/vulkan.hpp>

#include <easylogging++.h>

#include "../../ikura.hpp"

/**
 * @brief debug callback for ValidationLayer. prints error content e.g. error message.
 */
static VKAPI_ATTR vk::Bool32 VKAPI_CALL validationLayerDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	LOG(WARNING) << "ValidationLayer message: " << pCallbackData->pMessage;
	return VK_FALSE;
}

/**
 * @brief Populates VkDebugUtilsMessengerCreateInfoEXT and return it.
 */
vk::DebugUtilsMessengerCreateInfoEXT RenderEngine::getDebugUtilsMessengerCI() {
	vk::DebugUtilsMessengerCreateInfoEXT ci;

	ci.messageSeverity =
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
	ci.messageType =
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
	ci.pfnUserCallback = validationLayerDebugCallback;

	return ci;
}

/**
 * @brief setup Extensions: DebugUtilsMessengerEXT
 */
void RenderEngine::setupExtensions(RenderEngineInitConfig initConfig) {
	if (isValidationLayerEnabled) {
		auto debugCI = getDebugUtilsMessengerCI();
		debugMessenger = instance.createDebugUtilsMessengerEXT(debugCI);
	}
}

/**
 * @brief destroy Extensions: DebugUtilsMessengerEXT
 */
void RenderEngine::destroyExtensions() {
	if (!isValidationLayerEnabled) {
		instance.destroyDebugUtilsMessengerEXT(debugMessenger);
	}
}