#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../engine/renderEngine/renderEngine.hpp"
#include "../window.hpp"
#include "../windowInputContext.hpp"

namespace ikura {
class NativeWindow : public Window,
                     public std::enable_shared_from_this<NativeWindow> {
  protected:
    WindowInputContext inputCtx;
    vk::SurfaceKHR surface;
    vk::SwapchainKHR swapChain;
    vk::Format swapChainFormat;
    vk::Extent2D swapChainExtent;

    std::vector<vk::Image> swapChainImages;
    uint32_t currentFrame = 0;
    bool swapChainResized = false;

    NativeWindow() {}

  public:
    virtual ~NativeWindow();

    const vk::SwapchainKHR getSwapChain() const;
    const vk::Format getSwapChainFormat() const;
    const vk::Extent2D getSwapChainExtent() const;
    const std::vector<vk::Image> getSwapChainImages() const;

    virtual int windowShouldClose();
    void draw() override;
};
} // namespace ikura