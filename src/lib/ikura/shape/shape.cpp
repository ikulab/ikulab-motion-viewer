#include <algorithm>

#include "./shape.hpp"

namespace ikura {
namespace shapes {
Shape::Shape(JointID id) : id(id) {}

const std::vector<BasicVertex> &Shape::getVertices() const { return vertices; };

const std::vector<BasicIndex> &Shape::getIndices() const { return indices; };

void Shape::setBaseIndex(BasicIndex baseIndex) {
    std::for_each(indices.begin(), indices.end(),
                  [=](BasicIndex &index) { index += baseIndex; });
}
} // namespace shapes
} // namespace ikura