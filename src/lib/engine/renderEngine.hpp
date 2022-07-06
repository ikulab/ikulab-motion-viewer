#pragma once

#include <vulkan/vulkan.hpp>

#include "../renderComponent/renderContent.hpp"
#include "../renderComponent/renderTarget.hpp"

class RenderEngine {
	VkDevice device;
	VkInstance instance;
public:
	void draw(RenderContent content, RenderTarget target);
};