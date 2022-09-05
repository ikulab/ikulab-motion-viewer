#include "./renderTarget.hpp"

#include <array>

#include <easylogging++.h>
#include <vulkan/vulkan.hpp>

// Forward declearation of helper functions ----------
vk::Format findDepthFormat(std::shared_ptr<RenderEngine> renderEngine);
void createImage(const vk::Extent2D imageExtent, const uint32_t mipLevels,
                 const vk::SampleCountFlagBits numSamples,
                 const vk::Format format, const vk::ImageTiling tiling,
                 const vk::ImageUsageFlags usage,
                 const vk::MemoryPropertyFlags properties,
                 VmaAllocator &allocator, vk::Image &image,
                 VmaAllocation &allocation);
void createImageView(const vk::Image image, const vk::Format format,
                     const vk::ImageAspectFlags aspectFlags,
                     const uint32_t mipLevels, vk::ImageView &imageView,
                     const vk::Device device);

namespace ikura {
void ImageResource::release(vk::Device device, VmaAllocator allocator) {
    device.destroyImageView(view);
    vmaDestroyImage(allocator, image, allocation);
}

void RenderTarget::createDefaultRenderPass() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Creating Default RenderPass...";

    // Attachment Descriptions ----------
    vk::AttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainFormat;
    colorAttachment.samples =
        renderEngine->getEngineInfo().limit.maxMsaaSamples;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::AttachmentDescription depthAttachment{};
    depthAttachment.format = findDepthFormat(renderEngine);
    depthAttachment.samples =
        renderEngine->getEngineInfo().limit.maxMsaaSamples;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout =
        vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::AttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = swapChainFormat;
    colorAttachmentResolve.samples = vk::SampleCountFlagBits::e1;
    colorAttachmentResolve.loadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachmentResolve.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachmentResolve.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachmentResolve.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachmentResolve.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachmentResolve.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    // Attachment Refs ----------
    vk::AttachmentReference colorAttachmentRef{
        0, vk::ImageLayout::eColorAttachmentOptimal};
    vk::AttachmentReference depthAttachmentRef{
        1, vk::ImageLayout::eDepthStencilAttachmentOptimal};
    vk::AttachmentReference colorAttachmentResolveRef{
        2, vk::ImageLayout::eColorAttachmentOptimal};

    // Subpass / Dependency ----------
    vk::SubpassDescription subpass{};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.pResolveAttachments = &colorAttachmentResolveRef;

    vk::SubpassDependency dependency{};

    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.srcStageMask =
        (vk::PipelineStageFlagBits::eColorAttachmentOutput |
         vk::PipelineStageFlagBits::eEarlyFragmentTests);
    dependency.srcAccessMask = vk::AccessFlagBits::eNone;

    dependency.dstSubpass = 0;
    dependency.dstStageMask =
        (vk::PipelineStageFlagBits::eColorAttachmentOutput |
         vk::PipelineStageFlagBits::eEarlyFragmentTests);
    dependency.dstAccessMask =
        (vk::AccessFlagBits::eColorAttachmentWrite |
         vk::AccessFlagBits::eDepthStencilAttachmentWrite);

    // RenderPass ----------
    std::array<vk::AttachmentDescription, 3> attachments = {
        colorAttachment, depthAttachment, colorAttachmentResolve};
    vk::RenderPassCreateInfo renderPassCI{};
    renderPassCI.attachmentCount = attachments.size();
    renderPassCI.pAttachments = attachments.data();
    renderPassCI.subpassCount = 1;
    renderPassCI.pSubpasses = &subpass;
    renderPassCI.dependencyCount = 1;
    renderPassCI.pDependencies = &dependency;

    renderPass = renderEngine->getDevice().createRenderPass(renderPassCI);
    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Default RenderPass has been created.";
}

void RenderTarget::createDefaultImageResources() {
    // Color Image
    createImage(swapChainExtent, 1,
                renderEngine->getEngineInfo().limit.maxMsaaSamples,
                swapChainFormat, vk::ImageTiling::eOptimal,
                vk::ImageUsageFlagBits::eTransientAttachment |
                    vk::ImageUsageFlagBits::eColorAttachment,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                *renderEngine->getVmaAllocator(), colorImageResource.image,
                colorImageResource.allocation);
    createImageView(colorImageResource.image, swapChainFormat,
                    vk::ImageAspectFlagBits::eColor, 1, colorImageResource.view,
                    renderEngine->getDevice());

    // Depth Image
    createImage(swapChainExtent, 1,
                renderEngine->getEngineInfo().limit.maxMsaaSamples,
                findDepthFormat(renderEngine), vk::ImageTiling::eOptimal,
                vk::ImageUsageFlagBits::eDepthStencilAttachment,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                *renderEngine->getVmaAllocator(), depthImageResource.image,
                depthImageResource.allocation);
    createImageView(depthImageResource.image, findDepthFormat(renderEngine),
                    vk::ImageAspectFlagBits::eDepth, 1, depthImageResource.view,
                    renderEngine->getDevice());

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Default ImageResources has been created.";
}

