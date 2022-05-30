#include <iostream>
#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include "./animator.hpp"
#include "./util/bvhParser.hpp"
#include "./shape/bone/stickTetrahedronBone.hpp"

void Animator::initFromBVH(std::string filePath) {
	BVHParser parser(filePath);
	parser.parseBVH();
	joints = parser.getSkentonData();
	motions = parser.getMotionData();
	numOfFrames = parser.getNumOfFrames();
	frameRate = parser.getFrameRate();
	loopDuration = frameRate * numOfFrames;
}

JointID Animator::Joint::getID() const {
	return id;
}

glm::vec3 Animator::Joint::getPos() const {
	return pos;
}

std::vector<JointID> Animator::Joint::getParentIDs() const {
	return parentIDs;
}

bool Animator::Joint::getIsEdge() const {
	return isEdge;
}

uint32_t Animator::getNumOfJoints() const {
	return joints.size();
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
	for (uint32_t frame = 0; frame < numOfFrames; frame++) {
		std::cout << "frame:" << frame << std::endl;
		for (uint32_t joint = 0; joint < joints.size(); joint++) {
			std::cout << "(("
				<< motions[frame][joint]->pos.x << ","
				<< motions[frame][joint]->pos.y << ","
				<< motions[frame][joint]->pos.z << "), ("
				<< motions[frame][joint]->rot.x << ","
				<< motions[frame][joint]->rot.y << ","
				<< motions[frame][joint]->rot.z << ")), "
				<< std::ends;
		}
		std::cout << std::endl;
	}
}


std::array<glm::mat4, MAX_ID> Animator::generateModelMatrices(float time) {
	float timeInLoop = std::fmod(time, loopDuration);
	int prevFrameIdx = std::floor(timeInLoop / frameRate);
	int nextFrameIdx = prevFrameIdx + 1;
	if (prevFrameIdx >= numOfFrames-1) {
		prevFrameIdx -= 1;
		nextFrameIdx -= 2;
	}
	float progressBetweenFrames = std::fmod(timeInLoop, frameRate) / frameRate;

	// calculate current motion
	std::vector<Motion> currentMotion;
	for (int i = 0; i < joints.size(); i++) {
		glm::vec3 posDiff = (
			(motions[nextFrameIdx][i]->pos - motions[prevFrameIdx][i]->pos)
			* progressBetweenFrames
		);
		glm::vec3 rotDiff = (
			(motions[nextFrameIdx][i]->rot - motions[prevFrameIdx][i]->rot)
			* progressBetweenFrames
		);
		Motion m{};
		m.pos = motions[prevFrameIdx][i]->pos + posDiff;
		m.rot = motions[prevFrameIdx][i]->rot + rotDiff;
		currentMotion.push_back(m);
	}

	// generate result matrices
	std::array<glm::mat4, MAX_ID> result;
	auto multiplyMatrices = [&](glm::mat4& result, JointID id) {
		// Motion position
		// result *= glm::translate(
		// 	glm::mat4(1.0),
		// 	currentMotion[id].pos
		// );

		// Motion rotation
		result *= glm::rotate(
			glm::mat4(1.0),
			currentMotion[id].rot.y,
			glm::vec3(0.0, 1.0, 0.0));
		result *= glm::rotate(
			glm::mat4(1.0),
			currentMotion[id].rot.x,
			glm::vec3(1.0, 0.0, 0.0));
		result *= glm::rotate(
			glm::mat4(1.0),
			currentMotion[id].rot.z,
			glm::vec3(0.0, 0.0, 1.0));
	
		// Joint offset
		result *= glm::translate(
			glm::mat4(1.0),
			joints[id]->getPos()
		);
	};

	for (int jointIdx = 0; jointIdx < joints.size(); jointIdx++) {
		result[jointIdx] = glm::mat4(1.0);
		result[jointIdx] *= glm::scale(glm::mat4(1.0), glm::vec3(0.04, 0.04, 0.04));

		const std::vector<JointID>& parents = joints[jointIdx]->getParentIDs();

		// multiplyMatrices(result[jointIdx], joints[jointIdx]->getID());
		for (int parentIdx = parents.size() - 1; parentIdx >= 0; parentIdx--) {
		// for (int parentIdx = 0; parentIdx < parents.size(); parentIdx++) {
			multiplyMatrices(result[jointIdx], parents[parentIdx]);
		}
	}

	return result;
}

std::array<std::unique_ptr<Shape>, MAX_ID> Animator::generateBones() {
	std::array<std::unique_ptr<Shape>, MAX_ID> result;
	for (int i = 0; i < joints.size(); i++) {
		// Root Joint
		if (joints[i]->getParentIDs().empty()) {
			result[i] = std::make_unique<Shape>(i);
			continue;
		}

		JointID p = joints[i]->getParentIDs().back();

		glm::vec3 childJoint = joints[i]->getPos();
		glm::vec3 parentJoint = joints[p]->getPos();
		float length = std::fabs(glm::distance(childJoint, parentJoint));

		result[i] = std::make_unique<StickTetrahedronBone>(length, i);
	}

	return std::move(result);
}
