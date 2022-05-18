#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <memory>

#include <glm/glm.hpp>

#include "./bvhParser.hpp"
#include "../animator.hpp"
#include "../definition/animation.hpp"

struct ParseCtx {
	std::ifstream& currentInputStream;
	bool isRootDefined;
	JointID currentID;
	std::vector<std::pair<JointID, Channel::Channel>> channels;
	std::vector<JointID> currentNodeStructure;
	std::vector<std::unique_ptr<Animator::Node>> result;
};

void parseNode(ParseCtx ctx) {
	std::string input, nodeName;
	uint numOfChannels;
	glm::vec3 pos;
	bool isEndSite = false;

	// Root / Joint / End Site definition
	ctx.currentInputStream >> input;
	if (input == TOKEN_ROOT && !ctx.isRootDefined) {
		ctx.isRootDefined = true;
	}
	else if (input == TOKEN_END_SITE && ctx.isRootDefined) {
		isEndSite = true;
	}
	else if (input == TOKEN_ROOT && ctx.isRootDefined) {
		throw parse_failed_error("Multiple Root definition detected.", ctx.currentInputStream);
	}
	else if ((input == TOKEN_JOINT || input == TOKEN_END_SITE) && !ctx.isRootDefined) {
		throw parse_failed_error("Root is not defined.", ctx.currentInputStream);
	}
	else {
		std::string msg;
		msg += "'";
		msg += TOKEN_ROOT;
		msg += "', '";
		msg += TOKEN_JOINT;
		msg += "' or '";
		msg += TOKEN_END_SITE;
		msg += "' are expected.";
		throw parse_failed_error(msg, ctx.currentInputStream);
	}

	// node name
	ctx.currentInputStream >> nodeName;

	// Beggin bracket
	ctx.currentInputStream >> input;
	if (input != TOKEN_BEGGIN_BRACKET) {
		throw parse_failed_error("Beggin bracket is expected.", ctx.currentInputStream);
	}

	// Offset token
	ctx.currentInputStream >> input;
	if (input != TOKEN_OFFSET) {
		std::string msg;
		msg += "'";
		msg += TOKEN_OFFSET;
		msg += "' is expected.";
		throw parse_failed_error(msg, ctx.currentInputStream);
	}

	// offset values
	try {
		ctx.currentInputStream >> pos.x >> pos.y >> pos.z;
	}
	catch (std::exception e) {
		throw parse_failed_error("Invalid Position format. 3 position value are expected.", ctx.currentInputStream);
	}
	if (isEndSite) {
		return;
	}

	// Channels token
	ctx.currentInputStream >> input;
	if (input != TOKEN_CHANNELS) {
		std::string msg;
		msg += "'";
		msg += TOKEN_CHANNELS;
		msg += "' is expected.";
		throw parse_failed_error(msg, ctx.currentInputStream);
	}

	// number of channels
	try {
		ctx.currentInputStream >> numOfChannels;
	}
	catch (std::exception e) {
		std::string msg;
		throw parse_failed_error("Number of channels (int value) is expected.", ctx.currentInputStream);
	}

	// register channels
	for (uint i = 0; i < numOfChannels; i++) {
		ctx.currentInputStream >> input;
		if (!Channel::isValidChannel(input)) {
			std::string msg;
			msg += "Invalid Channel name at #";
			msg += i;
			msg += ".";
			throw parse_failed_error(msg, ctx.currentInputStream);
		}

		ctx.channels.push_back({
			ctx.currentID, Channel::convertStr2Channel(input)
		});
	}

	ctx.currentNodeStructure.push_back(ctx.currentID);

	parseNode(ctx);

	// End bracket
	ctx.currentInputStream >> input;
	if (input != TOKEN_END_BRACKET) {
		throw parse_failed_error("End bracket is expected.", ctx.currentInputStream);
	}

	// create Node
	ctx.currentNodeStructure.pop_back();
	// ctx.result.push_back(std::move(std::make_unique<Animator::Node>(
	// 	nodeName, ctx.currentID, pos, ctx.currentNodeStructure
	// )));

	ctx.currentID++;
}

std::vector<std::unique_ptr<Animator::Node>> parseBVH(std::string filePath) {
	std::ifstream inputStream(filePath);
	if (!inputStream) {
		throw std::runtime_error("failed to open .bvh file.");
	}

	ParseCtx ctx{
		.currentInputStream = inputStream,
		.isRootDefined = false,
		.currentID = 0U,
		.channels = {},
		.currentNodeStructure = {},
		.result = {}
	};
	try {
		// Hierarchy section definition
		std::string input;
		inputStream >> input;
		if (input != TOKEN_TOP) {
			std::string msg;
			msg += "Top of .bvh file should be '";
			msg += TOKEN_TOP;
			msg += "'.";
			throw parse_failed_error(msg, inputStream);
		}

		parseNode(ctx);
	}
	catch (parse_failed_error e) {
		std::cerr << e.what() << std::endl;
		std::cerr << e.where() << std::endl;
		exit(1);
	}

	// return std::move(ctx.result);
	return std::vector<std::unique_ptr<Animator::Node>>();
}