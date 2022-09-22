#include "./window.hpp"

#include <easylogging++.h>

#include "../renderComponent/defaultSetting.hpp"

namespace ikura {
void Window::draw() {}

void Window::createDefaultDescriptorSetLayout() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Creating default DescriptorSetLayout...";

    vk::DescriptorSetLayoutBinding modelMatLayoutBinding{};
    modelMatLayoutBinding.binding =
        shapes::DESCRIPTOR_SET_BINDING_MODEL_MATRIX_UBO;
    modelMatLayoutBinding.descriptorCount = 1;
    modelMatLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    modelMatLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

    vk::DescriptorSetLayoutBinding sceneMatLayoutBinding{};
    sceneMatLayoutBinding.binding =
        shapes::DESCRIPTOR_SET_BINDING_SCENE_MATRIX_UBO;
    sceneMatLayoutBinding.descriptorCount = 1;
    sceneMatLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    sceneMatLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

    std::array<vk::DescriptorSetLayoutBinding, shapes::NUM_OF_DESCRIPTORS>
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
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Desctoying default DescriptorSetLayout...";
    renderEngine->getDevice().destroyDescriptorSetLayout(descriptorSetLayout);
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Default DescriptorSetLayout has been destroyed.";
}

const int Window::getNumOfFrames() const { return numOfFrames; }

const vk::DescriptorSetLayout Window::getDescriptorSetLayout() const {
    return descriptorSetLayout;
}

const int Window::getWidth() const { return width; }

const int Window::getHeight() const { return height; }

const std::shared_ptr<RenderTarget> &Window::getRenderTarget() {
    return renderTarget;
}

const std::shared_ptr<RenderContent> &Window::getRenderContent() {
    return renderContent;
}

} // namespace ikura