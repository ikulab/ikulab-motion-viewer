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
    auto target =
        std::make_unique<RenderTarget>(shared_from_this(), renderEngine);
    target->setDefaultResources();
    renderTargets.push_back(std::move(target));
}

void NativeWindow::addDefaultRenderContent() {
    auto content = std::make_unique<RenderContent>(shared_from_this(), renderEngine);
    content->setDefaultResources();
    renderContents.push_back(std::move(content));
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
} // namespace ikura