#pragma once

#include <vulkan/vulkan.hpp>

#include "./window.hpp"

class NativeWindow : public Window {
	VkSurfaceKHR surface;
protected:
	NativeWindow() {}
};