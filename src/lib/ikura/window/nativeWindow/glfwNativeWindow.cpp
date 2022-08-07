#include "./glfwNativeWindow.hpp"


// Forward declearation of helper functions ----------
vk::SurfaceFormatKHR chooseSwapChainFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
vk::PresentModeKHR chooseSwapChainPresentMode(const std::vector<vk::PresentModeKHR>& presentModes);
vk::Extent2D chooseSwapChainExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

namespace ikura {
	void GlfwNativeWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto nativeWindow = reinterpret_cast<GlfwNativeWindow*>(glfwGetWindowUserPointer(window));
		nativeWindow->frameBufferResized = true;
	}

	GlfwNativeWindow::GlfwNativeWindow(GLFWwindow* window, const RenderEngine& renderEngine) {
		instance = renderEngine.getInstance();
		physicalDevice = renderEngine.getPhysicalDevice();
		device = renderEngine.getDevice();
		queueFamilyIndices = renderEngine.getQueueFamilyIndices();

		// Set GLFW Basic Properties
		this->window = window;
		glfwGetFramebufferSize(this->window, &width, &height);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

		// Create Surface
		VkSurfaceKHR vkSurface;
		if ((glfwCreateWindowSurface(instance, this->window, nullptr, &vkSurface)) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create VkSurfaceKHR from glfwCreateWindowSurface().");
		}
		surface = vk::SurfaceKHR(vkSurface);

		// Fill SurfaceSupportInfo
		surfaceSupport.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
		surfaceSupport.formats = physicalDevice.getSurfaceFormatsKHR(surface);
		surfaceSupport.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);

		createSwapChain();
	}

	GlfwNativeWindow::~GlfwNativeWindow() {
		glfwDestroyWindow(window);
	}

	void GlfwNativeWindow::createSwapChain() {
		vk::SurfaceFormatKHR format = chooseSwapChainFormat(surfaceSupport.formats);
		vk::PresentModeKHR presentMode = chooseSwapChainPresentMode(surfaceSupport.presentModes);
		vk::Extent2D extent = chooseSwapChainExtent(surfaceSupport.capabilities, window);

		uint32_t imageCount = surfaceSupport.capabilities.minImageCount + 1;
		if (surfaceSupport.capabilities.maxImageCount > 0 &&
			imageCount > surfaceSupport.capabilities.maxImageCount) {

			imageCount = surfaceSupport.capabilities.maxImageCount;
		}

		vk::SwapchainCreateInfoKHR swapChainCI;
		swapChainCI.surface = surface;
		swapChainCI.minImageCount = imageCount;

		swapChainCI.imageFormat = format.format;
		swapChainCI.imageColorSpace = format.colorSpace;
		swapChainCI.imageExtent = extent;
		swapChainCI.imageArrayLayers = 1;
		swapChainCI.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

		swapChainCI.preTransform = surfaceSupport.capabilities.currentTransform;
		swapChainCI.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		swapChainCI.presentMode = presentMode;
		swapChainCI.clipped = VK_TRUE;

		uint32_t indices[] = {
			queueFamilyIndices.get(QueueFamilyIndices::GRAPHICS),
			queueFamilyIndices.get(QueueFamilyIndices::PRESENT)
		};

		if (queueFamilyIndices.isShareingIndexBetweenGraphicsAndPresent()) {
			swapChainCI.imageSharingMode = vk::SharingMode::eExclusive;
			swapChainCI.queueFamilyIndexCount = 0;
		}
		else {
			swapChainCI.imageSharingMode = vk::SharingMode::eConcurrent;
			swapChainCI.queueFamilyIndexCount = 2;
			swapChainCI.pQueueFamilyIndices = indices;
		}

		swapChain = device.createSwapchainKHR(swapChainCI);
	}
}

vk::SurfaceFormatKHR chooseSwapChainFormat(const std::vector<vk::SurfaceFormatKHR>& formats) {
	for (const auto& format : formats) {
		if (format.format == vk::Format::eR8G8B8A8Srgb &&
			format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {

			return format;
		}
	}

	return formats[0];
}

vk::PresentModeKHR chooseSwapChainPresentMode(const std::vector<vk::PresentModeKHR>& presentModes) {
	for (const auto& presentMode : presentModes) {
		// if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
		if (presentMode == vk::PresentModeKHR::eMailbox) {
			return presentMode;
		}
	}

	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D chooseSwapChainExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		vk::Extent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(
			actualExtent.width,
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(
			actualExtent.height,
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height);
		return actualExtent;
	}
}