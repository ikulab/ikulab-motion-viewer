#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <ikura.hpp>

#include "./common.hpp"

// forward declearation
struct Motion;

class Animator {
    std::vector<std::vector<std::shared_ptr<Motion>>> motions;
    uint32_t numOfFrames;
    uint32_t currentFrame;
    float frameRate;
    float loopDuration;

  public:
    class Joint {
        ikura::GroupID id;
        std::string name;
        glm::vec3 pos;
        std::vector<ikura::GroupID> parentIDs;
        bool isEdge;

        static ikura::GroupID currentID;

      public:
        Joint(std::string name, ikura::GroupID id, glm::vec3 pos,
              std::vector<ikura::GroupID> parentIDs, bool isEdge)
            : name(name), id(id), pos(pos), parentIDs(parentIDs),
              isEdge(isEdge) {}

        ikura::GroupID getID() const;
        glm::vec3 getPos() const;
        std::vector<ikura::GroupID> getParentIDs() const;
        bool getIsEdge() const;

        void showInfo();
    };

    void initFromBVH(std::string filePath);
    void
    generateBones(std::vector<std::shared_ptr<ikura::shapes::Shape>> &bones);
    std::array<glm::mat4, ikura::NUM_OF_MODEL_MATRIX>
    generateModelMatrices(float time);

    uint32_t getNumOfJoints() const;
    uint32_t getNumOfFrames() const;
    uint32_t getCurrentFrame() const;
    float getFrameRate() const;

    void showSkeltonInfo();
    void showMotionInfo();

  private:
    std::vector<std::shared_ptr<Animator::Joint>> joints;
};