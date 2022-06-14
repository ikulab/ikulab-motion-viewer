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
	// "staging" array
	std::array<std::unique_ptr<Shape>, MAX_ID> tmpShapes = anim->generateBones();
	// set base index
	uint32_t baseIndex = 0;
	for (int i = 0; i < anim->getNumOfJoints(); i++) {
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
	for (int i = 0; i < anim->getNumOfJoints(); i++) {
		base->addVertices(shapes[i]->getVertices());
		base->addindices(shapes[i]->getIndices());
	}
}

void App::run() {
	while (!base->windowShouldClose()) {
		base->vSync();
		base->updateClock();
		base->updateCamera();
		base->pollWindowEvent();
		base->drawImGuiFrame();
		base->drawFrame();

		base->resetMouseCtx();
	}
}
