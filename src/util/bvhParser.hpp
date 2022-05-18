#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "../animator.hpp"

#define TOKEN_TOP "HIERARCHY"
#define TOKEN_ROOT "ROOT"
#define TOKEN_OFFSET "OFFSET"
#define TOKEN_CHANNELS "CHANNELS"
#define TOKEN_JOINT "JOINT"
#define TOKEN_END_SITE "End Site"
#define TOKEN_END_SITE_END "End"
#define TOKEN_END_SITE_SITE "Site"
#define TOKEN_MOTION "MOTION"
#define TOKEN_FRAMES "Frames:"
#define TOKEN_FRAME_TIME "Frame Time:"
#define TOKEN_FRAME_TIME_FRAME "Frame"
#define TOKEN_FRAME_TIME_TIME "Time:"
#define TOKEN_BEGGIN_BRACKET "{"
#define TOKEN_END_BRACKET "}"

namespace Channel {
	enum Channel {
		Xposition, Yposition, Zposition,
		Xrotation, Yrotation, Zrotation
	};

	bool isValidChannel(std::string str);

	Channel convertStr2Channel(std::string str);
};

class parse_failed_error : public std::runtime_error {
	std::string errorLine;
	uint errorLineNum;
public:
	parse_failed_error(std::string msg, const std::unique_ptr<std::ifstream>& ist)
		: std::runtime_error("parse failed: " + msg) {

		errorLineNum = 0;
		errorLine = {};

		uint pos = ist->tellg();
		ist->seekg(0);

		std::string line;
		while (std::getline(*ist, line)) {
			if (pos < ist->tellg()) {
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

class BVHParser {
	void parseJoints(bool isJointTokenRead);
	void parseMotion();

	std::vector<std::unique_ptr<Animator::Joint>> skelton;
	std::vector<std::vector<std::unique_ptr<Motion>>> motion;

	std::unique_ptr<std::ifstream> inputStream;
	bool isRootDefined = false;
	JointID currentID = 0;
	std::vector<std::pair<JointID, Channel::Channel>> channels;
	std::vector<JointID> jointIDStack;

	uint numOfFrames;
	float frameRate;
public:
	BVHParser(std::string filePath);

	void parseBVH();
	std::vector<std::unique_ptr<Animator::Joint>> getSkentonData() {
		return std::move(skelton);
	}
	std::vector<std::vector<std::unique_ptr<Motion>>> getMotionData() {
		return std::move(motion);
	}
	uint getNumOfFrames() { return numOfFrames; }
	float getFrameRate() { return frameRate; }
};