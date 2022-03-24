#pragma once

#include <GLFW/glfw3.h>

class FirstTriangleApp {
	GLFWwindow* window;
	uint32_t WINDOW_WIDTH = 800;
	uint32_t WINDOW_HEIGHT = 600;

	void initWindow();
	void initVulkan();
	void createInstance();
	void mainLoop();
	void cleanUp();

public:
	void run();
};