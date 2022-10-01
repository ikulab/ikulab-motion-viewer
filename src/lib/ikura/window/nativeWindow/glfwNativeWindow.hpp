#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>

#include "./nativeWindow.hpp"

namespace ikura {
class GlfwNativeWindow : public NativeWindow {
  protected:
    GLFWwindow *window;

    bool frameBufferResized = false;
    vk::SwapchainCreateInfoKHR swapChainCICache;

    void createSwapChain();
    static void framebufferResizeCallback(GLFWwindow *window, int width,
                                          int height);
    void recordCommandBuffer(uint32_t imageIndex);
    void recreateSwapChain() override;

    void destroyGlfwWindow();

  public:
    GlfwNativeWindow(const std::shared_ptr<RenderEngine> renderEngine,
                     GLFWwindow *window, vk::SurfaceKHR surface,
                     std::string name);
    ~GlfwNativeWindow();

    void destroyResources() override;
    void draw() override;
    bool closed() override;

    GLFWwindow *getGLFWWindow() const;
};
} // namespace ikura