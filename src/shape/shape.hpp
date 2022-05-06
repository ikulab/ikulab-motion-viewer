#pragma once

#include <iostream>
#include <vector>

#include "../definition/vertex.hpp"

class Shape {
protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	uint32_t id = 0;
public:
	inline const std::vector<Vertex>& getVertices() const {
		return vertices;
	};
	inline const std::vector<uint32_t>& getIndices() const {
		return indices;
	};

	void setBaseIndex(uint32_t baseIndex);
};