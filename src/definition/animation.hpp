#pragma once

#include <cstdint>

typedef uint32_t JointID;

struct Motion {
	glm::vec3 pos = {};
	glm::vec3 rot = {};
};
