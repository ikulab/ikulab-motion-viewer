#pragma once

#include <memory>

#include <imgui/imgui.h>

#include "../uiContext.hpp"

#define PADDING(pad) ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (pad))

class UiContext;

class UiWindow {
protected:
	std::shared_ptr<UiContext> uiCtx;
public:
	virtual void drawWindow() const;
	UiWindow(std::shared_ptr<UiContext> uiCtx);
};