#include "./app.hpp"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

void App::initIkura() {
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

    // Create Device in RenderEngine (requires sample Surface)
    renderEngine->setSampleSurface(surface);
    renderEngine->createDevice();

    // Initialize AppEngine
    appEngine = std::make_unique<ikura::AppEngine>(renderEngine);

    // Setup ikura Window
    mainWindow = std::make_shared<ikura::GlfwNativeWindow>(
        renderEngine, glfwWindow, surface, "main");
    ikura::BasicRenderComponentProvider basicRenderComponentProvider(
        renderEngine);
    renderTarget =
        basicRenderComponentProvider.createBasicRenderTarget(mainWindow);
    renderContent =
        basicRenderComponentProvider.createBasicRenderContent(mainWindow);

    mainWindow->setRenderTarget(renderTarget);
    mainWindow->setRenderContent(renderContent);

    // Add Window
    appEngine->addWindow(mainWindow);
}

void App::setShapes() {
    auto cube =
        ikura::shapes::SeparatedColorCube(1.0, 1.0, 1.0, {0.0, 0.0, 0.0},
                                          {{{0.0, 0.0, 1.0},
                                            {0.0, 1.0, 0.0},
                                            {0.0, 1.0, 1.0},
                                            {1.0, 0.0, 0.0},
                                            {1.0, 0.0, 1.0},
                                            {1.0, 1.0, 0.0}}},
                                          0);
    renderContent->setVertices(cube.getVertices());
    renderContent->setIndices(cube.getIndices());

    renderContent->uploadVertexBuffer();
    renderContent->uploadIndexBuffer();

    renderContent->updateDemoUBO(mainWindow);
}

App::App() {
    initIkura();
    setShapes();
}

void App::run() {
    while (!appEngine->shouldTerminated()) {
        auto currentFrame = mainWindow->getCurrentFrameIndex();
        ikura::BasicModelMatUBO modelMat;
        ikura::BasicSceneMatUBO sceneMat;

        modelMat.model[0] = glm::mat4(1.0);

        sceneMat.view =
            glm::lookAt({2.0, 2.0, 4.0} /* eye */, {0.0, 0.0, 0.0} /* center */,
                        glm::vec3(0.0, 0.0, 1.0) /* up */);
        sceneMat.proj = glm::perspective(glm::radians(45.0f),
                                         mainWindow->getWidth() /
                                             (float)mainWindow->getHeight(),
                                         0.01f, 1000.0f);
        // Convert to RightHand Z-up
        sceneMat.proj[1][1] *= -1;

        renderContent->updateUniformBuffer(currentFrame, modelMat, sceneMat);

        appEngine->drawAllWindows();
    }

    renderEngine->waitForDeviceIdle();
}