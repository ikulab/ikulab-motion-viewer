#include "./appEngine.hpp"

#include <iostream>

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>

AppEngine::AppEngine(std::shared_ptr<RenderEngine> renderEngine) {
    this->renderEngine = renderEngine;
}

void AppEngine::addWindow(
    std::shared_ptr<ikura::GlfwNativeWindow> glfwNativeWindow) {
    glfwNativeWindow->createDefaultDescriptorSetLayout();
    glfwNativeWindow->addDefaultRenderTarget();
    glfwNativeWindow->addDefaultRenderContent();
    windows.push_back(glfwNativeWindow);
}

void AppEngine::addWindow(
    std::shared_ptr<ikura::ImGuiVirtualWindow> imGuiVirtualWindow) {
    windows.push_back(imGuiVirtualWindow);
}

int AppEngine::shouldTerminated() {
    return std::any_of(windows.begin(), windows.end(),
                       [&](const std::shared_ptr<ikura::Window> window) {
                           auto pw = dynamic_cast<ikura::NativeWindow *>(
                               window.get());
                           if (pw != nullptr) {
                               return pw->windowShouldClose();
                           }
                           return 0;
                       });
}

void AppEngine::drawAllWindows() {
    // Poll GLFW Events (execute once per loop)
    for (const auto &window : windows) {
        auto pw = dynamic_cast<ikura::GlfwNativeWindow *>(window.get());
        if (pw != nullptr) {
            glfwPollEvents();
            break;
        }
    }

    for (auto &window : windows) {
        window->draw();
    }
}