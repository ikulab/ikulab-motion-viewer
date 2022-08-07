#include "./renderEngine.hpp"

#include <map>
#include <vector>

#include <GLFW/glfw3.h>

#include <easylogging++.h>


struct PhysicalDeviceEvaluation {
	int score;
	bool isQueueFamiliesCompleted;
	bool isAllExtensionsSupported;
	bool isSwapChainAdequate;

	bool isSuitable() const {
		return isAllExtensionsSupported
			&& isQueueFamiliesCompleted
			&& isSwapChainAdequate;
	}

	bool operator<(const PhysicalDeviceEvaluation& right) const {
		return this->score < right.score;
	}
};

// Forward declearation of helper functions ----------
QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR testSurface);
void EvaluateDeviceExtensionSupport(std::vector<const char*> extensionNames, vk::PhysicalDevice device, PhysicalDeviceEvaluation& eval);
void EvaluateSurfaceSupport(vk::SurfaceKHR testSurface, vk::PhysicalDevice device, PhysicalDeviceEvaluation& eval);
vk::SampleCountFlagBits getMaxMsaaSamples(vk::PhysicalDevice device);


void RenderEngine::createDevice(RenderEngineInitConfig initConfig) {
	// Extensions ----------
	deviceExtensionNames = initConfig.deviceExtensionNames;

	// Pick PhysicalDevice ==========
	auto devices = instance.enumeratePhysicalDevices();

	if (devices.empty()) {
		throw std::runtime_error(
			"Failed to any find PhysicalDevice with Vulkan support."
		);
	}

	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Picking up suitable PhysicalDevice...";
	auto physicalDevInfo = initConfig.suitablePhysicalDevicePicker(this, devices);
	physicalDevice = physicalDevInfo.device;
	queueFamilyIndices = physicalDevInfo.queueFamilyIndices;

	if (VLOG_IS_ON(VLOG_LV_3_PROCESS_TRACKING)) {
		VLOG(VLOG_LV_3_PROCESS_TRACKING)
			<< "Picked up suitable PhysicalDevice: "
			<< physicalDevice.getProperties().deviceName;
	}

	// Create LogicalDevice ==========
	vk::DeviceCreateInfo deviceCI{};

	// DeviceQueue Create ----------
	std::vector<vk::DeviceQueueCreateInfo> queueCI;
	{
		auto uniqueQ = queueFamilyIndices.generateUniqueSet();
		float priority = 1.0f;
		for (const auto queue : uniqueQ) {
			vk::DeviceQueueCreateInfo ci{};
			ci.queueFamilyIndex = queue;
			ci.queueCount = 1;
			ci.pQueuePriorities = &priority;

			queueCI.push_back(ci);
		}
	}
	deviceCI.pQueueCreateInfos = queueCI.data();
	deviceCI.queueCreateInfoCount = static_cast<uint32_t>(queueCI.size());

	// PhysicalDevice Feature ----------
	vk::PhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	deviceCI.pEnabledFeatures = &deviceFeatures;

	// Layers / Extensions ----------
	// NOTE: DeviceLayer is now deprecated, but for capabilities
	deviceCI.ppEnabledExtensionNames = deviceExtensionNames.data();
	deviceCI.enabledExtensionCount = deviceExtensionNames.size();
	deviceCI.ppEnabledLayerNames = layerNames.data();
	deviceCI.enabledLayerCount = layerNames.size();

	device = physicalDevice.createDevice(deviceCI);

	// Initialize Vulkan Hpp Default DIspatcher
	VULKAN_HPP_DEFAULT_DISPATCHER.init(device);

	// Hold queue in variables
	queues.graphicsQueue = device.getQueue(queueFamilyIndices.get(QueueFamilyIndices::GRAPHICS), 0);
	queues.presentQueue = device.getQueue(queueFamilyIndices.get(QueueFamilyIndices::PRESENT), 0);

	// set RenderEngineInfo
	engineInfo.limit.maxMsaaSamples = getMaxMsaaSamples(physicalDevice);
	engineInfo.queueFamily.isGraphicsAndPresentSameIndex = (
		queueFamilyIndices.get(QueueFamilyIndices::GRAPHICS) ==
		queueFamilyIndices.get(QueueFamilyIndices::PRESENT)
	);
}

