#include <iostream>
#include <memory>
#include <array>
#include <algorithm>

#include <glm/glm.hpp>

#include "app.hpp"
#include "base.hpp"

#include "animator.hpp"

#include "./shape/bone/stickTetrahedronBone.hpp"
#include "./shape/floor/gridFloor.hpp"
#include "./shape/floor/filledFloor.hpp"

#include "./shape/debug/directionDebugObject.hpp"

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
	tmpShapes.push_back(std::move(std::make_unique<GridFloor>(
		1000.0, 1000.0,
		1, 10, 10,
		glm::vec3(0.2, 0.9, 0.2),
		FLOOR_ID
	)));
	tmpShapes.push_back(std::move(std::make_unique<DirectionDebugObject>(
		40.0, DEBUG_OBJECT_ID
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

void App::updateUbo() {
	std::array<glm::mat4, NUM_OF_JOINT_ID> modelMats
		= anim->generateModelMatrices(base->getSecondsFromStart());

	// bone transformation
	for (int i = 0; i < anim->getNumOfJoints(); i++) {
		modelUbo->model[i] = modelMats[i];
	}

	// debug
#ifdef NODEBUG
	modelUbo->model[DEBUG_OBJECT_ID] = glm::mat4(0.0);
#endif

	// global scaling
	for (auto& m : modelUbo->model) {
		m = glm::scale(glm::mat4(1.0), glm::vec3(0.01)) * m;
	}

	SceneMatUBO sceneUbo;
	VkExtent2D extent = base->getSwapChainExtent();
	sceneUbo.view = camera->generateViewMat();
	sceneUbo.proj = glm::perspective(
		glm::radians(45.0f),
		extent.width / (float)extent.height,
		0.01f,
		1000.0f
	);

	// デフォルトでは 左手系 Z-down になっている
	// この式によって 右手系 Z-up に変換する
	sceneUbo.proj[1][1] *= -1;

	vkMapMemory(
		device,
		uniformBufferMemories[currentImage][DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO],
		0, sizeof(SceneMatUBO), 0, &data);
	memcpy(data, &sceneUbo, sizeof(SceneMatUBO));
	vkUnmapMemory(device, uniformBufferMemories[currentImage][DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO]);
}

void App::run() {
	while (!base->windowShouldClose()) {
		base->vSync();
		base->updateClock();

		base->pollWindowEvent();
		base->updateCamera();

		base->drawImGuiFrame();
		base->drawFrame();

		base->resetMouseInputContext();
	}
}
