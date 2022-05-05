#include <cmath>

#include "./bone.hpp"

Bone::Bone(glm::vec3 rootPos, glm::vec3 tipPos)
	: rootPos{ rootPos }, tipPos{ tipPos } {

	length = glm::distance(rootPos, tipPos);
	subVec = tipPos - rootPos;

	// Top vertex
	vertices.push_back({
		{0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}
	});

	vertices.insert(vertices.end(), {
		{{-0.1, -0.1, 0.9}, {1.0, 1.0, 1.0}},
		{{-0.1,  0.1, 0.9}, {1.0, 1.0, 1.0}},
		{{ 0.1,  0.1, 0.9}, {1.0, 1.0, 1.0}},
		{{ 0.1, -0.1, 0.9}, {1.0, 1.0, 1.0}}
	});

	// Bottom vertex
	vertices.push_back({
		{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}
	});

	indices.assign({
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		5, 2, 1,
		5, 3, 2,
		5, 4, 3,
		5, 1, 4
	});
}