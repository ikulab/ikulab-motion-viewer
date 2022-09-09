#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../engine/renderEngine/renderEngine.hpp"
#include "../window.hpp"
#include "../windowInputContext.hpp"

namespace ikura {
class NativeWindow : public Window, public std::enable_shared_from_this<NativeWindow> {
  protected:
    WindowInputContext inputCtx;
    vk::SurfaceKHR surface;
    vk::SwapchainKHR swapChain;
    vk::Format swapChainFormat;
    vk::Extent2D swapChainExtent;

    std::vector<vk::Image> swapChainImages;

    NativeWindow() {}

  public:
    virtual ~NativeWindow();
    void addDefaultRenderTarget() override;
    void addDefaultRenderContent() override;

    const vk::SwapchainKHR getSwapChain() const;
    const vk::Format getSwapChainFormat() const;
    const vk::Extent2D getSwapChainExtent() const;
    const std::vector<vk::Image> getSwapChainImages() const;
};
} // namespace ikura