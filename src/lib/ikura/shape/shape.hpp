#pragma once

#include <iostream>
#include <vector>

#include "../common/renderPrimitiveTypes.hpp"
#include "../definition/animation.hpp"

namespace ikura {
namespace shapes {
class Shape {
  protected:
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    JointID id;

  public:
    Shape(JointID id);

    const virtual std::vector<Vertex> &getVertices() const;
    const virtual std::vector<Index> &getIndices() const;

    void virtual setBaseIndex(Index baseIndex);
};
} // namespace shapes
} // namespace ikura