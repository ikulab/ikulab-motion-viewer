#pragma once

#include <iostream>
#include <vector>

#include "../common/renderPrimitiveTypes.hpp"
#include "../definition/animation.hpp"

namespace ikura {
namespace shapes {
class Shape {
  protected:
    std::vector<BasicVertex> vertices;
    std::vector<BasicIndex> indices;
    JointID id;

  public:
    Shape(JointID id);

    const virtual std::vector<BasicVertex> &getVertices() const;
    const virtual std::vector<BasicIndex> &getIndices() const;

    void virtual setBaseIndex(BasicIndex baseIndex);
};
} // namespace shapes
} // namespace ikura