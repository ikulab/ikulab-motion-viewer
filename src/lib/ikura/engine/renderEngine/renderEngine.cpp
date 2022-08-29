#include "./renderEngine.hpp"

#include <GLFW/glfw3.h>

#include <easylogging++.h>

#define VMA_IMPLEMENTATION
// for compability
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

#include "../../ikura.hpp"


QueueFamilyIndices::QueueFamilyIndices() {
	indices.insert({
		std::make_pair(GRAPHICS, decltype(indices)::mapped_type()),
		std::make_pair(PRESENT, decltype(indices)::mapped_type())
	});
}

uint32_t QueueFamilyIndices::get(const QueueIndexKey key) const {
	return indices.at(key).value();
}

void QueueFamilyIndices::set(QueueIndexKey key, uint32_t value) {
	indices.insert_or_assign(key, value);
}

std::set<uint32_t> QueueFamilyIndices::generateUniqueSet() {
	std::set<uint32_t> result;
	for (const auto& value : indices) {
		if (value.second.has_value()) {
			result.insert(value.second.value());
		}
	}

	return result;
}

bool QueueFamilyIndices::isComplete() const {
	return std::all_of(
		indices.begin(), indices.end(),
		[](const decltype(indices)::value_type value) {
			return value.second.has_value();
		}
	);
}

bool QueueFamilyIndices::isShareingIndexBetweenGraphicsAndPresent() const {
	return get(GRAPHICS) == get(PRESENT);
}


RenderEngine::RenderEngine(RenderEngineInitConfig initConfig) {
	glfwInit();
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Initialized GLFW.";

	this->initConfig = initConfig;
	vmaAllocator = std::make_shared<VmaAllocator>();
}

RenderEngine::~RenderEngine() {
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Destroying VmaAllocator...";
	vmaDestroyAllocator(*vmaAllocator);
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "VmaAllocator has been Destroyed.";

	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Destroying Vulkan Device...";
	device.destroy();
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Device has been destroyed.";

	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Destroying Vulkan Extensions...";
	destroyExtensions();
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Extensions have been destroyed.";

	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Destroying Vulkan Instance...";
	instance.destroy();
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Instance has been destroyed.";

	glfwTerminate();
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Terminated GLFW.";
}

const vk::Instance RenderEngine::getInstance() const {
	return instance;
}

const vk::PhysicalDevice RenderEngine::getPhysicalDevice() const {
	return physicalDevice;
}

const vk::Device RenderEngine::getDevice() const {
	return device;
}

const RenderEngineInfo RenderEngine::getEngineInfo() const {
	return engineInfo;
}

const QueueFamilyIndices RenderEngine::getQueueFamilyIndices() const {
	return queueFamilyIndices;
}

void RenderEngine::setSampleSurface(vk::SurfaceKHR surface) {
	this->sampleSurface = surface;
}


RenderEngineInitConfig RenderEngineInitConfig::defaultDebugSetting() {
	RenderEngineInitConfig initConfig = defaultCommonSetting();

	initConfig.layerNames.push_back(VALIDATION_LAYER_NAME);

	initConfig.instanceExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	initConfig.instanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

	initConfig.deviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	return initConfig;
}

RenderEngineInitConfig RenderEngineInitConfig::defaultCommonSetting() {
	RenderEngineInitConfig initConfig;
	initConfig.applicationName = "Ikura Application";
	initConfig.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	initConfig.suitablePhysicalDevicePicker = RenderEngine::getSuitablePhysicalDeviceInfo;

	return initConfig;
}