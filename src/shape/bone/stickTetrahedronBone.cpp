#include <cmath>
#include <memory>
#include <algorithm>

#include <glm/glm.hpp>

#include "./stickTetrahedronBone.hpp"
#include "../cube/singleColorCube.hpp"

StickTetrahedronBone::StickTetrahedronBone(float length, JointID id) : Bone(length, id) {
	cube = std::make_unique<SingleColorCube>(
		length, 1.0, 1.0,
		glm::vec3(length / 2.0f, 0.0, 0.0),
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