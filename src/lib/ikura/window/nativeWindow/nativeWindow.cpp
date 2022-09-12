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
    renderTarget =
        std::make_unique<RenderTarget>(shared_from_this(), renderEngine);
    renderTarget->setDefaultResources();
}

void NativeWindow::addDefaultRenderContent() {
    renderContent =
        std::make_unique<RenderContent>(shared_from_this(), renderEngine);
    renderContent->setDefaultResources();
}

const vk::SwapchainKHR NativeWindow::getSwapChain() const { return swapChain; }

const vk::Format NativeWindow::getSwapChainFormat() const {
    return swapChainFormat;
}

const vk::Extent2D NativeWindow::getSwapChainExtent() const {
    return swapChainExtent;
}

const std::vector<vk::Image> NativeWindow::getSwapChainImages() const {
    return swapChainImages;
}

int NativeWindow::windowShouldClose() { return 0; }

void NativeWindow::draw() {}

} // namespace ikura