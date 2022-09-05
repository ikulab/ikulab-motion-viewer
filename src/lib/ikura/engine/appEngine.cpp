#include "./appEngine.hpp"

#include <vulkan/vulkan.hpp>

AppEngine::AppEngine(std::shared_ptr<RenderEngine> renderEngine) {
    this->renderEngine = renderEngine;
}

void AppEngine::addWindow(
    std::shared_ptr<ikura::GlfwNativeWindow> glfwNativeWindow) {
    glfwNativeWindow->addDefaultRenderTarget();
    windows.push_back(glfwNativeWindow);
}

void AppEngine::addWindow(
    std::shared_ptr<ikura::ImGuiVirtualWindow> imGuiVirtualWindow) {
    windows.push_back(imGuiVirtualWindow);
}