#pragma once

#include <memory>
#include <vector>

#include "./base.hpp"
#include "./shape/shape.hpp"
#include "./animator.hpp"

class App {
	std::unique_ptr<Base> base;
	std::vector<std::unique_ptr<Shape>> shapes;

	std::shared_ptr<Animator> anim;

	void init();
	void createShapes();
	void registerShapes();

public:
	App() {
		init();
	}

	void run();
};