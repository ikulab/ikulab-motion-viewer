#include <iostream>
#include <algorithm>

#include "./animator.hpp"
#include "./util/bvhParser.hpp"

void Animator::initFromBVH(std::string filePath) {
	BVHParser parser(filePath);
	parser.parseBVH();
	joints = parser.getSkentonData();
	motion = parser.getMotionData();
	numOfFrames = parser.getNumOfFrames();
	frameRate = parser.getFrameRate();
}

void Animator::Joint::showInfo() {
	std::cout << id << ": " << name << std::ends;
	std::cout << (isEdge ? " (Edge)" : " ") << std::endl;
	std::cout << "\tPosition: ( "
		<< pos.x << ", "
		<< pos.y << ", "
		<< pos.z << " )" << std::endl;
	std::cout << "\tParents: ( " << std::ends;
	std::for_each(parentIDs.begin(), parentIDs.end(), [](JointID id) {
		std::cout << id << ", " << std::ends;
	});
	std::cout << ")" << std::endl;
}

void Animator::showSkeltonInfo() {
	std::for_each(joints.begin(), joints.end(), [](const std::unique_ptr<Joint>& joint) {
		joint->showInfo();
	});
	std::cout << "Number of Joints:" << joints.size() << std::endl;
}

void Animator::showMotionInfo() {
	for (uint frame = 0; frame < numOfFrames; frame++) {
		std::cout << "frame:" << frame << std::endl;
		for (uint joint = 0; joint < joints.size(); joint++) {
			std::cout << "(("
				<< motion[frame][joint]->pos.x << ","
				<< motion[frame][joint]->pos.y << ","
				<< motion[frame][joint]->pos.z << "), ("
				<< motion[frame][joint]->rot.x << ","
				<< motion[frame][joint]->rot.y << ","
				<< motion[frame][joint]->rot.z << ")), "
				<< std::ends;
		}
		std::cout << std::endl;
	}
}