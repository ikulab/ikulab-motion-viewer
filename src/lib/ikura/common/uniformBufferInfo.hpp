#pragma once

#include <glm/glm.hpp>

const int NUM_OF_JOINT_ID = 256;
const int NUM_OF_ID_OTHER_THAN_JOINTS = 2;
const int NUM_OF_ID = NUM_OF_JOINT_ID + NUM_OF_ID_OTHER_THAN_JOINTS;

struct ModelMatUBO {
    alignas(16) glm::mat4 model[NUM_OF_ID];
};

struct SceneMatUBO {
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};
