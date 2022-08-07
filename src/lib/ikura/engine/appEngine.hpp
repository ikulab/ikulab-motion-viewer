#pragma once

#include <vector>
#include <memory>

#include "../window/window.hpp"
#include "./renderEngine/renderEngine.hpp"
#include "../window/nativeWindow/glfwNativeWindow.hpp"
#include "../window/virtualWindow/imGuiVirtualWindow.hpp"

class AppEngine {
	std::vector<std::shared_ptr<ikura::Window>> windows;

public:
	void addWindow(std::shared_ptr<ikura::GlfwNativeWindow> glfwNativeWindow);
	void addWindow(std::shared_ptr<ikura::ImGuiVirtualWindow> imGuiVirtualWindow);
};