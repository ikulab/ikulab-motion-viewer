#include <algorithm>

#include "./shape.hpp"

void Shape::setBaseIndex(uint32_t baseIndex) {
    std::for_each(indices.begin(), indices.end(), [=](uint32_t& index) {
        index += baseIndex;
    });
}
