#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <fstream>

#include <glm/glm.hpp>

#include "./definition/common.hpp"

#define TOKEN_TOP "HIERARCHY"
#define TOKEN_ROOT "ROOT"
#define TOKEN_OFFSET "OFFSET"
#define TOKEN_CHANNELS "CHANNEL"
#define TOKEN_JOINT "JOINT"
#define TOKEN_END_SITE "End Site"
#define TOKEN_MOTION "MOTION"
#define TOKEN_FRAMES "Frames:"
#define TOKEN_FRAME_TIME "Frame Time:"
#define TOKEN_BEGGIN_BRACKET "{"
#define TOKEN_END_BRACKET "}"

namespace Channel {
	enum Channel {
		Xposition, Yposition, Zposition,
		Xrotation, Yrotation, Zrotation
	};

	bool isValidChannel(std::string str) {
		return (
			str == "Xposition" || str == "Yposition" || str == "Zposition" ||
			str == "Xrotation" || str == "Yrotation" || str == "Zrotation"
		);
	}

	Channel convertStr2Channel(std::string str) {
		if (str == "Xposition")
			return Xposition;
		else if (str == "Yposition")
			return Yposition;
		else if (str == "Zposition")
			return Zposition;
		else if (str == "Xrotation")
			return Xrotation;
		else if (str == "Yrotation")
			return Yrotation;
		else if (str == "Zrotation")
			return Zrotation;
	}
};

class parse_failed_error : public std::runtime_error {
	std::string errorLine;
	uint errorLineNum;
public:
	parse_failed_error(std::string msg, std::ifstream& ist)
		: std::runtime_error("parse failed: " + msg) {

		errorLineNum = 0;
		errorLine = {};

		uint pos = ist.tellg();
		ist.seekg(0);

		std::string line;
		while (std::getline(ist, line)) {
			if (pos < ist.tellg()) {
				errorLine = line;
				break;
			}
			errorLineNum++;
		}
		errorLineNum++;

		if (errorLine.empty()) {
			errorLine = "<unknown>";
		}
	}

	std::string where() {
		std::string res;
		res += "line: ";
		res += std::to_string(errorLineNum);
		res += "\n";
		res += errorLine;

		return res;
	}
};

class Animator {
	class Node {
		uint32_t id;
		std::string name;
		glm::vec3 pos;
		std::vector<uint32_t> parentIds;

		static uint32_t currentId;
	public:
		Node(std::string name, uint32_t id);
	};

	std::vector<std::unique_ptr<Animator::Node>> nodes;

public:
	void initFromBVH(std::string filePath); // TODO: implement
	std::array<glm::mat4, MAX_ID> generateModelMatrices(); // TODO: implement
};