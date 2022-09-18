#include "./app.hpp"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

App::App() {}

void App::init() {
    // Initialize RenderEngine
    ikura::RenderEngineInitConfig renderConfig =
        ikura::RenderEngineInitConfig::defaultDebugSetting();
    renderConfig.applicationName = "IkulabMotionViewer";
    renderConfig.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

    renderEngine = std::make_shared<ikura::RenderEngine>(renderConfig);
    renderEngine->createInstance();
    renderEngine->setupExtensions();

    // Create GLFW Window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *glfwWindow =
        glfwCreateWindow(400, 300, "Ikura Window", nullptr, nullptr);

    // Create Surface
    VkSurfaceKHR vkSurface;
    if ((glfwCreateWindowSurface(renderEngine->getInstance(), glfwWindow,
                                 nullptr, &vkSurface)) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create VkSurfaceKHR from glfwCreateWindowSurface().");
    }
    vk::SurfaceKHR surface = vk::SurfaceKHR(vkSurface);

    // Create RenderEngine's Device
    renderEngine->setSampleSurface(surface);
    renderEngine->createDevice();

    // Initialize AppEngine
    appEngine = std::make_unique<ikura::AppEngine>(renderEngine);

    // Add Window
    auto mainWindow = std::make_shared<ikura::GlfwNativeWindow>(
        renderEngine, glfwWindow, surface, "main");
    appEngine->addWindow(mainWindow);

    std::cout << "Hello Ikura!!" << std::endl;

    mainWindow->getRenderContent()->uploadVertexBuffer();
    mainWindow->getRenderContent()->uploadIndexBuffer();

    while (!appEngine->shouldTerminated()) {
        appEngine->drawAllWindows();
    }

    renderEngine->waitForDeviceIdle();
}