#pragma once

#include <imgui/imgui.h>

#include "./virtualWindow.hpp"

class ImGuiVirtualWindow : public VirtualWindow {
	ImGuiContext *imGuiContext;
};