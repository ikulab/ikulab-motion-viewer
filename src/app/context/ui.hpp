#pragma once

#include <imgui/imgui.h>

class UI {
  public:
    static void makePadding(int pad);

    struct AnimationControlWindow {
        bool sizeInitialized = false;
        bool show = true;
    } animationControlWindow;

    struct DebugWindow {
        bool sizeInitialized = false;
        bool show = false;
    } debugWindow;

    bool showImGuiDemoWindow = false;
    bool showFloor = true;
    bool showAxisObject = false;
};