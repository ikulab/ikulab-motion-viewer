#include "./renderContent.hpp"

#include <vulkan/vulkan.hpp>

#include <easylogging++.h>

#include "../window/nativeWindow/nativeWindow.hpp"
#include "./defaultSetting.hpp"

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

void RenderContent::createDefaultVertexIndexBuffer() {}

void RenderContent::setDefaultResources() {
    createDefaultUniformBuffers();
    createDefaultDescriptorSets();
}

RenderContent::RenderContent(std::shared_ptr<NativeWindow> nativeWindow,
                             std::shared_ptr<RenderEngine> renderEngine) {
    this->nativeWindow = nativeWindow;
    this->renderEngine = renderEngine;
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
}
} // namespace ikura