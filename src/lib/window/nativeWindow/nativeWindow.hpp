#pragma once

#include <vulkan/vulkan.hpp>

#include "../window.hpp"
#include "../windowInputContext.hpp"

class NativeWindow : public Window {
protected:
	WindowInputContext inputCtx;
	VkSurfaceKHR surface;
	NativeWindow() {}
};