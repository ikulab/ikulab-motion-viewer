#pragma once

#include <vector>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "shape.hpp"
#include "../definition/vertex.hpp"

class Cube : public Shape {
	float width, height, depth;
	glm::vec3 pos;
public:
	Cube(float width, float height, float depth, glm::vec3 pos, uint32_t baseIndex = 0) : width(width), height(height), depth(depth), pos(pos) {
		this->baseIndex = baseIndex;
	};

	std::vector<Vertex> getVertices();
	std::vector<uint32_t> getIndices();
};

class GradationCube : public Shape {
	float width, height, depth;
	glm::vec3 pos;
public:
	GradationCube(float width, float height, float depth, glm::vec3 pos, uint32_t baseIndex = 0) : width(width), height(height), depth(depth), pos(pos) {
		this->baseIndex = baseIndex;
	};

	std::vector<Vertex> getVertices();
	std::vector<uint32_t> getIndices();
};