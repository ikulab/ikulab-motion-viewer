#pragma once

#include "../shape.hpp"

class Floor : public Shape {
    float width, height;

  public:
    Floor(float width, float height, JointID id);
};