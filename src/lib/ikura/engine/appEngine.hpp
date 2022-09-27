#pragma once

#include <chrono>
#include <memory>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../window/nativeWindow/glfwNativeWindow.hpp"
#include "../window/virtualWindow/imGuiVirtualWindow.hpp"
#include "../window/window.hpp"
#include "./renderEngine/renderEngine.hpp"

namespace ikura {
class AppEngine {
    std::shared_ptr<RenderEngine> renderEngine;
    std::vector<std::shared_ptr<Window>> windows;

    float fps = 60.0;
    std::chrono::_V2::system_clock::time_point startTime;
    std::chrono::_V2::system_clock::time_point currentTime;
    float secondsFromStart;

  public:
    AppEngine(std::shared_ptr<RenderEngine> renderEngine);

    void addWindow(std::shared_ptr<GlfwNativeWindow> glfwNativeWindow);
    void addWindow(std::shared_ptr<ImGuiVirtualWindow> imGuiVirtualWindow);

    void vSync();
    void setStartTime();
    float getSecondsFromStart() const;

    int shouldTerminated();
    void drawAllWindows();
    void destroyClosedWindow();
};
} // namespace ikura