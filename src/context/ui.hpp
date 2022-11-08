#pragma once

#include <imgui/imgui.h>

class UI {
  public:
    static void makePadding(int pad);

    enum AnimationControlWindowMode {
        Basic, Editor
    };

    struct AnimationControlWindow {
        bool sizeInitialized = false;
        bool positionInitialized = false;
        bool show = true;

        AnimationControlWindowMode mode = Basic;
    } animationControlWindow;

    struct DebugWindow {
        bool sizeInitialized = false;
        bool show = false;
    } debugWindow;

    bool showImGuiDemoWindow = false;
    bool showFloor = true;
    bool showAxisObject = false;
    bool enableVsinc = true;
};