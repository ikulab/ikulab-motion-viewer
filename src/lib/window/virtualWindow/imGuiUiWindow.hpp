#pragma once

#include <imgui/imgui.h>

#include "./virtualWindow.hpp"

class UiWindow : public VirtualWindow {
	ImGuiContext *imGuiContext;
};