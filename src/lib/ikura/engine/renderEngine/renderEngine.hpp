#pragma once

#include <vector>
#include <set>
#include <string>
#include <functional>

#include <vulkan/vulkan.hpp>

#include <vk_mem_alloc.h>

#include "../../renderComponent/renderContent.hpp"
#include "../../renderComponent/renderTarget.hpp"
#include "../../ikura.hpp"

// Forward Declearration ----------
class RenderEngine;

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

struct RenderEngineInitConfig {
	// app info ----------
	const char* applicationName;
	uint32_t applicationVersion;

	// Layers / Extensions ----------
	std::vector<const char*> layerNames;
	std::vector<const char*> instanceExtensionNames;
	std::vector<const char*> deviceExtensionNames;

	// callbacks ----------
	std::function<vk::PhysicalDevice(const RenderEngine*, std::vector<vk::PhysicalDevice>)> suitablePhysicalDevicePicker;

	// Template providers ----------
	static RenderEngineInitConfig defaultDebugSetting();

private:
	static RenderEngineInitConfig defaultCommonSetting();
};

class RenderEngine {
	// Variables ==========
	// Basic Vulkan objects ----------
	vk::PhysicalDevice physicalDevice;
	vk::Device device;
	vk::Instance instance;
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
	RenderEngineInitConfig initConfig;
	RenderEngineInfo engineInfo;

	// Misc ----------
	vk::SurfaceKHR sampleSurface;	// for PhysicalDevice suitability evaluation
	std::shared_ptr<VmaAllocator> vmaAllocator;

	// Functions ==========
	// Destruction ----------
	void destroyExtensions();

	// Misc ----------
	static vk::DebugUtilsMessengerCreateInfoEXT getDebugUtilsMessengerCI();
public:
	// Functions ==========
	// Constructor / Desctuctor ----------
	RenderEngine(RenderEngineInitConfig initConfig);
	~RenderEngine();

	// Creation / Setup ----------
	void createInstance();
	void createDevice();
	void setupExtensions();

	// Getter ----------
	vk::Instance getInstance() const;
	vk::PhysicalDevice getPhysicalDevice() const;
	vk::Device getDevice() const;

	// Setter ----------
	void setSampleSurface(vk::SurfaceKHR surface);

	// Interface ----------
	void draw(ikura::RenderContent content, ikura::RenderTarget target);

	// Misc ----------
	static vk::PhysicalDevice getSuitablePhysicalDeviceInfo(const RenderEngine* pEngine, std::vector<vk::PhysicalDevice> devices);
};


// Helper function ----------
QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR sampleSurface);