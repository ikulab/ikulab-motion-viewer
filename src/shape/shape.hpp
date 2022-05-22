#pragma once

#include <iostream>
#include <vector>

#include "../definition/vertex.hpp"
#include "../definition/animation.hpp"

class Shape {
protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	JointID id;
public:
	Shape(JointID id);

	const std::vector<Vertex>& getVertices() const;
	const std::vector<uint32_t>& getIndices() const;

	void setBaseIndex(uint32_t baseIndex);
};