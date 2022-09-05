#pragma once

#include <vector>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "../engine/renderEngine/renderEngine.hpp"

namespace ikura {
	class ImageResource {
		public:
		vk::Image image;
		vk::ImageView view;
		VmaAllocation allocation;
	};

	class RenderTarget {
		vk::CommandBuffer commandBuffer;
		std::vector<vk::PipelineLayout> graphicsPipelineLayouts;
		std::vector<vk::Pipeline> graphicsPipelines;
		vk::RenderPass renderPass;
		vk::Framebuffer frameBuffer;

		ImageResource colorImageResource;
		ImageResource depthImageResource;

		vk::DescriptorSetLayout descriptorSetLayout;
		vk::DescriptorPool descriptorPool;
		vk::AttachmentDescription attachments;

		std::shared_ptr<RenderEngine> renderEngine;
		vk::Format swapChainFormat;

		void createDefaultRenderPass();
		void createDefaultImageResources();

	public:
		RenderTarget(vk::Format swapChainFormat, const std::shared_ptr<RenderEngine> renderEngine);
		~RenderTarget();

		void setDefaultResources();
	};
}