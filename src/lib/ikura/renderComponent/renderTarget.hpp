#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../engine/renderEngine/renderEngine.hpp"

namespace ikura {
// Forward declearation ----------
class NativeWindow;

class ImageResource {
  public:
    vk::Image image;
    vk::ImageView view;
    std::optional<VmaAllocation> allocation;
    bool releaseImage = true;
    bool releaseImageView = true;

    void release(vk::Device device, VmaAllocator allocator);
};

class RenderTarget {
    // Variables ==========
    std::shared_ptr<RenderEngine> renderEngine;
    std::weak_ptr<NativeWindow> nativeWindow;

    // Basic objects for render ----------
    std::vector<vk::CommandBuffer> renderCmdBuffers;
    vk::PipelineLayout graphicsPipelineLayout;
    vk::Pipeline graphicsPipeline;
    vk::RenderPass renderPass;
    std::vector<vk::Framebuffer> frameBuffers;

    // Sync objects ----------
    std::vector<vk::Semaphore> imageAvailableSemaphores;
    std::vector<vk::Semaphore> renderFinishedSemaphores;
    std::vector<vk::Fence> renderingFence;

    // ImageResource ----------
    ImageResource colorImageResource;
    ImageResource depthImageResource;
    std::vector<ImageResource> renderImageResources;

    // Methods ==========
    // Creation of default resources ----------
    void createDefaultRenderPass();
    void createDefaultImageResources();
    void createDefaultFrameBuffers();
    void createDefaultGraphicsPipeline();

    // Init of default resources ----------
    void initRenderImageResourcesFromNativeWindow();

    // Others ----------
    void createSyncObjects();
    void createRenderCmdBuffers();

  public:
    RenderTarget(const std::shared_ptr<NativeWindow> nativeWindow,
                 const std::shared_ptr<RenderEngine> renderEngine);
    ~RenderTarget();

    void setDefaultResources();

    vk::CommandBuffer &getRenderCommandBuffer(int index);
    vk::Semaphore &getImageAvailableSemaphore(int index);
    vk::Semaphore &getRenderFinishedSemaphore(int index);
    vk::Fence &getRenderingFence(int index);
};
} // namespace ikura