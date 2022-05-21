#pragma once

#include <cstdint>

#include "./bone.hpp"

class StickBone : public Bone {
public:
	StickBone(uint8_t numSplit, float length, JointID id);
};