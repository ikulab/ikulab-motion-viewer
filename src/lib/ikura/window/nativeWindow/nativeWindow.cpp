#include "./nativeWindow.hpp"

#include <vulkan/vulkan.hpp>

namespace ikura {
	NativeWindow::~NativeWindow() {
		device.destroySwapchainKHR(swapChain);
		instance.destroySurfaceKHR(surface);	
	}
}