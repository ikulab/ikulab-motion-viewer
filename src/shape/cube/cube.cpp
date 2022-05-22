#include <iostream>
#include <algorithm>

#include "cube.hpp"

Cube::Cube(float width, float height, float depth, glm::vec3 pos, JointID id)
	:Shape(id), width(width), height(height), depth(depth), pos(pos) {}