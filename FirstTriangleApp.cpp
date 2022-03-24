#include "FirstTriangleApp.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

void FirstTriangleApp::createInstance() {
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
}

void FirstTriangleApp::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "First Triangle App", nullptr, nullptr);
}

void FirstTriangleApp::initVulkan() {
	createInstance();
}

void FirstTriangleApp::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void FirstTriangleApp::cleanUp() {
	glfwDestroyWindow(window);

	glfwTerminate();
}

void FirstTriangleApp::run() {
	initWindow();
	initVulkan();
	mainLoop();
	cleanUp();
}