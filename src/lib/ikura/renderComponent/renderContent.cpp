#include "./renderContent.hpp"

#include <vulkan/vulkan.hpp>

#include "../window/nativeWindow/nativeWindow.hpp"
#include "./defaultSetting.hpp"

namespace ikura {
void RenderContent::createDefaultDescriptorSets() {
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

    // DescriptorSets ----------
    size_t numOfDescriptorWrites =
        nativeWindow.lock()->getNumOfFrames() * NUM_OF_DESCRIPTORS;
    std::vector<vk::WriteDescriptorSet> descriptorWrites;
    descriptorWrites.resize(numOfDescriptorWrites);
    std::vector<vk::DescriptorBufferInfo> bufferInfos;
    bufferInfos.resize(numOfDescriptorWrites);

    size_t index = 0;
    for (size_t frame = 0; frame < nativeWindow.lock()->getNumOfFrames();
         frame++) {
        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = NUM_OF_DESCRIPTOR_SETS;
        allocInfo.pSetLayouts = &nativeWindow.lock()->getDescriptorSetLayout();

        descriptorSets[frame].resize(NUM_OF_DESCRIPTOR_SETS);
        if (vkAllocateDescriptorSets(device, &allocInfo,
                                     descriptorSets[frame].data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets.");
        }

        // Model Matrix UBO
        bufferInfos[index].buffer =
            uniformBuffers[frame][DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO];
        bufferInfos[index].offset = 0;
        bufferInfos[index].range = VK_WHOLE_SIZE;
        descriptorWrites[index] = {};
        descriptorWrites[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[index].dstSet =
            descriptorSets[frame][DESCRIPTOR_SET_INDEX_MODEL_MATRIX_UBO];
        descriptorWrites[index].dstBinding =
            DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO;
        descriptorWrites[index].dstArrayElement = 0;
        descriptorWrites[index].descriptorType =
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[index].descriptorCount = 1;
        descriptorWrites[index].pBufferInfo = &bufferInfos[index];
        index++;

        // Scene Matrix UBO
        bufferInfos[index].buffer =
            uniformBuffers[frame][DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO];
        bufferInfos[index].offset = 0;
        bufferInfos[index].range = VK_WHOLE_SIZE;
        descriptorWrites[index] = {};
        descriptorWrites[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[index].dstSet =
            descriptorSets[frame][DESCRIPTOR_SET_INDEX_SCENE_MATRIX_UBO];
        descriptorWrites[index].dstBinding =
            DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO;
        descriptorWrites[index].dstArrayElement = 0;
        descriptorWrites[index].descriptorType =
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[index].descriptorCount = 1;
        descriptorWrites[index].pBufferInfo = &bufferInfos[index];
        index++;
    }

    vkUpdateDescriptorSets(device, index, descriptorWrites.data(), 0, nullptr);
}

void RenderContent::setDefaultResources() {}

RenderContent::RenderContent(std::shared_ptr<NativeWindow> nativeWindow,
                             std::shared_ptr<RenderEngine> renderEngine) {
    this->nativeWindow = nativeWindow;
    this->renderEngine = renderEngine;
}

RenderContent::~RenderContent() {}
} // namespace ikura