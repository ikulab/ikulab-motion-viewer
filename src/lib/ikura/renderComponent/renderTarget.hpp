#pragma once

#include <vector>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "../engine/renderEngine/renderEngine.hpp"

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

		std::shared_ptr<RenderEngine> renderEngine;
		vk::Format swapChainFormat;

		void createDefaultRenderPass();

	public:
		RenderTarget(vk::Format swapChainImageFormat, const std::shared_ptr<RenderEngine> renderEngine);
		~RenderTarget();
	};
}