#pragma once

#include <array>
#include <memory>

#include "../cube/singleColorCube.hpp"
#include "../shape.hpp"

// TODO: make another abstract class as parent

namespace ikura {
namespace shapes {
class DirectionDebugObject : public Shape {
    float size;
    /// x, y, z
    std::array<std::unique_ptr<SingleColorCube>, 3> axes;

  public:
    DirectionDebugObject(float size, GroupID id);
};
} // namespace shapes
} // namespace ikura