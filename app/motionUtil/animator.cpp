#include <algorithm>
#include <cmath>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "./animator.hpp"
#include "./bvhParser.hpp"

// ----------------------------------------
// Animator::Joint
// ----------------------------------------

ikura::GroupID Animator::Joint::getID() const { return id; }

glm::vec3 Animator::Joint::getPos() const { return pos; }

std::vector<ikura::GroupID> Animator::Joint::getParentIDs() const {
    return parentIDs;
}

bool Animator::Joint::getIsEdge() const { return isEdge; }

// ----------------------------------------
// Animator
// ----------------------------------------

void Animator::updateAnimator(float deltaTime) {
    if (animationStopped) {
        return;
    }

    animationTime += deltaTime * animationSpeed;

    float loopStartTime;
    float loopEndTime;
    if (loopEnabled) {
        loopStartTime = loopStartFrameIndex * frameRate;
        loopEndTime = loopEndFrameIndex * frameRate;
        animationTime =
            fmod((animationTime - loopStartTime), loopDurationTime) +
            loopStartTime;
    } else {
        loopStartTime = 0;
        loopEndTime = numOfFrames * frameRate;
        animationTime = fmod(animationTime, numOfFrames * frameRate);
    }
}

void Animator::initFromBVH(std::string filePath) {
    BVHParser parser(filePath);
    parser.parseBVH();
    assert(parser.getSkentonData().size() <= ikura::NUM_OF_MODEL_MATRIX);

    joints = parser.getSkentonData();
    motions = parser.getMotionData();
    numOfFrames = parser.getNumOfFrames();
    frameRate = parser.getFrameRate();

    loopStartFrameIndex = 0;
    loopEndFrameIndex = numOfFrames - 1;
    loopDurationTime = frameRate * numOfFrames;

    animationTime = 0;
    animationSpeed = 1.0;
    animationStopped = false;

    sourceFilePath = filePath;
}

void Animator::generateBones(
    std::vector<std::shared_ptr<ikura::shapes::Shape>> &bones) {
    assert(joints.size() <= ikura::NUM_OF_MODEL_MATRIX);
    bones.clear();
    bones.resize(joints.size());

    uint32_t baseIndex = 0;
    for (ikura::GroupID id = 0; id < joints.size(); id++) {
        if (joints[id]->getParentIDs().empty()) {
            // Root Joint
            bones[id] = std::make_shared<ikura::shapes::SingleColorCube>(
                2.0, 2.0, 2.0, glm::vec3(0.0, 0.0, 0.0),
                glm::vec3(1.0, 0.0, 0.0), id);
        } else {
            float length = glm::length(joints[id]->getPos());
            bones[id] =
                std::make_shared<ikura::shapes::OctahedronBone>(length, id);
        }
        bones[id]->setBaseIndex(baseIndex);
        baseIndex += bones[id]->getVertices().size();
    }
}

std::array<glm::mat4, ikura::NUM_OF_MODEL_MATRIX>
Animator::generateModelMatrices() {
    uint32_t frameIndex = getCurrentFrameIndex();

    // calculate current motion
    std::vector<Motion> currentMotion;
    for (ikura::GroupID id = 0; id < joints.size(); id++) {
        Motion m{};
        m.pos = motions[frameIndex][id]->pos;
        m.rot = motions[frameIndex][id]->rot;
        currentMotion.push_back(m);
    }

    // generate result matrices
    std::array<glm::mat4, ikura::NUM_OF_MODEL_MATRIX> result;

    for (ikura::GroupID id = 0; id < joints.size(); id++) {
        result[id] = glm::mat4(1.0);
        // convert "right-hand Y-up" to "right-hand Z-up"
        result[id] *= glm::rotate(glm::mat4(1.0), glm::radians(90.0f),
                                  glm::vec3(1.0, 0.0, 0.0));

        const std::vector<ikura::GroupID> &parentIDs =
            joints[id]->getParentIDs();

        for (size_t parentIdx = 0; parentIdx < parentIDs.size(); parentIdx++) {
            ikura::GroupID pID = parentIDs[parentIdx];
            // Move to each parent's position
            if (pID == 0) {
                // Motion position
                result[id] *=
                    glm::translate(glm::mat4(1.0), currentMotion[pID].pos
                                   // glm::vec3(0.0)
                    );
            } else {
                // Joint offset
                result[id] *=
                    glm::translate(glm::mat4(1.0), joints[pID]->getPos());
            }

            // Motion rotation
            result[id] *= glm::rotate(glm::mat4(1.0),
                                      glm::radians(currentMotion[pID].rot.y),
                                      glm::vec3(0.0, 1.0, 0.0));
            result[id] *= glm::rotate(glm::mat4(1.0),
                                      glm::radians(currentMotion[pID].rot.x),
                                      glm::vec3(1.0, 0.0, 0.0));
            result[id] *= glm::rotate(glm::mat4(1.0),
                                      glm::radians(currentMotion[pID].rot.z),
                                      glm::vec3(0.0, 0.0, 1.0));
        }

        // Move to current joint's position
        result[id] *=
            glm::translate(glm::mat4(1.0), id != 0 ? joints[id]->getPos()
                                                   : currentMotion[id].pos);

        // rotate current joint object to turn to parent
        glm::vec3 pos = glm::normalize(joints[id]->getPos());
        glm::vec3 orig = glm::vec3(1.0, 0.0, 0.0);
        glm::vec3 cross = glm::normalize(glm::cross(pos, orig));
        if (glm::length(cross) > 0) {
            result[id] *= glm::rotate(
                glm::mat4(1.0),
                glm::pi<float>() - glm::acos(glm::dot(pos, orig)), cross);
        } else if (pos.x > 0) {
            result[id] *= glm::rotate(glm::mat4(1.0), glm::radians(180.0f),
                                      glm::vec3(0.0, 1.0, 0.0));
        }
    }

    return result;
}

