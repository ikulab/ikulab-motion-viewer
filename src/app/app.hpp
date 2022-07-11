#pragma once

#include <memory>

#include <ikura/engine/appEngine.hpp>
#include <ikura/engine/renderEngine/renderEngine.hpp>

class App {
	std::unique_ptr<AppEngine> appEngine;
	std::unique_ptr<RenderEngine> renderEngine;
public:
	App();
	void init();
};