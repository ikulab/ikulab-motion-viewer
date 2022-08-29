#include "./app.hpp"

#include <iostream>

App::App() {}

void App::init() {
	// Initialize Ikura
	initIkura();

	// Initialize RenderEngine
	RenderEngineInitConfig renderConfig = RenderEngineInitConfig::defaultDebugSetting();
	renderConfig.applicationName = "IkulabMotionViewer";
	renderConfig.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	renderEngine = std::make_shared<RenderEngine>(renderConfig);
	renderEngine->createInstance();
	renderEngine->setupExtensions();

	// Create GLFW Window 
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* glfwWindow = glfwCreateWindow(400, 300, "Ikura Window", nullptr, nullptr);

	// Create Surface
	VkSurfaceKHR vkSurface;
	if ((glfwCreateWindowSurface(
		renderEngine->getInstance(), glfwWindow, nullptr, &vkSurface
		)) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create VkSurfaceKHR from glfwCreateWindowSurface().");
	}
	vk::SurfaceKHR surface = vk::SurfaceKHR(vkSurface);

	// Create RenderEngine's Device
	renderEngine->setSampleSurface(surface);
	renderEngine->createDevice();

	// Initialize AppEngine
	appEngine = std::make_unique<AppEngine>(renderEngine);

	// Add Window
	appEngine->addWindow(std::make_shared<ikura::GlfwNativeWindow>(
		renderEngine, glfwWindow, surface, "main"
	));

	std::cout << "Hello Ikura!!" << std::endl;
}