uint32_t Animator::getNumOfJoints() const { return joints.size(); }

uint32_t Animator::getNumOfFrames() const { return numOfFrames; }

float Animator::getFrameRate() const { return frameRate; }

uint32_t Animator::getLoopStartFrameIndex() const {
    return loopStartFrameIndex;
}

uint32_t Animator::getLoopEndFrameIndex() const { return loopEndFrameIndex; }

uint32_t Animator::getCurrentFrameIndex() const {
    return std::floor(animationTime / frameRate);
}

float Animator::getAnimationTime() const { return animationTime; }

float Animator::getAnimationSpeed() const { return animationSpeed; }

std::string Animator::getSourceFilePath() { return sourceFilePath; }

bool Animator::isAnimationStopped() const { return animationStopped; }

void Animator::stopAnimation() { animationStopped = true; }

void Animator::resumeAnimation() { animationStopped = false; }

void Animator::setLoopEnabled(bool enabled) { loopEnabled = enabled; }

void Animator::enableLoop() { loopEnabled = true; }

void Animator::disableLoop() { loopEnabled = false; }

void Animator::updateLoopRange(uint32_t _loopStartFrameIndex,
                               uint32_t _loopEndFrameIndex) {

    loopStartFrameIndex = std::clamp(_loopStartFrameIndex, 0U, numOfFrames - 1);
    loopEndFrameIndex = std::clamp(_loopEndFrameIndex, 0U, numOfFrames - 1);

    loopDurationTime = (loopEndFrameIndex - loopStartFrameIndex) * frameRate;

    if (loopEnabled) {
        uint32_t clampedFrameIndex = std::clamp(
            getCurrentFrameIndex(), loopStartFrameIndex, loopEndFrameIndex);
        if (clampedFrameIndex != getCurrentFrameIndex()) {
            animationTime = clampedFrameIndex * frameRate;
        }
    }
}

void Animator::seekAnimation(uint32_t frameIndex) {
    if (loopEnabled) {
        frameIndex =
            std::clamp(frameIndex, loopStartFrameIndex, loopEndFrameIndex);
    }

    animationTime = frameIndex * frameRate;
}

void Animator::incrementFrameIndex(int inc) {
    uint32_t currentFrameIndex = getCurrentFrameIndex();

    // if (currentFrameIndex + inc < 0), but considering unsigned int
    if (inc < 0 && currentFrameIndex < -inc) {
        seekAnimation(0);
    } else {
        uint32_t newFrameIndex =
            std::clamp(currentFrameIndex + inc, 0U, numOfFrames - 1);
        seekAnimation(newFrameIndex);
    }
}

void Animator::setAnimationSpeed(float speed) { animationSpeed = speed; }

void Animator::Joint::showInfo() {
    std::cout << id << ": " << name << std::ends;
    std::cout << (isEdge ? " (Edge)" : " ") << std::endl;
    std::cout << "\tPosition: ( " << pos.x << ", " << pos.y << ", " << pos.z
              << " )" << std::endl;
    std::cout << "\tParents: ( " << std::ends;
    std::for_each(parentIDs.begin(), parentIDs.end(), [](ikura::GroupID id) {
        std::cout << id << ", " << std::ends;
    });
    std::cout << ")" << std::endl;
}

void Animator::showSkeltonInfo() {
    std::for_each(
        joints.begin(), joints.end(),
        [](const std::shared_ptr<Joint> &joint) { joint->showInfo(); });
    std::cout << "Number of Joints:" << joints.size() << std::endl;
}

void Animator::showMotionInfo() {
    for (uint32_t frame = 0; frame < numOfFrames; frame++) {
        std::cout << "frame:" << frame << std::endl;
        for (uint32_t joint = 0; joint < joints.size(); joint++) {
            std::cout << "((" << motions[frame][joint]->pos.x << ","
                      << motions[frame][joint]->pos.y << ","
                      << motions[frame][joint]->pos.z << "), ("
                      << motions[frame][joint]->rot.x << ","
                      << motions[frame][joint]->rot.y << ","
                      << motions[frame][joint]->rot.z << ")), " << std::ends;
        }
        std::cout << std::endl;
    }
}