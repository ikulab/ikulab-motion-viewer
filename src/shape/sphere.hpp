#pragma once

#include <glm/glm.hpp>

#include "./shape.hpp"

class Sphere : public Shape {
	glm::vec3 pos;
	float r;
	uint32_t numSplitH, numSplitV;
public:
	Sphere(float r, uint32_t numSplitH, uint32_t numSplitV, glm::vec3 pos, JointID id);
};