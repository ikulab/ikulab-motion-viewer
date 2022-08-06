#include "./ikura.hpp"

#include <vulkan/vulkan.hpp>
#include <string>

#include <easylogging++.h>
#include <magic_enum.hpp>

// Initialize Vulkan Hpp Default Dispatcher ----------
#ifndef INIT_VULKAN_HPP_DEFAULT_DISPATCH_LORDER
#	define INIT_VULKAN_HPP_DEFAULT_DISPATCH_LORDER
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

void initIkura() {
	// Initialize Vulkan Hpp Default DIspatcher
	vk::DynamicLoader dl;
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr
		= dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
}

void CheckError(VkResult result, std::string failMsg) {
	if (result == VK_SUCCESS) {
		return;
	}

	failMsg += " VkResult: ";
	failMsg += magic_enum::enum_name(result);

	LOG(FATAL) << failMsg;
}