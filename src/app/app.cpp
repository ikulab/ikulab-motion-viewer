#include "./app.hpp"

#include <algorithm>
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <tinyfiledialogs.h>

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
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    int xpos, ypos, monitorW, monitorH;
    glfwGetMonitorWorkarea(primaryMonitor, &xpos, &ypos, &monitorW, &monitorH);

    int windowW, windowH;
    if (monitorW < 1920 || monitorH < 1080) {
        windowW = monitorW;
        windowH = monitorH;
    } else {
        windowW = 1920;
        windowH = 1080;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *glfwWindow =
        glfwCreateWindow(windowW, windowH, "Ikura Window", nullptr, nullptr);
    glfwSetWindowPos(glfwWindow, xpos, ypos);

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
    setGlfwWindowEvents(mainWindow->getGLFWWindow());

    // Setup ikura ImGuiWindow ----------
    imGuiVirtualWindow =
        std::make_shared<ikura::ImGuiVirtualWindow>(renderEngine, mainWindow);

    // Add Window ----------
    appEngine->addWindow(mainWindow);
    // appEngine->addWindow(subWindow);

    mainWindow->addVirtualWindow(imGuiVirtualWindow);
}

void App::setShapes() {
    // Joints ----------
    animator.initFromBVH("./models/swing.bvh");
    std::vector<std::shared_ptr<ikura::shapes::Shape>> shapes;
    animator.generateBones(shapes);

    if (shapes.size() + NUM_OF_GROUPS_OTHER_THAN_JOINTS >
        ikura::NUM_OF_MODEL_MATRIX) {
        throw std::runtime_error("Too many Joints in loaded model.");
    }

    // Add other than Joint object ----------
    ikura::BasicIndex baseIndex =
        shapes.back()->getBaseIndex() + shapes.back()->getVertices().size();

    // DebugObj
    auto debugObj = std::make_shared<ikura::shapes::DirectionDebugObject>(
        40.0, DEBUG_OBJ_GROUP_ID);
    debugObj->setBaseIndex(baseIndex);
    baseIndex += debugObj->getVertices().size();
    shapes.push_back(debugObj);

    // Floor
    auto floor = std::make_shared<ikura::shapes::GridFloor>(
        1000.0, 1000.0, 1, 10, 10, glm::vec3(0.2, 0.9, 0.2), FLOOR_GROUP_ID);
    floor->setBaseIndex(baseIndex);
    // baseIndex += floor->getVertices().size();
    shapes.push_back(floor);

    // Register all Vertices / Indices ----------
    std::vector<ikura::BasicVertex> vertices;
    std::vector<ikura::BasicIndex> indices;
    for (auto &shape : shapes) {
        vertices.insert(vertices.end(), shape->getVertices().begin(),
                        shape->getVertices().end());
        indices.insert(indices.end(), shape->getIndices().begin(),
                       shape->getIndices().end());
    }

    mainRenderContent->setVertices(vertices);
    mainRenderContent->setIndices(indices);

    mainRenderContent->uploadVertexBuffer();
    mainRenderContent->uploadIndexBuffer();
}

void App::initContexts() { camera.init(); }

void App::setGlfwWindowEvents(GLFWwindow *window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
}

void App::updateMatrices() {
    auto currentFrame = mainWindow->getCurrentFrameIndex();
    ikura::BasicModelMatUBO modelMat;
    ikura::BasicSceneMatUBO sceneMat;

    // Joints
    auto modelMat4s =
        animator.generateModelMatrices(appEngine->getSecondsFromStart());
    for (int i = 0; i < ikura::NUM_OF_MODEL_MATRIX; i++) {
        modelMat.model[i] = modelMat4s[i];
    }

    // Other objects
    modelMat.model[FLOOR_GROUP_ID] = glm::mat4(1.0);
    modelMat.model[DEBUG_OBJ_GROUP_ID] = glm::mat4(1.0);

    // global scaling
    for (auto &m : modelMat.model) {
        m = glm::scale(glm::mat4(1.0), glm::vec3(0.1)) * m;
    }

    sceneMat.view = camera.generateViewMat();
    sceneMat.proj = glm::perspective(glm::radians(45.0f),
                                     mainWindow->getWidth() /
                                         (float)mainWindow->getHeight(),
                                     0.01f, 1000.0f);
    // Convert to RightHand Z-up
    sceneMat.proj[1][1] *= -1;

    mainRenderContent->updateUniformBuffer(currentFrame, modelMat, sceneMat);
}

