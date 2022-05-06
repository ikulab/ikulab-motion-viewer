#include <iostream>
#include <memory>
#include <array>

#include <glm/glm.hpp>

#include "app.hpp"
#include "base.hpp"

#include "shape/shape.hpp"
#include "shape/cube.hpp"
#include "shape/sphere.hpp"
#include "shape/bone.hpp"

void App::init() {
	base = std::make_unique<Base>();

	createShapes();
	registerShapes();

	base->initVulkan();
}

void App::createShapes() {
	// "staging" array
	std::array<std::unique_ptr<Shape>, 1> tmpShapes {
		std::make_unique<Cube>(
			1.0, 1.0, 1.0,
			glm::vec3(0.0, 0.0, 0.0)
		)
	};

	// set base index
	uint32_t baseIndex = 0;
	for (const auto& shape : tmpShapes) {
		shape->setBaseIndex(baseIndex);
		baseIndex += static_cast<uint32_t>(shape->getVertices().size());
	}

	// move to shapes vector
	shapes.assign(
		std::make_move_iterator(tmpShapes.begin()),
		std::make_move_iterator(tmpShapes.end())
	);
}

void App::registerShapes() {
	for (const auto& shape : shapes) {
		base->addVertices(shape->getVertices());
		base->addindices(shape->getIndices());
	}
}

void App::run() {
	while (!base->windowShouldClose()) {
		base->pollWindowEvent();
		base->drawFrame();
	}
}
