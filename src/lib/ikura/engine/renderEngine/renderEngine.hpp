#pragma once

#include <vector>
#include <set>
#include <string>
#include <functional>

#include <vulkan/vulkan.hpp>

#include "../../renderComponent/renderContent.hpp"
#include "../../renderComponent/renderTarget.hpp"
#include "../../ikura.hpp"

// forward declearation
struct RenderEngineInitConfig;

struct RenderEngineInfo {
	struct SupportInfo {
		bool isGlfwSupported;
	} support;

	struct QueueFamilyInfo {
		bool isGraphicsAndPresentSameIndex;
	} queueFamily;

	struct LimitInfo {
		vk::SampleCountFlagBits maxMsaaSamples;
	} limit;
};


class QueueFamilyIndices {
	typedef uint32_t QueueIndexKey;
	std::map<QueueIndexKey, std::optional<uint32_t>> indices;

public:
	static const QueueIndexKey GRAPHICS = 0;
	static const QueueIndexKey PRESENT = 1;

	QueueFamilyIndices();

	uint32_t get(QueueIndexKey key);
	void set(QueueIndexKey key, uint32_t value);

	std::set<uint32_t> generateUniqueSet();

	bool isComplete();
	bool isShareingIndexBetweenGraphicsAndPresent();
};

struct PhysicalDeviceInfo {
	vk::PhysicalDevice device;
	QueueFamilyIndices queueFamilyIndices;
};


class RenderEngine {
	// Variables ==========
	// Basic Vulkan objects ----------
	vk::PhysicalDevice physicalDevice;
	vk::Device device;
	vk::Instance instance;
	QueueFamilyIndices queueFamilyIndices;
	struct Queues {
		vk::Queue graphicsQueue;
		vk::Queue presentQueue;
	} queues;

	// Layer / Extension ----------
	std::vector<const char*> layerNames;
	std::vector<const char*> instanceExtensionNames;
	std::vector<const char*> deviceExtensionNames;
	vk::DebugUtilsMessengerEXT debugMessenger;
	bool isValidationLayerEnabled = false;

	// Property / Information ----------
	RenderEngineInfo engineInfo;

	// Functions ==========
	// Creation / Setup ----------
	void createInstance(RenderEngineInitConfig initConfig);
	void createDevice(RenderEngineInitConfig initConfig);
	void setupExtensions(RenderEngineInitConfig initConfig);

	// Destruction ----------
	void destroyExtensions();

	// Misc ----------
	static vk::DebugUtilsMessengerCreateInfoEXT getDebugUtilsMessengerCI();
public:
	RenderEngine(RenderEngineInitConfig initConfig);
	~RenderEngine();

	static PhysicalDeviceInfo getSuitablePhysicalDeviceInfo(const RenderEngine* pEngine, std::vector<vk::PhysicalDevice> devices);

	void draw(RenderContent content, RenderTarget target);

	// Getter functions ----------
	vk::Instance getInstance() const;
	vk::PhysicalDevice getPhysicalDevice() const;
	vk::Device getDevice() const;
	QueueFamilyIndices getQueueFamilyIndices() const;
};

struct RenderEngineInitConfig {
	// app info ----------
	const char* applicationName;
	uint32_t applicationVersion;

	// Layers / Extensions ----------
	std::vector<const char*> layerNames;
	std::vector<const char*> instanceExtensionNames;
	std::vector<const char*> deviceExtensionNames;

	// callbacks ----------
	std::function<PhysicalDeviceInfo(const RenderEngine*, std::vector<vk::PhysicalDevice>)> suitablePhysicalDevicePicker;

	// Template providers ----------
	static RenderEngineInitConfig defaultDebugSetting() {
		RenderEngineInitConfig initConfig = defaultCommonSetting();

		initConfig.layerNames.push_back(VALIDATION_LAYER_NAME);

		initConfig.instanceExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		initConfig.instanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

		initConfig.deviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		return initConfig;
	}

private:
	static RenderEngineInitConfig defaultCommonSetting() {
		RenderEngineInitConfig initConfig;
		initConfig.applicationName = "Ikura Application";
		initConfig.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

		initConfig.suitablePhysicalDevicePicker = RenderEngine::getSuitablePhysicalDeviceInfo;

		return initConfig;
	}
};