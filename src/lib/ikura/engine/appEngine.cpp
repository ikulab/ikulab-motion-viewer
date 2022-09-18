#include "./appEngine.hpp"

#include <iostream>

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

#include "../shape/shapes.hpp"

AppEngine::AppEngine(std::shared_ptr<RenderEngine> renderEngine) {
    this->renderEngine = renderEngine;
}

void AppEngine::addWindow(
    std::shared_ptr<ikura::GlfwNativeWindow> glfwNativeWindow) {
    glfwNativeWindow->createDefaultDescriptorSetLayout();
    glfwNativeWindow->addDefaultRenderTarget();
    glfwNativeWindow->addDefaultRenderContent();

    windows.push_back(glfwNativeWindow);
    addTestShapes();
}

void AppEngine::addWindow(
    std::shared_ptr<ikura::ImGuiVirtualWindow> imGuiVirtualWindow) {
    windows.push_back(imGuiVirtualWindow);
}

int AppEngine::shouldTerminated() {
    return std::any_of(windows.begin(), windows.end(),
                       [&](const std::shared_ptr<ikura::Window> window) {
                           auto pw = dynamic_cast<ikura::NativeWindow *>(
                               window.get());
                           if (pw != nullptr) {
                               return pw->windowShouldClose();
                           }
                           return 0;
                       });
}

void AppEngine::drawAllWindows() {
    // Poll GLFW Events (execute once per loop)
    for (const auto &window : windows) {
        auto pw = dynamic_cast<ikura::GlfwNativeWindow *>(window.get());
        if (pw != nullptr) {
            glfwPollEvents();
            break;
        }
    }

    for (auto &window : windows) {
        window->draw();
    }
}

void AppEngine::addTestShapes() {
    SingleColorCube cube(1.0, 1.0, 1.0, {0, 0, 0}, {1.0, 1.0, 1.0}, 0);

    windows[0]->getRenderContent()->setVertices(cube.getVertices());
    windows[0]->getRenderContent()->setIndices(cube.getIndices());

    ModelMatUBO modelMat;
    modelMat.model[0] = glm::mat4(1.0);

    SceneMatUBO sceneMat;
    sceneMat.view =
        glm::lookAt({2.0, 2.0, 4.0} /* eye */, {0.0, 0.0, 0.0} /* center */,
                    glm::vec3(0.0, 0.0, 1.0) /* up */);
    sceneMat.proj = glm::perspective(glm::radians(45.0f),
                                     windows[0]->getWidth() /
                                         (float)windows[0]->getHeight(),
                                     0.01f, 1000.0f);
    // Convert to RightHand Z-up
    sceneMat.proj[1][1] *= -1;

    windows[0]->getRenderContent()->updateUniformBuffer(modelMat, sceneMat);
}