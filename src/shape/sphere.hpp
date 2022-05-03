#pragma once

#include <glm/glm.hpp>

#include "./shape.hpp"

class Sphere : public Shape {
	glm::vec3 pos;
	float r;
	uint32_t numSplitZ, numSplitXY;
public:
	Sphere(glm::vec3 pos, float r, uint32_t numSplitZ, uint32_t numSplitXY, uint32_t baseIndex = 0)
		: pos(pos), r(r), numSplitZ(numSplitZ), numSplitXY(numSplitXY), Shape(baseIndex) {};

	std::vector<Vertex> getVertices() const override;
	std::vector<uint32_t> getIndices() const override;
};