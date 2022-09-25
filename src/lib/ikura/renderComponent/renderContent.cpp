#include "./renderContent.hpp"

#include <vulkan/vulkan.hpp>

#include <easylogging++.h>

#include "../window/nativeWindow/nativeWindow.hpp"
#include "./defaultSetting.hpp"

namespace ikura {
// Forward declearation of helper functions ----------
void destroyBufferResourceIfAllocated(BufferResource &bufferResource,
                                      VmaAllocator &allocator);
void uploadViaStagingBuffer(void *srcData, BufferResource &dstBufferResource,
                            vk::BufferUsageFlags dstBufferUsage,
                            vk::DeviceSize bufferSize,
                            std::shared_ptr<RenderEngine> renderEngine);

void BufferResource::release(VmaAllocator allocator) {
    vmaDestroyBuffer(allocator, buffer, alloc);
}

RenderContent::RenderContent(std::shared_ptr<RenderEngine> renderEngine,
                             vk::DescriptorSetLayout descriptorSetLayout,
                             int numOfFrames) {
    this->renderEngine = renderEngine;
    this->descriptorSetLayout = descriptorSetLayout;
    this->numOfFrames = numOfFrames;
}

RenderContent::~RenderContent() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Destroying default DescriptorPool...";
    renderEngine->getDevice().destroyDescriptorPool(descriptorPool);
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Default DescriptorPool has been destroyed.";

    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Destroying default UniformBuffers...";
    for (auto &resources : uniformBufferResources) {
        for (auto &r : resources) {
            r.release(*renderEngine->getVmaAllocator());
        }
    }
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Default UniformBuffers has been destroyed.";

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Destroying VertexBuffer and IndexBuffer...";
    destroyBufferResourceIfAllocated(vertexBufferResource,
                                     *renderEngine->getVmaAllocator());
    destroyBufferResourceIfAllocated(indexBufferResource,
                                     *renderEngine->getVmaAllocator());
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "VertexBuffer and IndexBuffer have been destroyed.";
}

void RenderContent::uploadVertexBuffer() {
    if (vertices.empty()) {
        LOG(INFO) << "Vertex array is empty. Stopping vertexBuffer upload.";
        return;
    }

    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Uploading VertexBuffer...";

    destroyBufferResourceIfAllocated(vertexBufferResource,
                                     *renderEngine->getVmaAllocator());

    // FIXME
    uploadViaStagingBuffer(vertices.data(), vertexBufferResource,
                           vk::BufferUsageFlagBits::eVertexBuffer,
                           vertices[0].getDataSize() * vertices.size(), renderEngine);

    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "VertexBuffer has been uploaded.";
}

void RenderContent::uploadIndexBuffer() {
    if (indices.empty()) {
        LOG(INFO) << "Index array is empty. Stopping indexBuffer upload.";
        return;
    }

    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Uploading IndexBuffer...";

    destroyBufferResourceIfAllocated(indexBufferResource,
                                     *renderEngine->getVmaAllocator());

    uploadViaStagingBuffer(indices.data(), indexBufferResource,
                           vk::BufferUsageFlagBits::eIndexBuffer,
                           sizeof(indices[0]) * indices.size(), renderEngine);

    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "IndexBuffer has been uploaded.";
}

void RenderContent::uploadVertexAndIndexBuffer() {
    uploadVertexBuffer();
    uploadIndexBuffer();
}

const vk::Buffer &RenderContent::getVertexBuffer() const {
    return vertexBufferResource.buffer;
}

const vk::Buffer &RenderContent::getIndexBuffer() const {
    return indexBufferResource.buffer;
}

const std::vector<vk::DescriptorSet> &
RenderContent::getDescriptorSets(int index) {
    return descriptorSets[index];
}

const std::vector<Index> &RenderContent::getIndices() const {
    return indices;
}

const std::vector<Vertex> &RenderContent::getVertices() const {
    return vertices;
}

void destroyBufferResourceIfAllocated(BufferResource &bufferResource,
                                      VmaAllocator &allocator) {
    if (bufferResource.buffer) {
        vmaDestroyBuffer(allocator, bufferResource.buffer,
                         bufferResource.alloc);
    }
}

void uploadViaStagingBuffer(void *srcData, BufferResource &dstBufferResource,
                            vk::BufferUsageFlags dstBufferUsage,
                            vk::DeviceSize bufferSize,
                            std::shared_ptr<RenderEngine> renderEngine) {
    // Staging buffer allocation ----------
    BufferResource stagingBufferResource;
    vk::BufferCreateInfo stagingBufferCI{};
    stagingBufferCI.size = bufferSize;
    stagingBufferCI.usage = vk::BufferUsageFlagBits::eTransferSrc;

    VmaAllocationCreateInfo allocCI{};
    allocCI.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
    allocCI.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                    VMA_ALLOCATION_CREATE_MAPPED_BIT;
    VmaAllocationInfo stagingBufferAllocInfo{};

    VkBuffer vkBuffer;
    auto vkBufferCI = (VkBufferCreateInfo)stagingBufferCI;
    vmaCreateBuffer(*renderEngine->getVmaAllocator(), &vkBufferCI, &allocCI,
                    &vkBuffer, &stagingBufferResource.alloc,
                    &stagingBufferAllocInfo);
    stagingBufferResource.buffer = vkBuffer;

    // Destination buffer allocation ----------
    vk::BufferCreateInfo dstBufferCI{};
    dstBufferCI.size = bufferSize;
    dstBufferCI.usage = dstBufferUsage | vk::BufferUsageFlagBits::eTransferDst;
    dstBufferCI.sharingMode = vk::SharingMode::eExclusive;

    allocCI = {};
    allocCI.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

    vkBufferCI = (VkBufferCreateInfo)dstBufferCI;
    vmaCreateBuffer(*renderEngine->getVmaAllocator(), &vkBufferCI, &allocCI,
                    &vkBuffer, &dstBufferResource.alloc, nullptr);
    dstBufferResource.buffer = vkBuffer;

    // Copy ----------
    // srcData to staging buffer
    memcpy(stagingBufferAllocInfo.pMappedData, srcData, bufferSize);

    // staging buffer to destination buffer
    auto cmdBuffer = renderEngine->beginSingleTimeCommands();

    vk::BufferCopy copy;
    copy.size = bufferSize;
    cmdBuffer.copyBuffer(stagingBufferResource.buffer, dstBufferResource.buffer,
                         copy);

    // submit command
    renderEngine->endSingleTimeCommands(cmdBuffer);

    stagingBufferResource.release(*renderEngine->getVmaAllocator());
}
} // namespace ikura