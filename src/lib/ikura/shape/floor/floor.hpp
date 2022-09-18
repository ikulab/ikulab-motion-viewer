#pragma once

#include "../shape.hpp"

namespace ikura {
namespace shapes {
class Floor : public Shape {
    float width, height;

  public:
    Floor(float width, float height, JointID id);
};
} // namespace shapes
} // namespace ikura