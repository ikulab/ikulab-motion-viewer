#pragma once

#include "../shape.hpp"

// TODO: make it pure abstract

class Bone : public Shape {
    float length;

  public:
    // make constructor protected
    Bone(float length, JointID id);
};