#include "./ikura.hpp"

#include <vulkan/vulkan.hpp>
#include <string>

#include <easylogging++.h>
#include <magic_enum.hpp>

void CheckError(VkResult result, std::string failMsg) {
	if (result == VK_SUCCESS) {
		return;
	}

	failMsg += " VkResult: ";
	failMsg += magic_enum::enum_name(result);

	LOG(FATAL) << failMsg;
}