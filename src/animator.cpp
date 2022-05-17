#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <utility>

#include "animator.hpp"

uint32_t Animator::Node::currentId = 0;

Animator::Node::Node(std::string name, uint32_t id) : name(name), id(id) {}

void Animator::initFromBVH(std::string filePath) {
	std::ifstream file(filePath);
	std::string input;
	if (!file) {
		throw std::runtime_error("failed to open .bvh file.");
	}

	try {
		bool isRootDefined = false;

		uint32_t currentId = 0;
		glm::vec3 pos;
		int numOfChannels;
		std::vector<std::pair<uint32_t, Channel::Channel>> channels;

		// Hierarchy section definition
		file >> input;
		if (input != TOKEN_TOP) {
			std::string msg;
			msg += "Top of .bvh file should be '";
			msg += TOKEN_TOP;
			msg += "'.";
			throw parse_failed_error(msg, file);
		}

		while (true) {
			// Root / Joint definition
			file >> input;
			if (input == TOKEN_ROOT && !isRootDefined) {
				isRootDefined = true;
			}
			else if (input == TOKEN_ROOT && isRootDefined) {
				throw parse_failed_error("Multiple Root definition detected.", file);
			}
			else if (input == TOKEN_JOINT && !isRootDefined) {
				throw parse_failed_error("Root is not defined.", file);
			}
			else {
				std::string msg;
				msg += "'";
				msg += TOKEN_ROOT;
				msg += "' or '";
				msg += TOKEN_JOINT;
				msg += "' are expected.";
				throw parse_failed_error(msg, file);
			}

			// node name
			file >> input;
			nodes.push_back(std::make_unique<Node>(input));

			// Beggin bracket
			file >> input;
			if (input != TOKEN_BEGGIN_BRACKET) {
				throw parse_failed_error("Beggin bracket is expected.", file);
			}

			// Offset token
			file >> input;
			if (input != TOKEN_OFFSET) {
				std::string msg;
				msg += "'";
				msg += TOKEN_OFFSET;
				msg += "' is expected.";
				throw parse_failed_error(msg, file);
			}

			// offset values
			try {
				file >> pos.x >> pos.y >> pos.z;
			}
			catch (std::exception e) {
				throw parse_failed_error("Invalid Position format. 3 position value are expected.", file);
			}

			// Channels token
			file >> input;
			if (input != TOKEN_CHANNELS) {
				std::string msg;
				msg += "'";
				msg += TOKEN_CHANNELS;
				msg += "' is expected.";
				throw parse_failed_error(msg, file);
			}

			// number of channels
			try {
				file >> numOfChannels;
			}
			catch (std::exception e) {
				std::string msg;
				throw parse_failed_error("Number of channels (int value) is expected.", file);
			}

			// register channels
			for (uint i = 0; i < numOfChannels; i++) {
				file >> input;
				if (!Channel::isValidChannel(input)) {
					std::string msg;
					msg += "Invalid Channel name at #";
					msg += i;
					msg += ".";
					throw parse_failed_error(msg, file);
				}

				channels.push_back({
					currentId, Channel::convertStr2Channel(input)
				});
			}
		
			currentId++;
		}
	}
	catch (parse_failed_error e) {
		std::cerr << e.what() << std::endl;
		std::cerr << e.where() << std::endl;
		exit(1);
	}
}