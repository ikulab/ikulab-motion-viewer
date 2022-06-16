#include "./filledFloor.hpp"

FilledFloor::FilledFloor(float width, float height, glm::vec3 color, JointID id)
	: color(color), Floor(width, height, id) {

	vertices.assign({
		{{-width / 2, -height / 2, 0}, color, id},
		{{-width / 2, height / 2, 0}, color, id},
		{{width / 2, height / 2, 0}, color, id},
		{{width / 2, -height / 2, 0}, color, id}
	});

	indices.assign({
		0, 1, 2, 2, 3, 0,
		3, 2, 1, 1, 0, 3
	});
}