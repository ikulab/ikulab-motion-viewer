#pragma once

#include <glm/glm.hpp>

#include "./shape.hpp"

class Bone : public Shape {
public:
	Bone(JointID id);
};