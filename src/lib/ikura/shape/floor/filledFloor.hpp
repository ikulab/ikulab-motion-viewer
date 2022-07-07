#pragma once

#include "./floor.hpp"

class FilledFloor : public Floor {
	glm::vec3 color;
public:
	FilledFloor(float width, float height, glm::vec3 color, bool frontSideOnly, JointID id);
};