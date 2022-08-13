#include "./nativeWindow.hpp"

#include <easylogging++.h>
#include <vulkan/vulkan.hpp>

#include "../../ikura.hpp"

namespace ikura {
	NativeWindow::~NativeWindow() {
		VLOG(VLOG_LV_3_PROCESS_TRACKING)
			<< "Destroying SwapChain for '"
			<< name
			<< "'...";
		device.destroySwapchainKHR(swapChain);
		VLOG(VLOG_LV_3_PROCESS_TRACKING)
			<< "SwapChain for '"
			<< name
			<< "' has been destroyed.";

		VLOG(VLOG_LV_3_PROCESS_TRACKING)
			<< "Destroying Surface for '"
			<< name
			<< "'...";
		instance.destroySurfaceKHR(surface);
		VLOG(VLOG_LV_3_PROCESS_TRACKING)
			<< "Surface for '"
			<< name
			<< "' has been destroyed.";
	}
}