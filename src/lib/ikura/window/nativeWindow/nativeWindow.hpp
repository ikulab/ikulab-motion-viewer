#pragma once

#include <vulkan/vulkan.hpp>

#include "../window.hpp"
#include "../windowInputContext.hpp"
#include "../../engine/renderEngine/renderEngine.hpp"

namespace ikura {
	class NativeWindow : public Window {
	protected:
		struct SurfaceSupportInfo {
			vk::SurfaceCapabilitiesKHR capabilities;
			std::vector<vk::SurfaceFormatKHR> formats;
			std::vector<vk::PresentModeKHR> presentModes;
		} surfaceSupport;

		vk::Instance instance;
		vk::PhysicalDevice physicalDevice;
		vk::Device device;
		QueueFamilyIndices queueFamilyIndices;

		WindowInputContext inputCtx;
		vk::SurfaceKHR surface;
		vk::SwapchainKHR swapChain;

		NativeWindow() {}
	public:
		virtual ~NativeWindow();
	};
}