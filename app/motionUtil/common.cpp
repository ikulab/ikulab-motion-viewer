#include "./common.hpp"

ChannelEnum convertStrToChannelEnum(std::string str) {
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

std::string convertChannelEnumToStr(ChannelEnum channel) {
    switch (channel) {
    case ChannelEnum::Xposition:
        return "Xposition";
    case ChannelEnum::Yposition:
        return "Yposition";
    case ChannelEnum::Zposition:
        return "Zposition";
    case ChannelEnum::Xrotation:
        return "Xrotation";
    case ChannelEnum::Yrotation:
        return "Yrotation";
    case ChannelEnum::Zrotation:
        return "Zrotation";
    }

    std::string msg;
    msg += "Cannot convert ChannelEnum to String. ";
    msg += "Unknown ChannelEnum value detected.";
    throw std::runtime_error(msg);
}

ChannelEnum convertRotationAxisEnumToChannelEnum(RotationAxisEnum rotation) {
    switch (rotation) {
    case RotationAxisEnum::X:
        return ChannelEnum::Xrotation;
    case RotationAxisEnum::Y:
        return ChannelEnum::Yrotation;
    case RotationAxisEnum::Z:
        return ChannelEnum::Zrotation;
    }

    std::string msg;
    msg += "Cannot convert RotationAxisEnum to Channel String. ";
    msg += "Unknown RotationAxisEnum value detected.";
    throw std::runtime_error(msg);
}

std::string convertRotationAxisEnumToChannelStr(RotationAxisEnum rotation) {
    switch (rotation) {
    case RotationAxisEnum::X:
        return convertChannelEnumToStr(ChannelEnum::Xrotation);
    case RotationAxisEnum::Y:
        return convertChannelEnumToStr(ChannelEnum::Yrotation);
    case RotationAxisEnum::Z:
        return convertChannelEnumToStr(ChannelEnum::Zrotation);
    }

    std::string msg;
    msg += "Cannot convert RotationAxisEnum to Channel String. ";
    msg += "Unknown RotationAxisEnum value detected.";
    throw std::runtime_error(msg);
}