/**
 * @brief Default PhysicalDevice picker function.
 * It investigates all available PhysicalDevice suitability and returns the best.
 *
 * @param pEngine pointer to RenderEngine
 * @param devices all available PhysicalDevices
 * @return most suitable PhysicalDevice
 */
PhysicalDeviceInfo RenderEngine::getSuitablePhysicalDeviceInfo(const RenderEngine* pEngine, std::vector<vk::PhysicalDevice> devices) {
	// create invisible Glfw Window to investigate Surface support
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* testWindow = glfwCreateWindow(10, 10, "", nullptr, nullptr);
	// use "Vk style" (not vk::) for compatibility to GLFW
	VkSurfaceKHR testSurface;
	glfwCreateWindowSurface(pEngine->instance, testWindow, nullptr, &testSurface);

	VLOG(VLOG_LV_4_PROCESS_TRACKING_SECONDARY)
		<< "Temporary VkSurface for PhysicalDevice suitability investigate has been created.";

	// if score(key) < 0, this device is not suitable.
	std::multimap<PhysicalDeviceEvaluation, PhysicalDeviceInfo> candidates;

	VLOG(VLOG_LV_6_ITEM_ENUMERATION) << "PhysicalDevices with vulkan support:";
	for (const auto& dev : devices) {
		PhysicalDeviceEvaluation eval{};

		if (VLOG_IS_ON(VLOG_LV_6_ITEM_ENUMERATION)) {
			auto props = dev.getProperties();
			VLOG(VLOG_LV_6_ITEM_ENUMERATION)
				<< "\tdeviceName: "
				<< props.deviceName
				<< ", driverVersion: "
				<< props.driverVersion;
		}

		QueueFamilyIndices indices = findQueueFamilies(dev, testSurface);
		eval.isQueueFamiliesCompleted = indices.isComplete();

		EvaluateDeviceExtensionSupport(pEngine->deviceExtensionNames, dev, eval);
		EvaluateSurfaceSupport(testSurface, dev, eval);

		if (!eval.isSuitable()) {
			eval.score = -1;
		}

		VLOG(VLOG_LV_6_ITEM_ENUMERATION) << "\t\tScore: " << eval.score;
		candidates.insert(std::make_pair(eval, PhysicalDeviceInfo{ dev, indices }));
	}

	// destroy test objects
	pEngine->instance.destroySurfaceKHR(testSurface);
	glfwDestroyWindow(testWindow);

	VLOG(VLOG_LV_4_PROCESS_TRACKING_SECONDARY)
		<< "Test VkSurface has been destroyed.";

	if (candidates.rbegin()->first.score < 0) {
		throw std::runtime_error("Cannot find suitable GPU.");
	}
	return candidates.rbegin()->second;
}

/**
 * @brief Populate QueueFamilyIndices for givin PhysicalDevice and return it.
 */
QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR testSurface) {
	QueueFamilyIndices result;

	uint32_t familyCount;
	auto families = device.getQueueFamilyProperties();

	uint32_t index = 0;
	for (const auto& prop : families) {
		// graphic family
		if (prop.queueFlags & vk::QueueFlagBits::eGraphics) {
			result.set(QueueFamilyIndices::GRAPHICS, index);
		}

		// present family
		vk::Bool32 presentSupport = VK_FALSE;
		presentSupport = device.getSurfaceSupportKHR(index, testSurface);
		if (presentSupport) {
			result.set(QueueFamilyIndices::PRESENT, index);
		}

		if (result.isComplete()) break;
		index++;
	}

	return result;
}

