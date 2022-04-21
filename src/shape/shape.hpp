#pragma once

#include <vector>

#include "../definition/vertex.hpp"

class Shape {
protected:
	uint32_t baseIndex;
public:
	virtual std::vector<Vertex> getVertices() {
		return std::vector<Vertex>();
	};
	virtual std::vector<uint32_t> getIndices() {
		return std::vector<uint32_t>();
	};
};