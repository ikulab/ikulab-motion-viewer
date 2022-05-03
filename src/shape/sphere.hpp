#pragma once

#include <glm/glm.hpp>

#include "./shape.hpp"

class Sphere : public Shape {
	glm::vec3 pos;
	float r;
	uint32_t numSplitV, numSplitH;
public:
	Sphere(glm::vec3 pos, float r, uint32_t numSplitV, uint32_t numSplitH);
};