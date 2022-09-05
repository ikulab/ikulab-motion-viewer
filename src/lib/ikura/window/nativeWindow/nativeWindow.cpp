#include "./nativeWindow.hpp"

#include <easylogging++.h>
#include <vulkan/vulkan.hpp>

namespace ikura {
NativeWindow::~NativeWindow() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Destroying SwapChain for '" << name << "'...";
    renderEngine->getDevice().destroySwapchainKHR(swapChain);
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "SwapChain for '" << name << "' has been destroyed.";

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Destroying Surface for '" << name << "'...";
    renderEngine->getInstance().destroySurfaceKHR(surface);
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Surface for '" << name << "' has been destroyed.";
}

void NativeWindow::addDefaultRenderTarget() {
    auto target = std::make_unique<RenderTarget>(swapChainFormat, swapChainExtent, renderEngine);
    target->setDefaultResources();
    renderTargets.push_back(std::move(target));
}
} // namespace ikura