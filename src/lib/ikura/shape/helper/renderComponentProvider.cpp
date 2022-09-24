#include "./renderComponentProvider.hpp"

#include <easylogging++.h>

#include "./descriptorSetProps.hpp"

namespace ikura::shapes {

void RenderComponentProvider::createDescriptorSetlayout() {
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

    std::array<vk::DescriptorSetLayoutBinding, NUM_OF_DESCRIPTORS>
        layoutBindings = {modelMatLayoutBinding, sceneMatLayoutBinding};
    vk::DescriptorSetLayoutCreateInfo layoutCI{};
    layoutCI.bindingCount = layoutBindings.size();
    layoutCI.pBindings = layoutBindings.data();

    descriptorSetLayout =
        renderEngine->getDevice().createDescriptorSetLayout(layoutCI);
}

RenderComponentProvider::RenderComponentProvider() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Creating DescriptorSetLayout for shapes.";
    createDescriptorSetlayout();
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "DescriptiorSetLayout for shapes has been created.";
}

std::shared_ptr<RenderTarget>
RenderComponentProvider::createRenderTargetForShapes(
    const std::shared_ptr<RenderEngine> renderEngine,
    const std::shared_ptr<NativeWindow> nativeWindow) {

    auto images = renderEngine->getDevice().getSwapchainImagesKHR(
        nativeWindow->getSwapChain());
    auto renderTarget = std::make_shared<RenderTarget>(
        renderEngine, nativeWindow->getSwapChainFormat(),
        nativeWindow->getSwapChainExtent(), descriptorSetLayout, images,
        nativeWindow->getNumOfFrames());

    return renderTarget;
}

std::shared_ptr<RenderContent>
RenderComponentProvider::createRenderContentForShapes(
    const std::shared_ptr<RenderEngine> renderEngine,
    const std::shared_ptr<NativeWindow> nativeWindow) {

    auto renderContent = std::make_shared<RenderContent>(
        renderEngine, descriptorSetLayout, nativeWindow->getNumOfFrames());
	
	return renderContent;
}

} // namespace ikura::shapes
