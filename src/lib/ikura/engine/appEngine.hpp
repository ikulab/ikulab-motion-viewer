#pragma once

#include <vector>
#include <memory>

#include "../window/window.hpp"
#include "../window/nativeWindow/glfwNativeWindow.hpp"
#include "../window/virtualWindow/imGuiVirtualWindow.hpp"

class AppEngine {
	std::vector<std::shared_ptr<Window>> windows;

public:
	void addWindow(std::shared_ptr<GlfwNativeWindow> glfwNativeWindow);
	void addWindow(std::shared_ptr<ImGuiVirtualWindow> glfwNativeWindow);
};