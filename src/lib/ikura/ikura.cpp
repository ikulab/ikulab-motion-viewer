#include "./ikura.hpp"

#include <vulkan/vulkan.hpp>
#include <string>

#include <easylogging++.h>

// Initialize Vulkan Hpp Default Dispatcher ----------
#ifndef INIT_VULKAN_HPP_DEFAULT_DISPATCH_LORDER
#	define INIT_VULKAN_HPP_DEFAULT_DISPATCH_LORDER
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

void initIkura() {
	// Initialize Vulkan Hpp Default Dispatcher
	std::unique_ptr<vk::DynamicLoader> vkDynamicLoader = std::make_unique<vk::DynamicLoader>();

	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr
		= vkDynamicLoader->getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
}