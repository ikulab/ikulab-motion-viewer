#pragma once

#include <glm/glm.hpp>

#include "./shape.hpp"

class Sphere : public Shape {
	glm::vec3 pos;
	float r;
	uint32_t numSplitV, numSplitH;
public:
	Sphere(glm::vec3 pos, float r, uint32_t numSplitV, uint32_t numSplitH, uint32_t baseIndex = 0)
		: pos(pos), r(r), numSplitV(numSplitV), numSplitH(numSplitH), Shape(baseIndex) {};

	std::vector<Vertex> getVertices() const override;
	std::vector<uint32_t> getIndices() const override;
};