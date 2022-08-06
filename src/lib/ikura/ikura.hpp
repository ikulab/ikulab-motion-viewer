#pragma once

#include <vulkan/vulkan.hpp>
#include <map>
#include <string>
#include <optional>

#define IKURA_APP_INFO_ENGINE_NAME "Ikura"
#define IKURA_APP_INFO_ENGINE_VER VK_MAKE_VERSION(1, 0, 0)
#define IKURA_APP_INFO_API_VER VK_API_VERSION_1_2

#define VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

#define VLOG_LV_1 1
#define VLOG_LV_2 2
#define VLOG_LV_3_PROCESS_TRACKING 3
#define VLOG_LV_4_PROCESS_TRACKING_SECONDARY 4
#define VLOG_LV_5 5
#define VLOG_LV_6_ITEM_ENUMERATION 6
#define VLOG_LV_7 7
#define VLOG_LV_8 8
#define VLOG_LV_9 9

void initIkura();
