#pragma once

#include <memory>
#include <vector>

/*
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

	// GLFW event callbacks ---
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
	void registerInputEvents();
	// ---

public:
	App() {
		init();
	}

	void run();
};
*/