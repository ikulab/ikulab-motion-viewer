#include <string>
#include <iostream>
#include <memory>
#include <fstream>

#include "animator.hpp"

uint32_t Animator::Node::currentId = 0;

Animator::Node::Node(std::string name) : name(name) {
	id = currentId;
	currentId++;
}

void Animator::initFromBVH(std::string filePath) {
	std::ifstream file(filePath);
	std::string input;
	if (!file) {
		throw std::runtime_error("failed to open .bvh file.");
	}

	try {
		// Hierarchy section definition
		file >> input;
		if (input != TOKEN_TOP) {
			std::string msg;
			msg += "Top of .bvh file should be '";
			msg += TOKEN_TOP;
			msg += "'";
			throw parse_failed_error(msg, file);
		}

		// Root definition
		file >> input;
		if (input != TOKEN_ROOT) {
			std::string msg;
			msg += "'";
			msg += TOKEN_ROOT;
			msg += "' is expected.";
			throw parse_failed_error(msg, file);
		}

		// Root Node
		file >> input;
		nodes.push_back(std::make_unique<Node>(input));

		// Beggin bracket
		file >> input;
		if (input != TOKEN_BEGGIN_BRACKET) {
			throw parse_failed_error("Beggin bracket is expected.", file);
		}

		// Offset
		file >> input;
		if (input != TOKEN_OFFSET) {
			
		}
	}
	catch (parse_failed_error e) {
		std::cerr << e.what() << std::endl;
		std::cerr << e.where() << std::endl;
		exit(1);
	}
}