#pragma once

#include <glm/glm.hpp>

#include "./gradationColorCube.hpp"

class SingleColorCube : public GradationColorCube {
public:
	SingleColorCube(float width, float height, float depth, glm::vec3 pos, glm::vec3 color, JointID id);
};