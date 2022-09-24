#pragma once

#include "../renderContent.hpp"

namespace ikura {
class BasicRenderContent : public RenderContent {
    void setupUniformBuffers();
    void setupDescriptorSets();
    void setupVertexAndIndexArray();

  public:
    BasicRenderContent(std::shared_ptr<RenderEngine> renderEngine,
                       vk::DescriptorSetLayout descriptorSetLayout,
                       int numOfFrames);
};
} // namespace ikura