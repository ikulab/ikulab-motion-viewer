#include <cmath>

#include "./stickBone.hpp"

StickBone::StickBone(int numSplit, float length, JointID id) : Bone(length, id), numSplit(numSplit) {
	
}