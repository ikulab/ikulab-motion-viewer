#include "./bvhExporter.hpp"

#include <cstring>
#include <fstream>

#include "./bvhParser.hpp"

#define INDENT ("  ")

// forward declearation of helper functions --------------------
void writeJointsRecursive(std::vector<std::shared_ptr<Animator::Joint>> joints,
                          ikura::GroupID currentJointID,
                          std::shared_ptr<Motion> motion,
                          std::ofstream &targetFile, uint32_t currentLevel);
void writeIndents(std::ofstream &targetFile, uint32_t level);
/// This function inserts pre-space if jointMotion owns channel.
/// e.g. writes " Xposition" to targetFile.
void writeChannelStrIfOwned(std::ofstream &targetFile,
                            std::shared_ptr<JointMotion> jointMotion,
                            ChannelEnum channel);

void exportLoopRangeToBvhFile(const Animator &animator,
                              std::filesystem::path destFile) {

    // vector<vector<Motion>>
    auto motion = animator.getMotion();
    std::ofstream targetFile(destFile);

    auto joints = animator.getJoints();

    targetFile << TOKEN_TOP << std::endl;
    writeJointsRecursive(joints, 0, motion, targetFile, 0);
}

void writeJointsRecursive(std::vector<std::shared_ptr<Animator::Joint>> joints,
                          ikura::GroupID currentJointID,
                          std::shared_ptr<Motion> motion,
                          std::ofstream &targetFile, uint32_t currentLevel) {

    auto currentJoint = joints[currentJointID];
    auto jointMotion = motion->jointMotions[currentJointID];

    // Joint Name
    std::string jointDeclaration;
    if (currentJointID == 0) {
        jointDeclaration = TOKEN_ROOT;
    } else if (currentJoint->getIsEdge()) {
        jointDeclaration = TOKEN_END_SITE;
    } else {
        jointDeclaration = TOKEN_JOINT;
    }
    writeIndents(targetFile, currentLevel);
    targetFile << jointDeclaration << " " << currentJoint->getName()
               << std::endl;

    // Beggin '{'
    writeIndents(targetFile, currentLevel);
    targetFile << TOKEN_BEGGIN_BRACKET << std::endl;

    currentLevel += 1;

    // Offset
    auto pos = currentJoint->getPos();
    writeIndents(targetFile, currentLevel);
    targetFile << TOKEN_OFFSET << " ";
    targetFile << pos.x << " " << pos.y << " " << pos.z << std::endl;

    // Channels (non-EndSize only)
    writeIndents(targetFile, currentLevel);
    targetFile << TOKEN_CHANNELS << " ";
    targetFile << jointMotion->ownedChannels.size();
    // write "?position" if current joint owns these channels
    writeChannelStrIfOwned(targetFile, jointMotion, ChannelEnum::Xposition);
    writeChannelStrIfOwned(targetFile, jointMotion, ChannelEnum::Yposition);
    writeChannelStrIfOwned(targetFile, jointMotion, ChannelEnum::Zposition);
    // write "?rotation" in rotation order
    for (const auto &r : motion->rotationOrder) {
        writeChannelStrIfOwned(targetFile, jointMotion,
                               convertRotationAxisEnumToChannelEnum(r));
    }
    targetFile << std::endl;

    // Child Joint (non-EndSite only)
    auto closestChildIDs = currentJoint->getClosestChildIDs();
    for (const auto &id : closestChildIDs) {
        auto childID = joints[id]->getID();
        writeJointsRecursive(joints, childID, motion, targetFile, currentLevel);
    }

    writeIndents(targetFile, currentLevel - 1);
    // End '}'
    targetFile << TOKEN_END_BRACKET << std::endl;
}

void writeIndents(std::ofstream &targetFile, uint32_t level) {
    for (uint32_t i = 0; i < level; i++) {
        targetFile << INDENT;
    }
}

void writeChannelStrIfOwned(std::ofstream &targetFile,
                            std::shared_ptr<JointMotion> jointMotion,
                            ChannelEnum channel) {

    if (jointMotion->ownedChannels.find(channel) !=
        jointMotion->ownedChannels.end()) {

        targetFile << " ";
        targetFile << convertChannelEnumToStr(channel);
    }
}
