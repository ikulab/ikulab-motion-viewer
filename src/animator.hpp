#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <fstream>

#include <glm/glm.hpp>

#include "./definition/common.hpp"
#include "./definition/animation.hpp"

class Animator {
public:
	class Node {
		uint32_t id;
		std::string name;
		glm::vec3 pos;
		std::vector<JointID> parentIDs;

		static uint32_t currentId;
	public:
		Node(std::string name, JointID id, glm::vec3 pos, std::vector<JointID> parentIDs)
			: name(name), id(id), pos(pos), parentIDs(parentIDs) {}
	};

	void initFromBVH(std::string filePath);
	std::array<glm::mat4, MAX_ID> generateModelMatrices(); // TODO: implement

private:
	std::vector<std::unique_ptr<Animator::Node>> nodes;
};