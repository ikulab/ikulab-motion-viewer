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
		std::cout << "constructing Cube" << std::endl;
		this->baseIndex = baseIndex;
	};

	std::vector<Vertex> getVertices() const;
	std::vector<uint32_t> getIndices() const;
};

class GradationCube : public Shape {
	float width, height, depth;
	glm::vec3 pos;
public:
	GradationCube(float width, float height, float depth, glm::vec3 pos, uint32_t baseIndex = 0) : width(width), height(height), depth(depth), pos(pos) {
		std::cout << "constructing GradationCube" << std::endl;
		this->baseIndex = baseIndex;
	};

	std::vector<Vertex> getVertices() const override;
	std::vector<uint32_t> getIndices() const override;
};