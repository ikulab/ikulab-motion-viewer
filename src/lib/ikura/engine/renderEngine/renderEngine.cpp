#include "./renderEngine.hpp"

#include <GLFW/glfw3.h>

#include <easylogging++.h>

#include "../../ikura.hpp"


QueueFamilyIndices::QueueFamilyIndices() {
	indices.insert({
		std::make_pair(GRAPHICS, decltype(indices)::mapped_type()),
		std::make_pair(PRESENT, decltype(indices)::mapped_type())
	});
}

uint32_t QueueFamilyIndices::get(QueueIndexKey key) {
	return indices[key].value();
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

bool QueueFamilyIndices::isComplete() {
	return std::all_of(
		indices.begin(), indices.end(),
		[](const decltype(indices)::value_type value) {
			return value.second.has_value();
		}
	);
}

bool QueueFamilyIndices::isShareingIndexBetweenGraphicsAndPresent() {
	return get(GRAPHICS) == get(PRESENT);
}


RenderEngine::RenderEngine(RenderEngineInitConfig initConfig) {
	glfwInit();
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Initialized GLFW.";

	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Creating Vulkan Instance...";
	createInstance(initConfig);
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Instance has been created.";

	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Setting up Extensions...";
	setupExtensions(initConfig);
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Extensions have been setup.";

	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Creating Vulkan Device...";
	createDevice(initConfig);
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Device has been created.";
}

RenderEngine::~RenderEngine() {
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

vk::Instance RenderEngine::getInstance() const {
	return instance;
}

vk::PhysicalDevice RenderEngine::getPhysicalDevice() const {
	return physicalDevice;
}

vk::Device RenderEngine::getDevice() const {
	return device;
}

QueueFamilyIndices RenderEngine::getQueueFamilyIndices() const {
	return queueFamilyIndices;
}