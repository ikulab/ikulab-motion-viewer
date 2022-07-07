#pragma once

#include <imgui/imgui.h>

#include "./virtualWindow.hpp"

class ImGuiUiWindow : public VirtualWindow {
	ImGuiContext *imGuiContext;
};