/**
 * @brief Evaluate DeviceExtension support.
 * If any of required extensions are NOT supported, makes eval.isAllExtensionSupported false.
 * The higher the number of DeviceExtensions, the higher eval.score.
 *
 * @param extensionNames DeviceExtension requirement
 * @return int
 */
void EvaluateDeviceExtensionSupport(std::vector<const char*> extensionNames, vk::PhysicalDevice device, PhysicalDeviceEvaluation& eval) {
	auto required = extensionNames;
	auto requiredEndIter = required.end();
	int totalNumberOfExtensions = 0;

	auto checkSupport = [&device, &required, &requiredEndIter, &totalNumberOfExtensions](vk::Optional<const std::string> layerName) {
		std::vector<vk::ExtensionProperties> exProps = device.enumerateDeviceExtensionProperties(layerName);
		totalNumberOfExtensions += exProps.size();

		if (layerName == nullptr) {
			VLOG(VLOG_LV_6_ITEM_ENUMERATION) << "\t\t\tGlobal:";
		}
		else {
			VLOG(VLOG_LV_6_ITEM_ENUMERATION)
				<< "\tExtensions associated with layer '"
				<< layerName
				<< "':";
		}

		if (exProps.empty()) {
			VLOG(VLOG_LV_6_ITEM_ENUMERATION) << "\t\t\t\t(None)";
		}

		for (const auto& exProp : exProps) {
			VLOG(VLOG_LV_6_ITEM_ENUMERATION)
				<< "\t\t\t\t"
				<< exProp.extensionName
				<< " version: "
				<< exProp.specVersion;

			requiredEndIter = std::remove_if(
				required.begin(),
				requiredEndIter,
				[&exProp](const char* name) {
					return std::strcmp(exProp.extensionName, name) == 0;
				}
			);
		}
	};

	VLOG(VLOG_LV_6_ITEM_ENUMERATION) << "\t\tDeviceExtensions:";

	checkSupport(nullptr);

	auto layerProps = device.enumerateDeviceLayerProperties();

	for (const auto& prop : layerProps) {
		checkSupport(std::string(prop.layerName));
	}

	eval.isAllExtensionsSupported = (requiredEndIter == required.begin());
	eval.score += totalNumberOfExtensions * 10;
}

/**
 * @brief Evaluates surface support.
 * If givin surface (i.e. PhysicalDevice) has NO available formats and presentModes,
 * eval.isSwapChainAdequate will be false.
 */
void EvaluateSurfaceSupport(vk::SurfaceKHR testSurface, vk::PhysicalDevice device, PhysicalDeviceEvaluation& eval) {
	auto formats = device.getSurfaceFormatsKHR(testSurface);
	auto presentModes = device.getSurfacePresentModesKHR(testSurface);

	eval.isSwapChainAdequate = (!formats.empty() && !presentModes.empty());
}

/**
 * @brief Returns max MSAA samples
 */
vk::SampleCountFlagBits getMaxMsaaSamples(vk::PhysicalDevice device) {
	auto props = device.getProperties();

	vk::SampleCountFlags counts = (
		props.limits.framebufferColorSampleCounts &
		props.limits.framebufferDepthSampleCounts
	);

	if (counts & vk::SampleCountFlagBits::e64) return vk::SampleCountFlagBits::e64;
	if (counts & vk::SampleCountFlagBits::e32) return vk::SampleCountFlagBits::e32;
	if (counts & vk::SampleCountFlagBits::e16) return vk::SampleCountFlagBits::e16;
	if (counts & vk::SampleCountFlagBits::e8) return vk::SampleCountFlagBits::e8;
	if (counts & vk::SampleCountFlagBits::e4) return vk::SampleCountFlagBits::e4;
	if (counts & vk::SampleCountFlagBits::e2) return vk::SampleCountFlagBits::e2;
	return vk::SampleCountFlagBits::e1;
}