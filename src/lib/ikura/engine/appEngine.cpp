#include "./appEngine.hpp"

#include <vulkan/vulkan.hpp>


void AppEngine::addWindow(std::shared_ptr<ikura::GlfwNativeWindow> glfwNativeWindow) {
	windows.push_back(glfwNativeWindow);
}

void AppEngine::addWindow(std::shared_ptr<ikura::ImGuiVirtualWindow> imGuiVirtualWindow) {
	windows.push_back(imGuiVirtualWindow);
}