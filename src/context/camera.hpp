#pragma once

#include <cmath>
#include <algorithm>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

#include "./mouse.hpp"
#include "./keyboard.hpp"

class Camera {
	glm::vec3 center{ 0.0, 0.0, 0.0 };
	//// in Radians
	float hRotation = 0.0;
	//// in Radians
	float vRotation = glm::radians(20.0);
	float distance = 10.0;

public:
	void updateCamera(Mouse mouseCtx, Keyboard keyCtx);

	glm::vec3 generatePos();
	glm::mat4 generateViewMat();
};
