#pragma once

#include <cstdint>

#include <glm/glm.hpp>

typedef uint32_t JointID;

struct Motion {
    glm::vec3 pos = {};
    glm::vec3 rot = {};
};
