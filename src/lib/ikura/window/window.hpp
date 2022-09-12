#pragma once

#include <memory>
#include <string>

#include <vulkan/vulkan.hpp>

#include "../renderComponent/renderContent.hpp"
#include "../renderComponent/renderTarget.hpp"

namespace ikura {
class Window {
  protected:
    Window() {}

    std::shared_ptr<RenderEngine> renderEngine;

    int width, height;
    std::string name;
    int numOfFrames = 3;

    vk::DescriptorSetLayout descriptorSetLayout;

    std::unique_ptr<RenderContent> renderContent;
    std::unique_ptr<RenderTarget> renderTarget;

  public:
    virtual ~Window();
    virtual void createDefaultDescriptorSetLayout();
    virtual void addDefaultRenderTarget();
    virtual void addDefaultRenderContent();
    virtual void draw();

    const int getNumOfFrames() const;
    const vk::DescriptorSetLayout getDescriptorSetLayout() const;
};
} // namespace ikura