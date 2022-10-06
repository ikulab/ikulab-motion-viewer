#pragma once

#include <imgui/imgui.h>

class UI {
  public:
    static void makePadding(int pad);

    bool windowSizeInitialized = false;

    bool showAnimationControlWindow = true;
    bool showDebugWindow = false;
    bool showDemoWindow = false;
};