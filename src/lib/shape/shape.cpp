#include <algorithm>

#include "./shape.hpp"

Shape::Shape(JointID id) : id(id) {}

const std::vector<Vertex>& Shape::getVertices() const {
    return vertices;
};

const std::vector<uint32_t>& Shape::getIndices() const {
    return indices;
};

void Shape::setBaseIndex(uint32_t baseIndex) {
    std::for_each(indices.begin(), indices.end(), [=](uint32_t& index) {
        index += baseIndex;
    });
}