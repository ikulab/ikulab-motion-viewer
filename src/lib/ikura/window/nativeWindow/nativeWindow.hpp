#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../engine/renderEngine/renderEngine.hpp"
#include "../window.hpp"
#include "../windowInputContext.hpp"

namespace ikura {
class NativeWindow : public Window {
  protected:
    std::shared_ptr<RenderEngine> renderEngine;

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
};
} // namespace ikura