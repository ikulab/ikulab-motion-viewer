#pragma once

#include "../shape.hpp"

class Bone : public Shape {
public:
	Bone(float length, JointID id);
};