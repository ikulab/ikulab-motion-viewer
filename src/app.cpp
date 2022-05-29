#include <iostream>
#include <memory>
#include <array>
#include <algorithm>

#include <glm/glm.hpp>

#include "app.hpp"
#include "base.hpp"

#include "animator.hpp"

#include "./shape/bone/stickTetrahedronBone.hpp"

void App::init() {
	base = std::make_unique<Base>();

	Animator anim;
	anim.initFromBVH("./models/swing.bvh");
	// anim.showSkeltonInfo();
	// anim.showMotionInfo();

	createShapes();
	registerShapes();

	base->initVulkan();
}

void App::createShapes() {
	// "staging" array
	std::array<std::unique_ptr<Shape>, 3> tmpShapes{
		std::make_unique<StickTetrahedronBone>(
			5, 1
		),
		std::make_unique<StickTetrahedronBone>(
			0.5, 0
		),
		std::make_unique<SingleColorCube>(
			7, 7, 0.2,
			glm::vec3(0, 0, -1.0),
			glm::vec3(0.2, 0.2, 0.2),
			2
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
