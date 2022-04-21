#include <memory>

#include <glm/glm.hpp>

#include "app.hpp"
#include "base.hpp"

#include "shape/shape.hpp"
#include "shape/cube.hpp"

void App::init() {
	base = std::make_unique<Base>();

	createShapes();
	registerShapes();

	base->initVulkan();
}

void App::createShapes() {
	auto upGradationCube = std::make_unique<GradationCube>(
		1.0, 2.0, 0.5, glm::vec3(-1.5, 0.0, 0.0)
	);
	auto upCube = std::make_unique<Cube>(
		1.0, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.0),
		upGradationCube->getVertices().size()
	);

	shapes.push_back(std::move(upGradationCube));
	shapes.push_back(std::move(upCube));
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
