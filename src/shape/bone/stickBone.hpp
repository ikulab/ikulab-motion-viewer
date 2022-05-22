#pragma once

#include <cstdint>

#include "./bone.hpp"

class StickBone : public Bone {
	int numSplit;
public:
	StickBone(int numSplit, float length, JointID id);
};