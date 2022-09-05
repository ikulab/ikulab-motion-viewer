#pragma once

#include <imgui/imgui.h>

#include "./virtualWindow.hpp"

namespace ikura {
class ImGuiVirtualWindow : public VirtualWindow {
    ImGuiContext *imGuiContext;
};
} // namespace ikura