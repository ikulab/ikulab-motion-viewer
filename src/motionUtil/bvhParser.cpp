#include "./bvhParser.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

// Forward declearation of helper functions ----------
bool isValidChannel(std::string str);
Channel convertStr2Channel(std::string str);

void BVHParser::parseJoints(bool isJointTokenRead) {
    std::string input, jointName;
    uint32_t numOfChannels;
    glm::vec3 pos;
    bool isRoot = false;
    bool isEndSite = false;

    jointIDStack.push_back(currentID);

    auto throwTooManyJointsError = [&] {
        std::string msg;
        msg += "Too many Joints in '";
        msg += filePath;
        msg += "'.\n";
        msg += "The max number of Joints in ikura is: ";
        msg += std::to_string(ikura::NUM_OF_MODEL_MATRIX);
        msg += ".";

        throw std::runtime_error(msg);
    };

    // Root / Joint / End Site definition
    if (!isJointTokenRead) {
        *inputStream >> input;
        bool isInvalidToken = false;
        if (isRootDefined) {
            if (input == TOKEN_ROOT) {
                throw parse_failed_error("Multiple Root definition detected.",
                                         inputStream);
            } else if (input == TOKEN_JOINT) {
            } else if (input == TOKEN_END_SITE_END) {
                *inputStream >> input;
                if (input == TOKEN_END_SITE_SITE) {
                    isEndSite = true;
                    jointName = "<Edge Joint>";
                } else {
                    isInvalidToken = true;
                }
            } else {
                isInvalidToken = true;
            }
        } else {
            if (input == TOKEN_ROOT) {
                isRootDefined = true;
                isRoot = true;
            } else if (input == TOKEN_JOINT || input == TOKEN_END_SITE_END) {
                throw parse_failed_error("Root is not defined.", inputStream);
            } else {
                isInvalidToken = true;
            }
        }

        if (isInvalidToken) {
            std::string msg;
            msg += "'";
            msg += TOKEN_ROOT;
            msg += "', '";
            msg += TOKEN_JOINT;
            msg += "' or '";
            msg += TOKEN_END_SITE;
            msg += "' are expected.";
            throw parse_failed_error(msg, inputStream);
        }
    }

    // joint name
    if (!isEndSite) {
        *inputStream >> jointName;
        if (isRoot) {
            jointName += " (Root)";
        }
    }

    // Beggin bracket
    *inputStream >> input;
    if (input != TOKEN_BEGGIN_BRACKET) {
        throw parse_failed_error("Beggin bracket is expected.", inputStream);
    }

    // Offset token
    *inputStream >> input;
    if (input != TOKEN_OFFSET) {
        std::string msg;
        msg += "'";
        msg += TOKEN_OFFSET;
        msg += "' is expected.";
        throw parse_failed_error(msg, inputStream);
    }

    // offset values
    try {
        *inputStream >> pos.x >> pos.y >> pos.z;
    } catch (std::exception e) {
        throw parse_failed_error(
            "Invalid Position format. 3 position value are expected.",
            inputStream);
    }

    if (!isEndSite) {
        // Channels token
        *inputStream >> input;
        if (input != TOKEN_CHANNELS) {
            std::string msg;
            msg += "'";
            msg += TOKEN_CHANNELS;
            msg += "' is expected.";
            throw parse_failed_error(msg, inputStream);
        }

        // number of channels
        try {
            *inputStream >> numOfChannels;
        } catch (std::exception e) {
            std::string msg;
            throw parse_failed_error(
                "Number of channels (int value) is expected.", inputStream);
        }

        // register channels
        for (uint32_t i = 0; i < numOfChannels; i++) {
            *inputStream >> input;
            if (!isValidChannel(input)) {
                std::string msg;
                msg += "Invalid Channel name at #";
                msg += i;
                msg += ".";
                throw parse_failed_error(msg, inputStream);
            }

            channels.push_back({currentID, convertStr2Channel(input)});
        }

        currentID++;
        if (currentID > ikura::NUM_OF_MODEL_MATRIX) {
            throwTooManyJointsError();
        }
        parseJoints(false);
    }

    // End bracket / Joint token
    while (true) {
        *inputStream >> input;
        if (input == TOKEN_JOINT) {
            currentID++;
            if (currentID > ikura::NUM_OF_MODEL_MATRIX) {
                throwTooManyJointsError();
            }
            parseJoints(true);
        } else if (input == TOKEN_END_BRACKET && !inputStream->eof()) {
            break;
        } else {
            std::string msg;
            msg += "End bracket or '";
            msg += TOKEN_JOINT;
            msg += "' are expected.";
            throw parse_failed_error(msg, inputStream);
        }
    }

    // create Joint
    ikura::GroupID id = jointIDStack.back();
    jointIDStack.pop_back();
    skelton.push_back(std::move(std::make_unique<Animator::Joint>(
        jointName, id, pos, jointIDStack, isEndSite)));
}

