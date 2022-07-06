#pragma once

#include <glm/glm.hpp>

#include "common.hpp"

struct ModelMatUBO {
	alignas(16) glm::mat4 model[NUM_OF_ID];
};

struct SceneMatUBO {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};
