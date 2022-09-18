#pragma once

#include <memory>

#include <ikura.hpp>

class App {
    std::unique_ptr<ikura::AppEngine> appEngine;
    std::shared_ptr<ikura::RenderEngine> renderEngine;

  public:
    App();
    void init();
};