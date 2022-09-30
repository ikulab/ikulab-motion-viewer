#pragma once

#include <algorithm>
#include <cmath>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./keyboard.hpp"
#include "./mouse.hpp"

class Camera {
  public:
    glm::vec3 center;
    /// in Radians
    float hRotation;
    /// in Radians
    float vRotation;
    float distance;

    void init();
    void updateCamera(Mouse mouseCtx, Keyboard keyCtx, bool isWindowFocused);

    glm::vec3 generatePos();
    glm::mat4 generateViewMat();
};
