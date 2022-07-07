#include <algorithm>
#include <array>

#include <glm/glm.hpp>

#include "./singleColorCube.hpp"

SingleColorCube::SingleColorCube(float width, float height, float depth, glm::vec3 pos, glm::vec3 color, JointID id)
	: GradationColorCube(width, height, depth, pos, std::array<glm::vec3, 8>(), id), color(color) {

	std::for_each(colors.begin(), colors.end(), [&](glm::vec3& col) {
		col = color;
	});
	init();
}
