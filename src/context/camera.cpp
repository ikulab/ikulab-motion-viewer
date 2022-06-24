#include "./camera.hpp"

glm::vec3 Camera::generatePos() {
	glm::vec3 pos;
	pos.x = distance * std::cos(hRotation) * std::cos(vRotation);
	pos.y = distance * std::sin(hRotation) * std::cos(vRotation);
	pos.z = distance * std::sin(vRotation);
	pos += center;
	return pos;
}

glm::mat4 Camera::generateViewMat() {
	return glm::lookAt(
		generatePos(),
		center,
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
}