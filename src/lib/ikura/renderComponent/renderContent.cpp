#include "./renderContent.hpp"

#include <vulkan/vulkan.hpp>

#include <easylogging++.h>

#include "../window/nativeWindow/nativeWindow.hpp"
#include "./defaultSetting.hpp"

// Forward declearation of helper functions ----------
void destroyBufferResourceIfAllocated(ikura::BufferResource &bufferResource,
                                      VmaAllocator &allocator);
void uploadViaStagingBuffer(void *srcData,
                            ikura::BufferResource &dstBufferResource,
                            vk::BufferUsageFlags dstBufferUsage,
                            vk::DeviceSize bufferSize,
                            std::shared_ptr<RenderEngine> renderEngine);

namespace ikura {
void BufferResource::release(VmaAllocator allocator) {
    vmaDestroyBuffer(allocator, buffer, alloc);
}

void RenderContent::createDefaultUniformBuffers() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Creating default UniformBuffers...";

    std::array<vk::DeviceSize, NUM_OF_DESCRIPTORS> uniformBufferSizes;
    uniformBufferSizes[DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO] =
        sizeof(ModelMatUBO);
    uniformBufferSizes[DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO] =
        sizeof(SceneMatUBO);

    uniformBufferResources.resize(nativeWindow.lock()->getNumOfFrames());

    for (size_t frame = 0; frame < nativeWindow.lock()->getNumOfFrames();
         frame++) {
        uniformBufferResources[frame].resize(NUM_OF_DESCRIPTORS);

        for (size_t descSetIndex = 0; descSetIndex < NUM_OF_DESCRIPTORS;
             descSetIndex++) {
            // TODO: create large DeviceMemory and assign part of them to each
            // uniformBuffers
            vk::BufferCreateInfo bufferCI{};
            bufferCI.size = uniformBufferSizes[descSetIndex];
            bufferCI.usage = vk::BufferUsageFlagBits::eUniformBuffer;
            bufferCI.sharingMode = vk::SharingMode::eExclusive;

            VmaAllocationCreateInfo allocCI{};
            allocCI.usage = VMA_MEMORY_USAGE_AUTO;
            allocCI.flags =
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                VMA_ALLOCATION_CREATE_MAPPED_BIT;

            auto vkBufferCI = (VkBufferCreateInfo)bufferCI;
            VkBuffer buffer;

            vmaCreateBuffer(*renderEngine->getVmaAllocator(), &vkBufferCI,
                            &allocCI, &buffer,
                            &uniformBufferResources[frame][descSetIndex].alloc,
                            nullptr);

            uniformBufferResources[frame][descSetIndex].buffer = buffer;
        }
    }

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Default UniformBuffers has been created.";
}

void RenderContent::createDefaultDescriptorSets() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Creating default DescriptorSets...";

    // DescriptorPool ----------
    std::array<vk::DescriptorPoolSize, 2> poolSizes;
    poolSizes[DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO].type =
        vk::DescriptorType::eUniformBuffer;
    poolSizes[DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO].descriptorCount =
        static_cast<uint32_t>(nativeWindow.lock()->getNumOfFrames() *
                              NUM_OF_DESCRIPTORS);
    poolSizes[DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO].type =
        vk::DescriptorType::eUniformBuffer;
    poolSizes[DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO].descriptorCount =
        static_cast<uint32_t>(nativeWindow.lock()->getNumOfFrames());

    vk::DescriptorPoolCreateInfo poolCI{};
    poolCI.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolCI.pPoolSizes = poolSizes.data();
    poolCI.maxSets = static_cast<uint32_t>(
        nativeWindow.lock()->getNumOfFrames() * NUM_OF_DESCRIPTOR_SETS);

    descriptorPool = renderEngine->getDevice().createDescriptorPool(poolCI);

    // DescriptorSets ==========
    // Array preparation ----------
    size_t numOfDescriptorWrites =
        nativeWindow.lock()->getNumOfFrames() * NUM_OF_DESCRIPTORS;

    std::vector<vk::WriteDescriptorSet> descriptorWrites;
    descriptorWrites.resize(numOfDescriptorWrites);
    std::vector<vk::DescriptorBufferInfo> bufferInfos;
    bufferInfos.resize(numOfDescriptorWrites);

    descriptorSets.resize(nativeWindow.lock()->getNumOfFrames());

    // DescriptorSet Allocation loop per frame ----------
    size_t index = 0;
    for (size_t frame = 0; frame < nativeWindow.lock()->getNumOfFrames();
         frame++) {
        // Allocation ----------
        auto layout = nativeWindow.lock()->getDescriptorSetLayout();

        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = NUM_OF_DESCRIPTOR_SETS;
        allocInfo.pSetLayouts = &layout;

        descriptorSets[frame] =
            renderEngine->getDevice().allocateDescriptorSets(allocInfo);

        // Fill Update Info ----------
        // Model Matrix UBO
        bufferInfos[index].buffer =
            uniformBufferResources[frame]
                                  [DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO]
                                      .buffer;
        bufferInfos[index].offset = 0;
        bufferInfos[index].range = VK_WHOLE_SIZE;

        descriptorWrites[index].dstSet =
            descriptorSets[frame][DESCRIPTOR_SET_INDEX_MODEL_MATRIX_UBO];
        descriptorWrites[index].dstBinding =
            DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO;
        descriptorWrites[index].dstArrayElement = 0;
        descriptorWrites[index].descriptorType =
            vk::DescriptorType::eUniformBuffer;
        descriptorWrites[index].descriptorCount = 1;
        descriptorWrites[index].pBufferInfo = &bufferInfos[index];
        index++;

        // Scene Matrix UBO
        bufferInfos[index].buffer =
            uniformBufferResources[frame]
                                  [DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO]
                                      .buffer;
        bufferInfos[index].offset = 0;
        bufferInfos[index].range = VK_WHOLE_SIZE;

        descriptorWrites[index].dstSet =
            descriptorSets[frame][DESCRIPTOR_SET_INDEX_SCENE_MATRIX_UBO];
        descriptorWrites[index].dstBinding =
            DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO;
        descriptorWrites[index].dstArrayElement = 0;
        descriptorWrites[index].descriptorType =
            vk::DescriptorType::eUniformBuffer;
        descriptorWrites[index].descriptorCount = 1;
        descriptorWrites[index].pBufferInfo = &bufferInfos[index];
        index++;
    }

    // Update ----------
    renderEngine->getDevice().updateDescriptorSets(
        index, descriptorWrites.data(), 0, nullptr);

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Default DescriptorSets has been created.";
}

