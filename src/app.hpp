#pragma once

#include <memory>
#include <vector>

#include "base.hpp"
#include "shape/shape.hpp"

class App {
	std::unique_ptr<Base> base;
	std::vector<std::unique_ptr<Shape>> shapes;

	void init();
	void createShapes();
	void registerShapes();

public:
	App() {
		init();
	}

	void run();
};