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
		renderEngine->getDevice().destroySwapchainKHR(swapChain);
		VLOG(VLOG_LV_3_PROCESS_TRACKING)
			<< "SwapChain for '"
			<< name
			<< "' has been destroyed.";

		VLOG(VLOG_LV_3_PROCESS_TRACKING)
			<< "Destroying Surface for '"
			<< name
			<< "'...";
		renderEngine->getInstance().destroySurfaceKHR(surface);
		VLOG(VLOG_LV_3_PROCESS_TRACKING)
			<< "Surface for '"
			<< name
			<< "' has been destroyed.";
	}

	void NativeWindow::addDefaultRenderTarget() {
		renderTargets.push_back(
			std::make_unique<RenderTarget>(swapChainFormat, renderEngine)
		);
	}
}