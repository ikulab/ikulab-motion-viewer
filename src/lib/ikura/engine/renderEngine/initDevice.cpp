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
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR testSurface);
void EvaluateDeviceExtensionSupport(std::vector<const char*> extensionNames, VkPhysicalDevice device, PhysicalDeviceEvaluation& eval);
void EvaluateSurfaceSupport(VkSurfaceKHR testSurface, VkPhysicalDevice device, PhysicalDeviceEvaluation& eval);


void RenderEngine::createDevice(RenderEngineInitConfig initConfig) {
	// Extensions ----------
	deviceExtensionNames = initConfig.deviceExtensionNames;

	// Pick PhysicalDevice ==========
	uint32_t devCount;
	vkEnumeratePhysicalDevices(instance, &devCount, nullptr);
	std::vector<VkPhysicalDevice> devices(devCount);
	vkEnumeratePhysicalDevices(instance, &devCount, devices.data());

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
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(physicalDevice, &props);
		VLOG(VLOG_LV_3_PROCESS_TRACKING)
			<< "Picked up suitable PhysicalDevice: "
			<< props.deviceName;
	}

	// Create LogicalDevice ==========
	VkDeviceCreateInfo deviceCI{};
	deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	// DeviceQueue Create ----------
	std::vector<VkDeviceQueueCreateInfo> queueCI;
	{
		auto uniqueQ = queueFamilyIndices.generateUniqueSet();
		float priority = 1.0f;
		for (const auto queue : uniqueQ) {
			VkDeviceQueueCreateInfo ci{};
			ci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			ci.queueFamilyIndex = queue;
			ci.queueCount = 1;
			ci.pQueuePriorities = &priority;

			queueCI.push_back(ci);
		}
	}
	deviceCI.pQueueCreateInfos = queueCI.data();
	deviceCI.queueCreateInfoCount = static_cast<uint32_t>(queueCI.size());

	// PhysicalDevice Feature ----------
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	deviceCI.pEnabledFeatures = &deviceFeatures;

	// Layers / Extensions ----------
	// NOTE: DeviceLayer is now deprecated, but for capabilities
	deviceCI.ppEnabledExtensionNames = deviceExtensionNames.data();
	deviceCI.enabledExtensionCount = deviceExtensionNames.size();
	deviceCI.ppEnabledLayerNames = layerNames.data();
	deviceCI.enabledLayerCount = layerNames.size();

	CheckError(
		vkCreateDevice(physicalDevice, &deviceCI, nullptr, &device),
		"Failed to create Logical Device."
	);

	// Hold queue in variables
	vkGetDeviceQueue(device, queueFamilyIndices.get(QueueFamilyIndices::GRAPHICS), 0, &queues.graphicsQueue);
	vkGetDeviceQueue(device, queueFamilyIndices.get(QueueFamilyIndices::PRESENT), 0, &queues.presentQueue);
}

/**
 * @brief Default PhysicalDevice picker function.
 * It investigates all available PhysicalDevice suitability and returns the best.
 * This function
 *
 * @param pEngine pointer to RenderEngine
 * @param devices all available PhysicalDevices
 * @return most suitable PhysicalDevice
 */
PhysicalDeviceInfo RenderEngine::getSuitablePhysicalDeviceInfo(const RenderEngine* pEngine, std::vector<VkPhysicalDevice> devices) {
	// create invisible Glfw Window to investigate Surface support
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* testWindow = glfwCreateWindow(10, 10, "", nullptr, nullptr);
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
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(dev, &props);
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
	vkDestroySurfaceKHR(pEngine->instance, testSurface, nullptr);
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
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR testSurface) {
	QueueFamilyIndices result;

	uint32_t familyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
	std::vector<VkQueueFamilyProperties> families(familyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

	uint32_t index = 0;
	for (const auto& prop : families) {
		// graphic family
		if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			result.set(QueueFamilyIndices::GRAPHICS, index);
		}

		// present family
		VkBool32 presentSupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, index, testSurface, &presentSupport);
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
void EvaluateDeviceExtensionSupport(std::vector<const char*> extensionNames, VkPhysicalDevice device, PhysicalDeviceEvaluation& eval) {
	auto required = extensionNames;
	auto requiredEndIter = required.end();
	int totalNumberOfExtensions = 0;

	auto checkSupport = [&device, &required, &requiredEndIter, &totalNumberOfExtensions](const char* layerName) {
		uint32_t exCount;
		vkEnumerateDeviceExtensionProperties(device, layerName, &exCount, nullptr);
		std::vector<VkExtensionProperties> exProps(exCount);
		vkEnumerateDeviceExtensionProperties(device, layerName, &exCount, exProps.data());

		totalNumberOfExtensions += exCount;

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

	uint32_t layerCount;
	vkEnumerateDeviceLayerProperties(device, &layerCount, nullptr);
	std::vector<VkLayerProperties> layerProps;
	vkEnumerateDeviceLayerProperties(device, &layerCount, layerProps.data());

	for (const auto& prop : layerProps) {
		checkSupport(prop.layerName);
	}

	eval.isAllExtensionsSupported = (requiredEndIter == required.begin());
	eval.score += totalNumberOfExtensions * 10;
}

/**
 * @brief Evaluates surface support.
 * If givin surface (i.e. PhysicalDevice) has NO available formats and presentModes,
 * eval.isSwapChainAdequate will be false.
 */
void EvaluateSurfaceSupport(VkSurfaceKHR testSurface, VkPhysicalDevice device, PhysicalDeviceEvaluation& eval) {
	// VkSurfaceCapabilitiesKHR capabilities;
	// vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, testSurface, &capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, testSurface, &formatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, testSurface, &formatCount, formats.data());

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, testSurface, &presentModeCount, nullptr);
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, testSurface, &presentModeCount, presentModes.data());

	eval.isSwapChainAdequate = (!formats.empty() && !presentModes.empty());
}