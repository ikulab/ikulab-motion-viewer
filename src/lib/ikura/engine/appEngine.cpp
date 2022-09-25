#include "./appEngine.hpp"

#include <iostream>

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

#include "../shape/shapes.hpp"

namespace ikura {
AppEngine::AppEngine(std::shared_ptr<RenderEngine> renderEngine) {
    this->renderEngine = renderEngine;
}

void AppEngine::addWindow(std::shared_ptr<GlfwNativeWindow> glfwNativeWindow) {
    windows.push_back(glfwNativeWindow);
}

void AppEngine::addWindow(
    std::shared_ptr<ImGuiVirtualWindow> imGuiVirtualWindow) {
    windows.push_back(imGuiVirtualWindow);
}

int AppEngine::shouldTerminated() {
    return std::any_of(windows.begin(), windows.end(),
                       [&](const std::shared_ptr<Window> window) {
                           auto pw = dynamic_cast<NativeWindow *>(window.get());
                           if (pw != nullptr) {
                               return pw->windowShouldClose();
                           }
                           return 0;
                       });
}

void AppEngine::drawAllWindows() {
    // Poll all GLFW Window Events (execute once per loop)
    for (const auto &window : windows) {
        auto pw = dynamic_cast<GlfwNativeWindow *>(window.get());
        if (pw != nullptr) {
            glfwPollEvents();
            break;
        }
    }

    for (auto &window : windows) {
        window->draw();
    }
}
} // namespace ikura