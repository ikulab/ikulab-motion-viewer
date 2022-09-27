#include "./app.hpp"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>

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
    vk::SurfaceKHR surface = vkSurface;

    // Create Device in RenderEngine (requires sample Surface)
    renderEngine->setSampleSurface(surface);
    renderEngine->createDevice();

    // Initialize AppEngine
    appEngine = std::make_unique<ikura::AppEngine>(renderEngine);

    // Setup main ikura Window ----------
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    int xpos, ypos;
    glfwGetMonitorWorkarea(primaryMonitor, &xpos, &ypos, nullptr, nullptr);

    glfwSetWindowPos(glfwWindow, xpos + 100, ypos + 100);
    mainWindow = std::make_shared<ikura::GlfwNativeWindow>(
        renderEngine, glfwWindow, surface, "main");

    basicRenderComponentProvider =
        std::make_shared<ikura::BasicRenderComponentProvider>(renderEngine);
    mainRenderTarget =
        basicRenderComponentProvider->createBasicRenderTarget(mainWindow);
    mainRenderContent =
        basicRenderComponentProvider->createBasicRenderContent(mainWindow);

    mainWindow->setRenderTarget(mainRenderTarget);
    mainWindow->setRenderContent(mainRenderContent);

    // Setup ikura sub Window ----------
    glfwWindow =
        glfwCreateWindow(400, 300, "Ikura Window Sub", nullptr, nullptr);
    glfwSetWindowPos(glfwWindow, xpos + 600, ypos + 100);

    if ((glfwCreateWindowSurface(renderEngine->getInstance(), glfwWindow,
                                 nullptr, &vkSurface)) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create VkSurfaceKHR from glfwCreateWindowSurface().");
    }
    surface = vkSurface;

    subWindow = std::make_shared<ikura::GlfwNativeWindow>(
        renderEngine, glfwWindow, surface, "sub");

    subRenderTarget =
        basicRenderComponentProvider->createBasicRenderTarget(subWindow);
    subWindow->setRenderTarget(subRenderTarget);
    subWindow->setRenderContent(mainRenderContent);

    // Add Window ----------
    appEngine->addWindow(mainWindow);
    appEngine->addWindow(subWindow);
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
    mainRenderContent->setVertices(cube.getVertices());
    mainRenderContent->setIndices(cube.getIndices());

    mainRenderContent->uploadVertexBuffer();
    mainRenderContent->uploadIndexBuffer();
}

void App::setGlfwWindowEvents(GLFWwindow *window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
}

App::App() {
    initIkura();
    setShapes();
    setGlfwWindowEvents(mainWindow->getGLFWWindow());
}

void App::run() {
    float angle = 0.0;
    appEngine->setStartTime();

    while (!appEngine->shouldTerminated()) {
        appEngine->vSync();
        camera.updateCamera(mouse, keyboard);
        mouse.resetScroll();

        auto currentFrame = mainWindow->getCurrentFrameIndex();
        ikura::BasicModelMatUBO modelMat;
        ikura::BasicSceneMatUBO sceneMat;

        modelMat.model[0] =
            glm::rotate(glm::mat4(1.0), angle, glm::vec3(0, 0, 1.0));
        angle = M_PI * appEngine->getSecondsFromStart() / 4.0;

        sceneMat.view = camera.generateViewMat();
        sceneMat.proj = glm::perspective(glm::radians(45.0f),
                                         mainWindow->getWidth() /
                                             (float)mainWindow->getHeight(),
                                         0.01f, 1000.0f);
        // Convert to RightHand Z-up
        sceneMat.proj[1][1] *= -1;

        mainRenderContent->updateUniformBuffer(currentFrame, modelMat,
                                               sceneMat);

        appEngine->drawAllWindows();
        appEngine->destroyClosedWindow();
    }

    renderEngine->waitForDeviceIdle();
}