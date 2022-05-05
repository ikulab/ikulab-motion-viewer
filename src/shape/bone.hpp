#pragma once

#include <glm/glm.hpp>

#include "./shape.hpp"

class Bone : public Shape {
	glm::vec3 rootPos, tipPos;
	glm::vec3 subVec;
	float length;
public:
	Bone(glm::vec3 rootPos, glm::vec3 tipPos);
};