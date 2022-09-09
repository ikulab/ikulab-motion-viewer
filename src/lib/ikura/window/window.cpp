#include "./window.hpp"

#include <easylogging++.h>

#include "../renderComponent/defaultSetting.hpp"

namespace ikura {
void Window::addDefaultRenderTarget() {}
void addDefaultRenderContent() {}

const int Window::getNumOfFrames() const { return numOfFrames; }

const vk::DescriptorSetLayout Window::getDescriptorSetLayout() const {
    return descriptorSetLayout;
}

void Window::createDefaultDescriptorSetLayout() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Creating default DescriptorSetLayout...";

    vk::DescriptorSetLayoutBinding modelMatLayoutBinding{};
    modelMatLayoutBinding.binding = DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO;
    modelMatLayoutBinding.descriptorCount = 1;
    modelMatLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    modelMatLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

    vk::DescriptorSetLayoutBinding sceneMatLayoutBinding{};
    sceneMatLayoutBinding.binding = DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO;
    sceneMatLayoutBinding.descriptorCount = 1;
    sceneMatLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    sceneMatLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

    std::array<vk::DescriptorSetLayoutBinding, NUM_OF_DESCRIPTOR_SETS>
        layoutBindings = {modelMatLayoutBinding, sceneMatLayoutBinding};
    vk::DescriptorSetLayoutCreateInfo layoutCI{};
    layoutCI.bindingCount = layoutBindings.size();
    layoutCI.pBindings = layoutBindings.data();

    descriptorSetLayout =
        renderEngine->getDevice().createDescriptorSetLayout(layoutCI);

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Default DescriptorSetLayout has been created.";
}

Window::~Window() {
    renderEngine->getDevice().destroyDescriptorSetLayout(descriptorSetLayout);
}
} // namespace ikura