void RenderTarget::setDefaultResources() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Creating default RenderTarget resources...";

    createDefaultRenderPass();
    createDefaultImageResources();
}

RenderTarget::RenderTarget(vk::Format swapChainFormat,
                           vk::Extent2D swapChainExtent,
                           const std::shared_ptr<RenderEngine> renderEngine) {
    this->swapChainFormat = swapChainFormat;
    this->swapChainExtent = swapChainExtent;
    this->renderEngine = renderEngine;
}

RenderTarget::~RenderTarget() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Destroying ImageResources...";
    colorImageResource.release(renderEngine->getDevice(),
                               *renderEngine->getVmaAllocator());
    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "ImageResources has been destroyed.";

    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Destroying RenderPass...";
    renderEngine->getDevice().destroyRenderPass(renderPass);
    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "RenderPass has been destroyed.";
}
} // namespace ikura

vk::Format findDepthFormat(std::shared_ptr<RenderEngine> renderEngine) {
    // TODO: Allow changing
    auto tiling = vk::ImageTiling::eOptimal;
    auto feature = vk::FormatFeatureFlagBits::eDepthStencilAttachment;

    const std::array<vk::Format, 3> candidates = {vk::Format::eD32Sfloat,
                                                  vk::Format::eD32SfloatS8Uint,
                                                  vk::Format::eD24UnormS8Uint};

    for (const auto &format : candidates) {
        auto props =
            renderEngine->getPhysicalDevice().getFormatProperties(format);

        if (tiling == vk::ImageTiling::eLinear) {
            if ((props.linearTilingFeatures & feature) == feature) {
                return format;
            }
        } else if (tiling == vk::ImageTiling::eOptimal) {
            if ((props.optimalTilingFeatures & feature) == feature) {
                return format;
            }
        }
    }

    throw std::runtime_error(
        "Failed to find supported depth attachment format.");
}

void createImage(const vk::Extent2D imageExtent, const uint32_t mipLevels,
                 const vk::SampleCountFlagBits numSamples,
                 const vk::Format format, const vk::ImageTiling tiling,
                 const vk::ImageUsageFlags usage,
                 const vk::MemoryPropertyFlags properties,
                 VmaAllocator &allocator, vk::Image &image,
                 VmaAllocation &allocation) {
    vk::ImageCreateInfo imageCI;
    imageCI.imageType = vk::ImageType::e2D;
    imageCI.extent = vk::Extent3D(imageExtent, 1);
    imageCI.mipLevels = mipLevels;
    imageCI.arrayLayers = 1;
    imageCI.format = format;
    imageCI.tiling = tiling;
    imageCI.initialLayout = vk::ImageLayout::eUndefined;
    imageCI.usage = usage;
    imageCI.samples = numSamples;
    imageCI.sharingMode = vk::SharingMode::eExclusive;

    VmaAllocationCreateInfo allocCI;
    allocCI.usage = VMA_MEMORY_USAGE_AUTO;

    auto vkImageCI = (VkImageCreateInfo)imageCI;
    auto vkImage = (VkImage)image;
    vmaCreateImage(allocator, &vkImageCI, &allocCI, &vkImage, &allocation,
                   nullptr);
}

void createImageView(const vk::Image image, const vk::Format format,
                     const vk::ImageAspectFlags aspectFlags,
                     const uint32_t mipLevels, vk::ImageView &imageView,
                     const vk::Device device) {
    vk::ImageViewCreateInfo viewCI;
    viewCI.image = image;
    viewCI.viewType = vk::ImageViewType::e2D;
    viewCI.format = format;

    viewCI.subresourceRange.aspectMask = aspectFlags;
    viewCI.subresourceRange.baseMipLevel = 0;
    viewCI.subresourceRange.levelCount = mipLevels;
    viewCI.subresourceRange.baseArrayLayer = 0;
    viewCI.subresourceRange.layerCount = 1;

    imageView = device.createImageView(viewCI, nullptr);
}
