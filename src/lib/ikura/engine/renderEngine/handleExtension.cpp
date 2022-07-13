#include "./renderEngine.hpp"

#include <vulkan/vulkan.hpp>

#include <easylogging++.h>

#include "../../ikura.hpp"

/**
 * @brief debug callback for ValidationLayer. prints error content e.g. error message.
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL validationLayerDebugCallback(
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
VkDebugUtilsMessengerCreateInfoEXT RenderEngine::getDebugUtilsMessengerCI() {
	VkDebugUtilsMessengerCreateInfoEXT ci{};

	ci.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	ci.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	ci.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	ci.pfnUserCallback = validationLayerDebugCallback;

	return ci;
}

/**
 * @brief Create Extension object from InstanceProcAddr.
 * e.g. VkDebugUtilsMessengerEXT.
 */
template<typename ExtensionType, typename ExtensionCreateInfoType, typename PfnType>
VkResult CreateExtensionObjectFromInstanceProcAddr(
	VkInstance instance,
	const ExtensionCreateInfoType* pCreateinfo,
	const VkAllocationCallbacks* pAllocator,
	ExtensionType* pExtensionObject,
	const char* createFunctionName) {

	auto func = (PfnType)vkGetInstanceProcAddr(instance, createFunctionName);

	if (func != nullptr) {
		return func(instance, pCreateinfo, pAllocator, pExtensionObject);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

/**
 * @brief Wrapper of CreateExtensionObjectFromInstanceProcAddr.
 * Creates DebugUtilsMessengerEXT
 */
VkResult CreateDebugUtilsMessengerEXT(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateinfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger) {

	return CreateExtensionObjectFromInstanceProcAddr<
		VkDebugUtilsMessengerEXT, VkDebugUtilsMessengerCreateInfoEXT, PFN_vkCreateDebugUtilsMessengerEXT
	>(
		instance, pCreateinfo, pAllocator, pDebugMessenger, "vkCreateDebugUtilsMessengerEXT"
	);
}

/**
 * @brief Destroy Extension object from InstanceProcAddr.
 * e.g. destroy VkDebugUtilsMessengerEXT.
 */
template<typename ExtensionType, typename PfnType>
void DestroyExtensionObjectFromInstanceProcAddr(
	VkInstance instance,
	ExtensionType extensionObject,
	const VkAllocationCallbacks* pAllocator,
	const char* destroyFunctionName) {

	auto func = (PfnType)vkGetInstanceProcAddr(instance, destroyFunctionName);

	if (func != nullptr) {
		func(instance, extensionObject, pAllocator);
	}
	else {
		throw std::runtime_error(
			"Cannot get destroy function from InstanceProcAddr."
		);
	}
}

/**
 * @brief Wrapper of DescroyExtensionObjectFromInstanceProcAddr.
 * Destroys DebugUtilsMessengerEXT
 */
void DestroyDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator) {

	return DestroyExtensionObjectFromInstanceProcAddr<
		VkDebugUtilsMessengerEXT, PFN_vkDestroyDebugUtilsMessengerEXT
	>(
		instance, debugMessenger, pAllocator, "vkDestroyDebugUtilsMessengerEXT"
	);
}

/**
 * @brief setup Extensions: DebugUtilsMessengerEXT
 */
void RenderEngine::setupExtensions(RenderEngineInitConfig initConfig) {
	if (!isValidationLayerEnabled) return;

	VkDebugUtilsMessengerCreateInfoEXT debugCI = getDebugUtilsMessengerCI();
	CheckError(
		CreateDebugUtilsMessengerEXT(instance, &debugCI, nullptr, &debugMessenger),
		"Failed to set up VkDebugUtilsMessengerEXT."
	);
}

void RenderEngine::destroyExtensions() {
	if (!isValidationLayerEnabled) return;

	DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
}