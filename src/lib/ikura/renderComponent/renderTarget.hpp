#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace ikura {
	class RenderTarget {
		vk::CommandBuffer commandBuffer;
		std::vector<vk::PipelineLayout> graphicsPipelineLayouts;
		std::vector<vk::Pipeline> graphicsPipelines;
		vk::RenderPass renderPass;
		vk::Framebuffer frameBuffer;

		vk::DescriptorSetLayout descriptorSetLayout;
		vk::DescriptorPool descriptorPool;
		vk::AttachmentDescription attachments;

		void createRenderPass();

	public:
		RenderTarget();
	};
}