void App::updateUI() {
    imGuiVirtualWindow->setCurrentImGuiContext();
    imGuiVirtualWindow->newFrame();

    ImGuiIO &io = ImGui::GetIO();

    // ImGui windows
    // Indicator window
    if (!ui.windowSizeInitialized) {
        ui.windowSizeInitialized = true;
#ifndef NODEBUG
        ImGui::SetNextWindowSize(ImVec2(300, 600));
#else
        ImGui::SetNextWindowSize(ImVec2(300, 250));
#endif
    }
    ImGui::Begin("インジケーター");

#ifndef NODEBUG
    ImGui::Checkbox("ImGui DemoWindowを表示する", &ui.showDemoWindow);
    ImGui::Text("IsWindowFocused: %d", ImGui::IsWindowFocused());
    UI::makePadding(20);
#endif

    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Text("Joints: %d", animator.getNumOfJoints());

    auto total = animator.getNumOfFrames();
    auto current = animator.getCurrentFrame();
    ImGui::Text("Frame: %d / %d", current, total);

    ImGui::ProgressBar((float)current / total, ImVec2(0.0, 0.0), "");
    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x + 5.0);
    ImGui::Text("%.1f%%", (float)current / total * 100);

    UI::makePadding(30);

#ifndef NODEBUG
    // mouse input status
    ImGui::Text("Cursor Pos: (%.1f, %.1f)", mouse.currentX, mouse.currentY);
    ImGui::Text("Scroll offset: (%.1f, %.1f)", mouse.scrollOffsetX,
                mouse.scrollOffsetY);
    ImGui::Text("DragStart: (%.1f, %.1f)", mouse.dragStartX, mouse.dragStartY);
    ImGui::Text("DragEnd: (%.1f, %.1f)", mouse.dragEndX, mouse.dragEndY);
    ImGui::Text("Delta: (%.1f, %.1f)", mouse.deltaX, mouse.deltaY);
    ImGui::Text("Button L/R/M: (%d / %d / %d)", mouse.leftButton,
                mouse.rightButton, mouse.middleButton);
    // camera status
    ImGui::Text("Camera rotation (degrees) H/V: (%.2f, %.2f)",
                glm::degrees(camera.hRotation), glm::degrees(camera.vRotation));
    ImGui::Text("Camera distance: %.2f", camera.distance);
    glm::vec3 cameraPos = camera.generatePos();
    ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y,
                cameraPos.z);
    ImGui::Text("Camera look-at position: (%.2f, %.2f, %.2f)", camera.center.x,
                camera.center.y, camera.center.z);
    UI::makePadding(20);

    // window status
    ImGui::Text("Window size: (%d, %d)", mainWindow->getWidth(),
                mainWindow->getHeight());
    UI::makePadding(20);
#endif

    if (ImGui::Button("ファイルを開く...")) {
        std::cout << "TODO: implement!!" << std::endl;
    }
    ImGui::Text("未実装です m(_ _)m");

    ImGui::End();

    // Demo window
    if (ui.showDemoWindow) {
        ImGui::ShowDemoWindow();
    }

    ImGui::Render();
}

App::App() {
    initIkura();
    setShapes();
    initContexts();
}

void App::run() {
    appEngine->setStartTime();

    while (!appEngine->shouldTerminated()) {
        appEngine->vSync();

        camera.updateCamera(
            mouse, keyboard,
            std::any_of(mainWindow->getVirtualWindows().begin(),
                        mainWindow->getVirtualWindows().end(),
                        [](const std::shared_ptr<ikura::VirtualWindow> window) {
                            return window->isFocused();
                        }));
        mouse.reset();

        updateMatrices();
        updateUI();

        appEngine->drawAllWindows();
        appEngine->destroyClosedWindow();
    }

    renderEngine->waitForDeviceIdle();
}