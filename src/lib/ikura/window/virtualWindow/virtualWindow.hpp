#pragma once

#include <memory>

#include "../nativeWindow/nativeWindow.hpp"
#include "../window.hpp"

namespace ikura {
class VirtualWindow : public Window {
  protected:
    std::shared_ptr<RenderEngine> renderEngine;

    VirtualWindow(std::shared_ptr<RenderEngine> renderEngine);

  public:
    ~VirtualWindow();

    virtual void recordCommandBuffer(vk::CommandBuffer);
    virtual bool isFocused() const;
};
} // namespace ikura