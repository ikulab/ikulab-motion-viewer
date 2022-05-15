#include <iostream>
#include <algorithm>

#include "cube.hpp"


Cube::Cube(float width, float height, float depth, glm::vec3 pos, uint32_t id)
	: width(width), height(height), depth(depth), pos(pos) {

	this->id = id;
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

	vertices.assign({
		// 0-3 Top face - red
		{leftUpTop, {1.0f, 0.0f, 0.0f}, id},
		{leftDownTop, {1.0f, 0.0f, 0.0f}, id},
		{rightDownTop, {1.0f, 0.0f, 0.0f}, id},
		{rightUpTop, {1.0f, 0.0f, 0.0f}, id},
		// 4-7 Bottom face - green
		{leftUpBottom, {0.0f, 1.0f, 0.0f}, id},
		{rightUpBottom, {0.0f, 1.0f, 0.0f}, id},
		{rightDownBottom, {0.0f, 1.0f, 0.0f}, id},
		{leftDownBottom, {0.0f, 1.0f, 0.0f}, id},
		// 8-11 Left face - blue
		{leftUpTop, {0.0f, 0.0f, 1.0f}, id},
		{leftUpBottom, {0.0f, 0.0f, 1.0f}, id},
		{leftDownBottom, {0.0f, 0.0f, 1.0f}, id},
		{leftDownTop, {0.0f, 0.0f, 1.0f}, id},
		// 12-15 Right face - purple
		{rightUpTop, {1.0f, 0.0f, 1.0f}, id},
		{rightDownTop, {1.0f, 0.0f, 1.0f}, id},
		{rightDownBottom, {1.0f, 0.0f, 1.0f}, id},
		{rightUpBottom, {1.0f, 0.0f, 1.0f}, id},
		// 16-19 Front face - cyan
		{leftDownTop, {0.0f, 1.0f, 1.0f}, id},
		{leftDownBottom, {0.0f, 1.0f, 1.0f}, id},
		{rightDownBottom, {0.0f, 1.0f, 1.0f}, id},
		{rightDownTop, {0.0f, 1.0f, 1.0f}, id},
		// 20-23 Back face - yellow
		{rightUpTop, {1.0f, 1.0f, 0.0f}, id},
		{rightUpBottom, {1.0f, 1.0f, 0.0f}, id},
		{leftUpBottom, {1.0f, 1.0f, 0.0f}, id},
		{leftUpTop, {1.0f, 1.0f, 0.0f}, id}
	});

	indices.assign({
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
	});
}

GradationCube::GradationCube(float width, float height, float depth, glm::vec3 pos)
	: width(width), height(height), depth(depth), pos(pos) {

	float xp = width / 2.0f + pos.r;
	float xn = -width / 2.0f + pos.r;
	float yp = height / 2.0f + pos.g;
	float yn = -height / 2.0f + pos.g;
	float zp = depth / 2.0 + pos.b;
	float zn = -depth / 2.0 + pos.b;

	vertices.assign({
		// Top face (Z = 0.5)
		// 0: left top
		{{xn, yn, zp}, {1.0f, 1.0f, 1.0f}},
		// 1: right top
		{{xp, yn, zp}, {1.0f, 1.0f, 1.0f}},
		// 2: right bottom
		{{xp, yp, zp}, {1.0f, 1.0f, 1.0f}},
		// 3: left bottom
		{{xn, yp, zp}, {1.0f, 1.0f, 1.0f}},
		// Bottom face (Z = -0.5)
		// 4: left top
		{{xn, yn, zn}, {1.0f, 1.0f, 1.0f}},
		// 5: right top
		{{xp, yn, zn}, {1.0f, 1.0f, 1.0f}},
		// 6: right bottom
		{{xp, yp, zn}, {1.0f, 1.0f, 1.0f}},
		// 7: left bottom
		{{xn, yp, zn}, {1.0f, 1.0f, 1.0f}}
	});

	indices.assign({
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
	});
}