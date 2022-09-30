#pragma once

#include <imgui/imgui.h>

#include "../nativeWindow/glfwNativeWindow.hpp"
#include "./virtualWindow.hpp"

namespace ikura {
class ImGuiVirtualWindow : public VirtualWindow {
  protected:
    std::shared_ptr<GlfwNativeWindow> nativeWindow;
    ImGuiContext *imGuiContext;

    vk::DescriptorPool imGuiDescriptorPool;

    void initImGuiResources();
    void destroyImGuiResources();

  public:
    ImGuiVirtualWindow(std::shared_ptr<RenderEngine> renderEngine,
                       std::shared_ptr<GlfwNativeWindow> nativeWindow);
    ~ImGuiVirtualWindow();

    void recordCommandBuffer(vk::CommandBuffer cmdBuffer) override;
    bool isFocused() const override;

    void setCurrentImGuiContext() const;
    void newFrame();
};
} // namespace ikura