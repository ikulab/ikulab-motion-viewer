#pragma once

#include <iostream>
#include <vector>

#include "../definition/vertex.hpp"

class Shape {
protected:
	uint32_t baseIndex;
public:
	Shape(uint32_t baseIndex) : baseIndex(baseIndex) {};

	virtual std::vector<Vertex> getVertices() const {
		return std::vector<Vertex>();
	};
	virtual std::vector<uint32_t> getIndices() const {
		return std::vector<uint32_t>();
	};

	void setBaseIndex(uint32_t index);
};