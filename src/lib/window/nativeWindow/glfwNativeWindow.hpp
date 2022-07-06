#pragma once

#include <GLFW/glfw3.h>

#include "./nativeWindow.hpp"

class GlfwNativeWindow : public NativeWindow {
	GLFWwindow *window;

public:
	~GlfwNativeWindow() {
		glfwDestroyWindow(window);
	}
};