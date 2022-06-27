#pragma once

#include <memory>
#include <vector>

#include "./base.hpp"
#include "./shape/shape.hpp"
#include "./animator.hpp"
#include "./context/camera.hpp"
#include "./context/keyboard.hpp"
#include "./context/mouse.hpp"
#include "./definition/ubo.hpp"

class App {
	std::unique_ptr<Base> base;
	std::vector<std::unique_ptr<Shape>> shapes;

	// TODO: to unique_ptr
	std::shared_ptr<Animator> anim;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Keyboard> keyboard;
	std::unique_ptr<Mouse> mouse;

	std::unique_ptr<ModelMatUBO> modelUbo;
	std::unique_ptr<SceneMatUBO> sceneUbo;

	void init();
	void createShapes();
	void registerShapes();

	void updateUbo();

public:
	App() {
		init();
	}

	void run();
};