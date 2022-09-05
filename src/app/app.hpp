#pragma once

#include <memory>

#include <ikura.hpp>

class App {
    std::unique_ptr<AppEngine> appEngine;
    std::shared_ptr<RenderEngine> renderEngine;

  public:
    App();
    void init();
};