#include "./octahedronBone.hpp"
#include "../sphere.hpp"

OctahedronBone::OctahedronBone(float length, JointID id) : Bone(length, id) {
	// Top vertex
	vertices.push_back({
		{0.0, 0.0, 1.0}, {0.8, 0.8, 0.8}, id
	});

	vertices.insert(vertices.end(), {
		{{-0.1, -0.1, 0.9}, {0.8, 0.8, 0.8}, id},
		{{-0.1,  0.1, 0.9}, {0.8, 0.8, 0.8}, id},
		{{ 0.1,  0.1, 0.9}, {0.8, 0.8, 0.8}, id},
		{{ 0.1, -0.1, 0.9}, {0.8, 0.8, 0.8}, id}
	});

	// Bottom vertex
	vertices.push_back({
		{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, id
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

	// root and top Spheres
	Sphere rootSphere(
		0.05, 10, 10,
		glm::vec3(0.0, 0.0, 0.0)
	);
	Sphere tipSphere(
		0.05, 10, 10,
		glm::vec3(0.0, 0.0, 1.0)
	);

	uint32_t baseIndex = static_cast<uint32_t>(vertices.size());
	rootSphere.setBaseIndex(baseIndex);
	baseIndex += static_cast<uint32_t>(rootSphere.getVertices().size());
	tipSphere.setBaseIndex(baseIndex);

	vertices.insert(vertices.end(),
		rootSphere.getVertices().begin(),
		rootSphere.getVertices().end()
	);
	vertices.insert(vertices.end(),
		tipSphere.getVertices().begin(),
		tipSphere.getVertices().end()
	);
	indices.insert(indices.end(),
		rootSphere.getIndices().begin(),
		rootSphere.getIndices().end()
	);
	indices.insert(indices.end(),
		tipSphere.getIndices().begin(),
		tipSphere.getIndices().end()
	);
}