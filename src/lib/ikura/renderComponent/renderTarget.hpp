#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../engine/renderEngine/renderEngine.hpp"

namespace ikura {
class ImageResource {
  public:
    vk::Image image;
    vk::ImageView view;
    VmaAllocation allocation;

    void release(vk::Device device, VmaAllocator allocator);
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
    vk::Extent2D swapChainExtent;

    void createDefaultRenderPass();
    void createDefaultImageResources();

  public:
    RenderTarget(vk::Format swapChainFormat, vk::Extent2D swapChainExtent,
                 const std::shared_ptr<RenderEngine> renderEngine);
    ~RenderTarget();

    void setDefaultResources();
};
} // namespace ikura