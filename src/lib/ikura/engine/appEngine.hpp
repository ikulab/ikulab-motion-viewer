#pragma once

#include <vector>
#include <memory>

#include <GLFW/glfw3.h>

#include "../window/window.hpp"
#include "./renderEngine/renderEngine.hpp"
#include "../window/nativeWindow/glfwNativeWindow.hpp"
#include "../window/virtualWindow/imGuiVirtualWindow.hpp"

class AppEngine {
	std::shared_ptr<RenderEngine> renderEngine;
	std::vector<std::shared_ptr<ikura::Window>> windows;

public:
	AppEngine(std::shared_ptr<RenderEngine> renderEngine);

	void addWindow(std::shared_ptr<ikura::GlfwNativeWindow> glfwNativeWindow);
	void addWindow(std::shared_ptr<ikura::ImGuiVirtualWindow> imGuiVirtualWindow);
};