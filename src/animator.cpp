#include "./animator.hpp"
#include "./util/bvhParser.hpp"

void Animator::initFromBVH(std::string filePath) {
	nodes = parseBVH(filePath);
}