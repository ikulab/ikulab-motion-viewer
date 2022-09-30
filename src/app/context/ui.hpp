#pragma once

#include <imgui/imgui.h>

class UI {
  public:
    static void makePadding(int pad);

    bool windowSizeInitialized = false;
    bool showDemoWindow = false;
};