#pragma once

#include <memory>

#include <ikura.hpp>

class App {
    std::unique_ptr<ikura::AppEngine> appEngine;
    std::shared_ptr<ikura::RenderEngine> renderEngine;

    std::shared_ptr<ikura::GlfwNativeWindow> mainWindow;
    std::shared_ptr<ikura::BasicRenderTarget> renderTarget;
    std::shared_ptr<ikura::BasicRenderContent> renderContent;

    void initIkura();
    void setShapes();

  public:
    App();
    void run();
};