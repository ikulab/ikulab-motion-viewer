#pragma once

#include <array>
#include <glm/glm.hpp>

#include "./cube.hpp"

class SeparatedColorCube : public Cube {
	/// order is: [top, bottom, left, right, front, back]
	std::array<glm::vec3, 6> colors;
public:
	SeparatedColorCube(float width, float height, float depth, glm::vec3 pos, std::array<glm::vec3, 6> colors, JointID id);
};