void RenderContent::setDefaultResources() {
    createDefaultUniformBuffers();
    createDefaultDescriptorSets();
}

RenderContent::RenderContent(std::shared_ptr<NativeWindow> nativeWindow,
                             std::shared_ptr<RenderEngine> renderEngine) {
    this->nativeWindow = nativeWindow;
    this->renderEngine = renderEngine;

    vertices.push_back(Vertex());
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
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

void RenderContent::setVertices(const std::vector<Vertex> &vertices) {
    this->vertices = vertices;
}

void RenderContent::setIndices(const std::vector<Index> &indices) {
    this->indices = indices;
}

void RenderContent::uploadVertexBuffer() {
    if (vertices.empty()) {
        LOG(INFO) << "Vertex array is empty. Stopping vertexBuffer upload.";
        return;
    }

    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Uploading VertexBuffer...";

    destroyBufferResourceIfAllocated(vertexBufferResource,
                                     *renderEngine->getVmaAllocator());

    uploadViaStagingBuffer(vertices.data(), vertexBufferResource,
                           vk::BufferUsageFlagBits::eVertexBuffer,
                           sizeof(vertices[0]) * vertices.size(), renderEngine);

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

// this code is supposed to debug.
// updates all frame uniform buffers
void RenderContent::updateUniformBuffer(ModelMatUBO &modelMatUBO,
                                        SceneMatUBO &sceneMatUBO) {
    void *data;
    for (int i = 0; i < nativeWindow.lock()->getNumOfFrames(); i++) {
        // Model Matrix
        vmaMapMemory(
            *renderEngine->getVmaAllocator(),
            uniformBufferResources[i][DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO]
                .alloc,
            &data);
        memcpy(data, &modelMatUBO, sizeof(modelMatUBO));
        vmaUnmapMemory(
            *renderEngine->getVmaAllocator(),
            uniformBufferResources[i][DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO]
                .alloc);

        // Scene Matrix
        vmaMapMemory(
            *renderEngine->getVmaAllocator(),
            uniformBufferResources[i][DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO]
                .alloc,
            &data);
        memcpy(data, &sceneMatUBO, sizeof(sceneMatUBO));
        vmaUnmapMemory(
            *renderEngine->getVmaAllocator(),
            uniformBufferResources[i][DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO]
                .alloc);
    }
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

const std::vector<Index> &RenderContent::getIndices() const { return indices; }

const std::vector<Vertex> &RenderContent::getVertices() const {
    return vertices;
}
} // namespace ikura

void destroyBufferResourceIfAllocated(ikura::BufferResource &bufferResource,
                                      VmaAllocator &allocator) {
    if (bufferResource.buffer) {
        vmaDestroyBuffer(allocator, bufferResource.buffer,
                         bufferResource.alloc);
    }
}

void uploadViaStagingBuffer(void *srcData,
                            ikura::BufferResource &dstBufferResource,
                            vk::BufferUsageFlags dstBufferUsage,
                            vk::DeviceSize bufferSize,
                            std::shared_ptr<RenderEngine> renderEngine) {
    // Staging buffer allocation ----------
    ikura::BufferResource stagingBufferResource;
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