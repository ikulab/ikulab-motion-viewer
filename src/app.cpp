#include <iostream>
#include <memory>
#include <array>
#include <algorithm>

#include <glm/glm.hpp>

#include "app.hpp"
#include "base.hpp"

#include "animator.hpp"

#include "./shape/bone/stickTetrahedronBone.hpp"
#include "./shape/floor/filledFloor.hpp"
#include "./shape/floor/gridFloor.hpp"

void App::init() {
	base = std::make_unique<Base>();

	anim = std::make_shared<Animator>();
	anim->initFromBVH("./models/swing.bvh");
	// anim->showSkeltonInfo();
	// anim->showMotionInfo();

	createShapes();
	registerShapes();

	base->setAnimator(anim);
	base->initVulkan();
	base->initImGui();
}

void App::createShapes() {
	// staging shapes vector
	std::vector<std::unique_ptr<Shape>> tmpShapes;
	// staging bones array
	std::array<std::unique_ptr<Shape>, NUM_OF_JOINT_ID> tmpBones = anim->generateBones();
	// move only existing bones to shapes vector
	for (auto& elm : tmpBones) {
		if (elm == nullptr) {
			continue;
		}
		tmpShapes.push_back(std::move(elm));
	}
	// push floor to shapes vector
	// tmpShapes.push_back(std::move(std::make_unique<FilledFloor>(
	// 	7.0, 7.0,
	// 	glm::vec3(0.15f),
	// 	FLOOR_ID
	// )));
	tmpShapes.push_back(std::move(std::make_unique<GridFloor>(
		7.0, 7.0,
		0.01, 10, 10,
		glm::vec3(0.2, 0.9, 0.2),
		FLOOR_ID
	)));

	// set base index
	uint32_t baseIndex = 0;
	for (int i = 0; i < anim->getNumOfJoints() + NUM_OF_ID_OTHER_THAN_JOINTS; i++) {
		tmpShapes[i]->setBaseIndex(baseIndex);
		baseIndex += static_cast<uint32_t>(tmpShapes[i]->getVertices().size());
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
		base->addIndices(shape->getIndices());
	}
}

void App::run() {
	while (!base->windowShouldClose()) {
		base->vSync();
		base->updateClock();

		base->pollWindowEvent();
		base->updateCamera();

		base->drawImGuiFrame();
		base->drawFrame();

		base->resetMouseCtx();
	}
}
