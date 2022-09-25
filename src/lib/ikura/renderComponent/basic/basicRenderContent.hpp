#pragma once

#include "../renderContent.hpp"

namespace ikura {
// Forward declearation
class Window;

class BasicRenderContent : public RenderContent {
    void setupUniformBuffers();
    void setupDescriptorSets();

  public:
    BasicRenderContent(std::shared_ptr<RenderEngine> renderEngine,
                       vk::DescriptorSetLayout descriptorSetLayout,
                       int numOfFrames);

    void setVertices(std::vector<BasicVertex> vertices);
    void setIndices(std::vector<BasicIndex> indices);

    void updateUniformBuffer(int frameIndex, BasicModelMatUBO &modelMatUBO,
                             BasicSceneMatUBO &sceneMatUBO);

    // Demo ----------
    void setDemoShape();
    void updateDemoUBO(std::shared_ptr<Window> window);
};
} // namespace ikura