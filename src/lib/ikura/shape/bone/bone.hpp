#pragma once

#include "../shape.hpp"

namespace ikura {
namespace shapes {

class Bone : public Shape {
    float length;

  protected:
    Bone(float length, JointID id);
};
} // namespace shapes
} // namespace ikura