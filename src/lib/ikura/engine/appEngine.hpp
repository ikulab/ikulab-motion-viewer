#pragma once

#include <memory>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../window/nativeWindow/glfwNativeWindow.hpp"
#include "../window/virtualWindow/imGuiVirtualWindow.hpp"
#include "../window/window.hpp"
#include "./renderEngine/renderEngine.hpp"

class AppEngine {
    std::shared_ptr<RenderEngine> renderEngine;
    std::vector<std::shared_ptr<ikura::Window>> windows;

  public:
    AppEngine(std::shared_ptr<RenderEngine> renderEngine);

    void addWindow(std::shared_ptr<ikura::GlfwNativeWindow> glfwNativeWindow);
    void
    addWindow(std::shared_ptr<ikura::ImGuiVirtualWindow> imGuiVirtualWindow);

    int shouldTerminated();
    void drawAllWindows();
};