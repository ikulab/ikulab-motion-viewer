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
	Cube(float width, float height, float depth, glm::vec3 pos, uint32_t id=0);
};

class GradationCube : public Shape {
	float width, height, depth;
	glm::vec3 pos;
public:
	GradationCube(float width, float height, float depth, glm::vec3 pos);
};