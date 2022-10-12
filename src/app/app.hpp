#pragma once

#include <memory>

#include <ikura.hpp>

#include "./context/camera.hpp"
#include "./context/keyboard.hpp"
#include "./context/mouse.hpp"
#include "./context/ui.hpp"
#include "./motionUtil/animator.hpp"

class App {
    // Variables ==========
    // Constants ----------
    const int NUM_OF_GROUPS_OTHER_THAN_JOINTS = 2;
    const ikura::GroupID AXIS_OBJ_GROUP_ID = ikura::NUM_OF_MODEL_MATRIX - 3;
    const ikura::GroupID FLOOR_GROUP_ID = ikura::NUM_OF_MODEL_MATRIX - 2;

    // ikura objects ----------
    std::unique_ptr<ikura::AppEngine> appEngine;
    std::shared_ptr<ikura::RenderEngine> renderEngine;

    std::shared_ptr<ikura::GlfwNativeWindow> mainWindow;
    std::shared_ptr<ikura::BasicRenderTarget> mainRenderTarget;
    std::shared_ptr<ikura::BasicRenderContent> mainRenderContent;

    std::shared_ptr<ikura::ImGuiVirtualWindow> imGuiVirtualWindow;

    std::shared_ptr<ikura::BasicRenderComponentProvider>
        basicRenderComponentProvider;

    // Contexts ----------
    Camera camera;
    Keyboard keyboard;
    Mouse mouse;
    UI ui;

    // Flags ----------
    bool modelLoaded = false;

    // Animation ----------
    Animator animator;
    float animationTime = 0.0;
    float animationSpeed = 1.0;
    bool stopAnimation = false;
    bool initAnimationTime = false;

    // Functions ==========
    // Init ----------
    void initIkura();
    void setShapes(const char *filePath);
    void initContexts();
    void setGlfwWindowEvents(GLFWwindow *window);

    void selectFileAndInitShapes();

    // Update ----------
    void updateMatrices();

    // UI ----------
    void updateUI();
    void updateMainMenu();
    void updateAnimationControlWindow();
    void updateDebugWindow();

    // Glfw Callbacks ----------
    static void cursorPositionCallback(GLFWwindow *window, double xPos,
                                       double yPos);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                    int mods);
    static void scrollCallback(GLFWwindow *window, double xOffset,
                               double yOffset);
    static void keyCallback(GLFWwindow *window, int key, int scanCode,
                            int action, int mods);

  public:
    App();
    void run();
};