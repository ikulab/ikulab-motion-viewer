#pragma once

#include <vector>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "../definition/vertex.hpp"

class Cube {
	float width, height, depth;
	glm::vec3 pos;
public:
	Cube(float width, float height, float depth, glm::vec3 pos) : width(width), height(height), depth(depth), pos(pos) {};

	std::vector<Vertex> getVertices();
	std::vector<uint32_t> getIndices();
};