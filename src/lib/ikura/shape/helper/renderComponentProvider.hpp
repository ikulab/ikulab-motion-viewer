#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "../../engine/renderEngine/renderEngine.hpp"
#include "../../renderComponent/renderContent.hpp"
#include "../../renderComponent/renderTarget.hpp"
#include "../../window/nativeWindow/nativeWindow.hpp"

namespace ikura::shapes {
class RenderComponentProvider {
    std::shared_ptr<RenderEngine> renderEngine;
    vk::DescriptorSetLayout descriptorSetLayout;

    void createDescriptorSetlayout();

  public:
    RenderComponentProvider();

    std::shared_ptr<RenderTarget> createRenderTargetForShapes(
        const std::shared_ptr<RenderEngine> renderEngine,
        const std::shared_ptr<NativeWindow> nativeWindow);
    std::shared_ptr<RenderContent> createRenderContentForShapes(
        const std::shared_ptr<RenderEngine> renderEngine,
        const std::shared_ptr<NativeWindow> nativeWindow);
};
} // namespace ikura::shapes