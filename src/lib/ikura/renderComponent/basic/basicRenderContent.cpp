#include "./basicRenderContent.hpp"

namespace ikura {
BasicRenderContent::BasicRenderContent(
    std::shared_ptr<RenderEngine> renderEngine,
    vk::DescriptorSetLayout descriptorSetLayout, int numOfFrames)

    : RenderContent(renderEngine, descriptorSetLayout, numOfFrames) {}
} // namespace ikura