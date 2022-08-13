#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <ikura/engine/appEngine.hpp>
#include <ikura/engine/renderEngine/renderEngine.hpp>

class App {
	std::unique_ptr<AppEngine> appEngine;
	std::shared_ptr<RenderEngine> renderEngine;
public:
	App();
	void init();
};