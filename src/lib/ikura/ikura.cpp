#include "./ikura.hpp"

#include <vulkan/vulkan.hpp>
#include <string>

#include <easylogging++.h>

// Initialize Vulkan Hpp Default Dispatcher ----------
#ifndef INIT_VULKAN_HPP_DEFAULT_DISPATCH_LORDER
#	define INIT_VULKAN_HPP_DEFAULT_DISPATCH_LORDER
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

#define VMA_IMPLEMENTATION
#ifndef VK_API_VERSION_MAJOR
#	define VK_API_VERSION_MAJOR(version) (((uint32_t)(version) >> 22) & 0x7FU)
#endif
#ifndef VK_API_VERSION_MINOR
#	define VK_API_VERSION_MINOR(version) (((uint32_t)(version) >> 12) & 0x3FFU)
#endif
#ifndef VK_API_VERSION_PATCH
#	define VK_API_VERSION_PATCH(version) ((uint32_t)(version) & 0xFFFU)
#endif
#include <vk_mem_alloc.h>

void initIkura() {
	// Initialize Vulkan Hpp Default Dispatcher
	std::unique_ptr<vk::DynamicLoader> vkDynamicLoader = std::make_unique<vk::DynamicLoader>();

	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr
		= vkDynamicLoader->getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

	// Initialize Vulkan Memory Allocator
	vmaAllocator = std::make_unique<VmaAllocator>();
}