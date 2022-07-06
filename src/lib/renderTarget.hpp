#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

class RenderTarget {
	VkCommandBuffer commandBuffer;
	std::vector<VkPipelineLayout> graphicsPipelineLayouts;
	std::vector<VkPipeline> graphicsPipelines;
	VkRenderPass renderPass;
	VkFramebuffer frameBuffer;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkAttachmentDescription attachments;
};