void BVHParser::parseMotion() {
    std::string input;

    // Frames: token
    *inputStream >> input;
    if (input != TOKEN_FRAMES) {
        std::string msg;
        msg += "'";
        msg += TOKEN_FRAMES;
        msg += "' is expected";
        throw parse_failed_error(msg, inputStream);
    }

    // frames value
    try {
        *inputStream >> numOfFrames;
    } catch (std::exception e) {
        throw parse_failed_error("Number of frames (int value) is expected.",
                                 inputStream);
    }

    // allocate motion data
    motion.resize(numOfFrames);
    for (size_t frame = 0; frame < numOfFrames; frame++) {
        motion[frame].resize(skelton.size());
        for (size_t joint = 0; joint < skelton.size(); joint++) {
            motion[frame][joint] = std::move(std::make_unique<Motion>());
        }
    }

    // Frame Time: token
    *inputStream >> input;
    if (input != TOKEN_FRAME_TIME_FRAME) {
        std::string msg;
        msg += "'";
        msg += TOKEN_FRAME_TIME;
        msg += "' is expected.";
        throw parse_failed_error(msg, inputStream);
    }
    *inputStream >> input;
    if (input != TOKEN_FRAME_TIME_TIME) {
        std::string msg;
        msg += "'";
        msg += TOKEN_FRAME_TIME;
        msg += "' is expected.";
        throw parse_failed_error(msg, inputStream);
    }

    // frame time value
    try {
        *inputStream >> frameRate;
    } catch (std::exception e) {
        throw parse_failed_error("Frame rate (float value) is expected.",
                                 inputStream);
    }
    // flush new line
    std::getline(*inputStream, input);

    std::stringstream strStream;
    uint32_t numOfChannels = static_cast<uint32_t>(channels.size());
    ikura::GroupID id;
    float value;
    for (uint32_t frame = 0; frame < numOfFrames; frame++) {
        strStream.clear();
        std::getline(*inputStream, input);
        if (frame < numOfFrames - 1 && inputStream->eof()) {
            std::string msg;
            msg += "The number of frames is smaller than the Hierarchy section "
                   "specification (";
            msg += numOfFrames;
            msg += ").";
            throw parse_failed_error(msg, inputStream);
        }

        strStream << input;

        for (uint32_t i = 0; i < numOfChannels; i++) {
            strStream >> value;
            if (i < numOfChannels - 1 && strStream.eof()) {
                std::string msg;
                msg += "The number of channel values is smaller than the "
                       "Hierarchy section specification (";
                msg += numOfChannels;
                msg += ").";
                throw parse_failed_error(msg, inputStream);
            }

            id = channels[i].first;
            switch (channels[i].second) {
            case Channel::Xposition:
                motion[frame][id]->pos.x = value;
                break;
            case Channel::Yposition:
                motion[frame][id]->pos.y = value;
                break;
            case Channel::Zposition:
                motion[frame][id]->pos.z = value;
                break;
            case Channel::Xrotation:
                motion[frame][id]->rot.x = value;
                break;
            case Channel::Yrotation:
                motion[frame][id]->rot.y = value;
                break;
            case Channel::Zrotation:
                motion[frame][id]->rot.z = value;
                break;
            }
        }
    }
}

void BVHParser::parseBVH() {
    try {
        // Hierarchy section definition
        std::string input;
        *inputStream >> input;
        if (input != TOKEN_TOP) {
            std::string msg;
            msg += "Top of .bvh file should be '";
            msg += TOKEN_TOP;
            msg += "'.";
            throw parse_failed_error(msg, inputStream);
        }
        parseJoints(false);

        // sort by ID
        std::sort(skelton.begin(), skelton.end(),
                  [](const std::shared_ptr<Animator::Joint> &a,
                     const std::shared_ptr<Animator::Joint> &b) {
                      return a->getID() < b->getID();
                  });

        // Motion section definition
        *inputStream >> input;
        if (input != TOKEN_MOTION) {
            std::string msg;
            msg += "Start of motion section should be '";
            msg += TOKEN_MOTION;
            msg += "'.";
            throw parse_failed_error(msg, inputStream);
        }
        parseMotion();
    } catch (parse_failed_error e) {
        std::cerr << e.what() << std::endl;
        std::cerr << e.where() << std::endl;
        exit(1);
    }
}

Channel convertStr2Channel(std::string str) {
    if (str == "Xposition")
        return Xposition;
    else if (str == "Yposition")
        return Yposition;
    else if (str == "Zposition")
        return Zposition;
    else if (str == "Xrotation")
        return Xrotation;
    else if (str == "Yrotation")
        return Yrotation;
    else if (str == "Zrotation")
        return Zrotation;

    std::string msg;
    msg += "Invalid channel '";
    msg += str;
    msg += "'.";
    throw std::runtime_error(msg);
}

BVHParser::BVHParser(std::string filePath) {
    this->filePath = filePath;
    inputStream = std::make_unique<std::ifstream>(filePath);

    if (!inputStream->is_open()) {
        std::string msg;
        msg += "failed to open .bvh file '";
        msg += filePath;
        msg += "'.";
        throw std::runtime_error(msg);
    }
}

bool isValidChannel(std::string str) {
    return (str == "Xposition" || str == "Yposition" || str == "Zposition" ||
            str == "Xrotation" || str == "Yrotation" || str == "Zrotation");
}