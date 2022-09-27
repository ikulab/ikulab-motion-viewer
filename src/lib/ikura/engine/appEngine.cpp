#include "./appEngine.hpp"

#include <iostream>
#include <thread>

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

void AppEngine::vSync() {
    auto rightNow = std::chrono::high_resolution_clock::now();

    // time taken to prev drawing process
    // currentTime must be updated previous frame
    auto delta = std::chrono::duration<float, std::chrono::nanoseconds::period>(
        rightNow - currentTime);

    auto waitTime =
        std::chrono::duration<float, std::chrono::nanoseconds::period>(
            std::chrono::nanoseconds(
                uint32_t(1000.0 * 1000.0 * 1000.0 * 1.0 / fps)) -
            delta);

    std::this_thread::sleep_for(waitTime);

    // update currentTime for next vSync
    currentTime = std::chrono::high_resolution_clock::now();
    secondsFromStart =
        std::chrono::duration<float, std::chrono::seconds::period>(currentTime -
                                                                   startTime)
            .count();
}

void AppEngine::setStartTime() {
    startTime = std::chrono::high_resolution_clock::now();
}

float AppEngine::getSecondsFromStart() const { return secondsFromStart; }

int AppEngine::shouldTerminated() {
    return std::all_of(windows.begin(), windows.end(),
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

void AppEngine::destroyClosedWindow() {
    
}
} // namespace ikura