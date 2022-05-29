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
	std::vector<std::vector<std::unique_ptr<Motion>>> motions;
	uint32_t numOfFrames;
	float frameRate;
	float loopDuration;

public:
	class Joint {
		JointID id;
		std::string name;
		glm::vec3 pos;
		std::vector<JointID> parentIDs;
		bool isEdge;

		static JointID currentId;
	public:
		Joint(std::string name, JointID id, glm::vec3 pos, std::vector<JointID> parentIDs, bool isEdge)
			: name(name), id(id), pos(pos), parentIDs(parentIDs), isEdge(isEdge) {}

		JointID getID() const;
		std::vector<JointID> getParentIDs() const;
		bool getIsEdge() const;

		void showInfo();
	};

	void initFromBVH(std::string filePath);
	std::array<glm::mat4, MAX_ID> generateModelMatrices(float time); // TODO: implement

	void showSkeltonInfo();
	void showMotionInfo();

private:
	std::vector<std::unique_ptr<Animator::Joint>> joints;
};