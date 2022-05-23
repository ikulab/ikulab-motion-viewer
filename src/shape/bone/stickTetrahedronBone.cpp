#include <cmath>
#include <memory>
#include <algorithm>

#include <glm/glm.hpp>

#include "./stickTetrahedronBone.hpp"

StickTetrahedronBone::StickTetrahedronBone(float length, JointID id) : Bone(length, id) {
	cube = std::make_unique<SingleColorCube>(
		0.02, 0.02, length,
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.8, 0.8, 0.8),
		id
	);
}

const std::vector<Vertex>& StickTetrahedronBone::getVertices() const {
	return cube->getVertices();
}

const std::vector<uint32_t>& StickTetrahedronBone::getIndices() const {
	return cube->getIndices();
}

void StickTetrahedronBone::setBaseIndex(uint32_t baseIndex) {
	cube->setBaseIndex(baseIndex);
}