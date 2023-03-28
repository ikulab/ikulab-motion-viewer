#pragma once

#include <array>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include <glm/glm.hpp>

enum ChannelEnum {
    Xposition,
    Yposition,
    Zposition,
    Xrotation,
    Yrotation,
    Zrotation
};

enum RotationAxisEnum { X, Y, Z };

struct JointState {
    glm::vec3 pos = {};
    glm::vec3 rot = {};
};

struct JointMotion {
    std::vector<std::shared_ptr<JointState>> jointStates;
    std::set<ChannelEnum> ownedChannels;
};

struct Motion {
    std::vector<std::shared_ptr<JointMotion>> jointMotions;
    std::array<RotationAxisEnum, 3> rotationOrder;
};

ChannelEnum convertStrToChannelEnum(std::string str);
ChannelEnum convertRotationAxisEnumToChannelEnum(RotationAxisEnum rotation);
std::string convertChannelEnumToStr(ChannelEnum channel);
std::string convertRotationAxisEnumToChannelStr(RotationAxisEnum rotation);
