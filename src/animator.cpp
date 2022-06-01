#include <iostream>
#include <algorithm>
#include <cmath>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "./animator.hpp"
#include "./util/bvhParser.hpp"
#include "./shape/bone/stickTetrahedronBone.hpp"
#include "./shape/cube/singleColorCube.hpp"

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
	float timeInLoop = std::fmod(time, (loopDuration - frameRate));
	int prevFrameIdx = std::floor(timeInLoop / frameRate);
	int nextFrameIdx = prevFrameIdx + 1;
	float progressBetweenFrames = std::fmod(timeInLoop, frameRate) / frameRate;

	// calculate current motion
	std::vector<Motion> currentMotion;
	for (JointID id = 0; id < joints.size(); id++) {
		glm::vec3 posDiff = (
			(motions[nextFrameIdx][id]->pos - motions[prevFrameIdx][id]->pos)
			* progressBetweenFrames
		);
		glm::vec3 rotDiff = (
			(motions[nextFrameIdx][id]->rot - motions[prevFrameIdx][id]->rot)
			* progressBetweenFrames
		);
		Motion m{};
		m.pos = motions[prevFrameIdx][id]->pos + posDiff;
		m.rot = motions[prevFrameIdx][id]->rot + rotDiff;
		currentMotion.push_back(m);
	}

	// generate result matrices
	std::array<glm::mat4, MAX_ID> result;

	for (JointID id = 0; id < joints.size(); id++) {
		result[id] = glm::mat4(1.0);
		// convert "right-hand Y-up" to "right-hand Z-up"
		result[id] *= glm::rotate(
			glm::mat4(1.0),
			glm::radians(90.0f),
			glm::vec3(1.0, 0.0, 0.0)
		);
		// scaling
		result[id] *= glm::scale(glm::mat4(1.0), glm::vec3(0.01, 0.01, 0.01));

		const std::vector<JointID>& parentIDs = joints[id]->getParentIDs();
		for (size_t parentIdx = 0; parentIdx < parentIDs.size(); parentIdx++) {
			JointID pID = parentIDs[parentIdx];
			// Move to each parent's position
			if (pID == 0) {
				// Motion position
				result[id] *= glm::translate(
					glm::mat4(1.0),
					// currentMotion[pID].pos
					glm::vec3(0.0)
				);
			}
			else {
				// Joint offset
				result[id] *= glm::translate(
					glm::mat4(1.0),
					joints[pID]->getPos()
				);
			}

			// Motion rotation
			result[id] *= glm::rotate(
				glm::mat4(1.0),
				glm::radians(currentMotion[pID].rot.y),
				glm::vec3(0.0, 1.0, 0.0));
			result[id] *= glm::rotate(
				glm::mat4(1.0),
				glm::radians(currentMotion[pID].rot.x),
				glm::vec3(1.0, 0.0, 0.0));
			result[id] *= glm::rotate(
				glm::mat4(1.0),
				glm::radians(currentMotion[pID].rot.z),
				glm::vec3(0.0, 0.0, 1.0));
		}

		// Move to current joint's position
		result[id] *= glm::translate(
			glm::mat4(1.0),
			joints[id]->getPos()
		);

		// rotate current joint object to turn to parent
		// glm::vec3 cVec = joints[id]->getPos();
		// glm::vec3 pVec = joints[joints[id]->getParentIDs().back()]->getPos();
		// cVec = glm::normalize(cVec);
		// pVec = glm::normalize(pVec);
		// float angle = glm::radians(glm::acos(glm::dot(cVec, pVec)));
	}

	return result;
}

std::array<std::unique_ptr<Shape>, MAX_ID> Animator::generateBones() {
	std::array<std::unique_ptr<Shape>, MAX_ID> result;
	for (JointID id = 0; id < joints.size(); id++) {
		// Root Joint
		if (joints[id]->getParentIDs().empty()) {
			// result[i] = std::make_unique<Shape>(i);
			result[id] = std::make_unique<SingleColorCube>(
				2.0, 2.0, 2.0,
				glm::vec3(0.0, 0.0, 0.0),
				glm::vec3(1.0, 0.0, 0.0),
				id
			);
		}
		else {
			float length = glm::length(joints[id]->getPos());
			result[id] = std::make_unique<StickTetrahedronBone>(length, id);
		}
	}

	return std::move(result);
}
