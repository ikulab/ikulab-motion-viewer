#include <iostream>
#include <algorithm>

#include "cube.hpp"

std::vector<Vertex> GradationCube::getVertices() {
	float xp = width / 2.0f + pos.r;
	float xn = -width / 2.0f + pos.r;
	float yp = height / 2.0f + pos.g;
	float yn = -height / 2.0f + pos.g;
	float zp = depth / 2.0 + pos.b;
	float zn = -depth / 2.0 + pos.b;

	std::vector<Vertex> result = {
		// Top face (Z = 0.5)
		// 0: left top - blue
		{{xn, yn, zp}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		// 1: right top - red
		{{xp, yn, zp}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		// 2: right bottom - blue
		{{xp, yp, zp}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		// 3: left bottom - green
		{{xn, yp, zp}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		// Bottom face (Z = -0.5)
		// 4: left top - red
		{{xn, yn, zn}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		// 5: right top - green
		{{xp, yn, zn}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		// 6: right bottom - red
		{{xp, yp, zn}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		// 7: left bottom - blue
		{{xn, yp, zn}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
	};
	return result;
}

std::vector<uint32_t> GradationCube::getIndices() {
	std::vector<uint32_t> result = {
		// Top
		0, 1, 2,
		2, 3, 0,
		// Bottom
		4, 7, 6,
		6, 5, 4,
		// Left
		0, 3, 7,
		7, 4, 0,
		// Right
		1, 5, 6,
		6, 2, 1,
		// Front
		2, 6, 7,
		7, 3, 2,
		// Back
		0, 4, 5,
		5, 1, 0
	};
	std::for_each(result.begin(), result.end(), [=](uint32_t& index) {
		index += baseIndex;
	});
	return result;
}

std::vector<Vertex> Cube::getVertices() {
	glm::vec3 leftDownBottom{
		-width / 2.0f + pos.x,
		-height / 2.0f + pos.y,
		-depth / 2.0f + pos.z
	};
	glm::vec3 leftDownTop = {
		-width / 2.0f + pos.x,
		-height / 2.0f + pos.y,
		depth / 2.0f + pos.z
	};
	glm::vec3 leftUpBottom = {
		-width / 2.0f + pos.x,
		height / 2.0f + pos.y,
		-depth / 2.0f + pos.z
	};
	glm::vec3 leftUpTop = {
		-width / 2.0f + pos.x,
		height / 2.0f + pos.y,
		depth / 2.0f + pos.z
	};
	glm::vec3 rightDownBottom = {
		width / 2.0f + pos.x,
		-height / 2.0f + pos.y,
		-depth / 2.0f + pos.z
	};
	glm::vec3 rightDownTop = {
		width / 2.0f + pos.x,
		-height / 2.0f + pos.y,
		depth / 2.0f + pos.z
	};
	glm::vec3 rightUpBottom = {
		width / 2.0f + pos.x,
		height / 2.0f + pos.y,
		-depth / 2.0f + pos.z
	};
	glm::vec3 rightUpTop = {
		width / 2.0f + pos.x,
		height / 2.0f + pos.y,
		depth / 2.0f + pos.z
	};

	std::vector<Vertex> result = {
		// 0-3 Top face - red
		{leftUpTop, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{leftDownTop, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{rightDownTop, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{rightUpTop, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		// 4-7 Bottom face - green
		{leftUpBottom, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{rightUpBottom, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{rightDownBottom, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{leftDownBottom, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		// 8-11 Left face - blue
		{leftUpTop, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{leftUpBottom, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{leftDownBottom, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{leftDownTop, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		// 12-15 Right face - purple
		{rightUpTop, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{rightDownTop, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{rightDownBottom, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{rightUpBottom, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		// 16-19 Front face - cyan
		{leftDownTop, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{leftDownBottom, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{rightDownBottom, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{rightDownTop, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		// 20-23 Back face - yellow
		{rightUpTop, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{rightUpBottom, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{leftUpBottom, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{leftUpTop, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	};
	return result;
}

std::vector<uint32_t> Cube::getIndices() {
	std::vector<uint32_t> result = {
		// Top
		0, 1, 2,
		2, 3, 0,
		// Bottom
		4, 5, 6,
		6, 7, 4,
		// Left but Top
		8, 9, 10,
		10, 11, 8,
		// Right but Bottom
		12, 13, 14,
		14, 15, 12,
		// Front but Left
		16, 17, 18,
		18, 19, 16,
		// Back but Right
		20, 21, 22,
		22, 23, 20
	};
	std::for_each(result.begin(), result.end(), [=](uint32_t& index) {
		index += baseIndex;
	});
	return result;
}