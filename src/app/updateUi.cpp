#include "./app.hpp"

#include <cmath>

#define MAIN_CONTROL_BUTTON_SIZE_UNIT 40
#define MAX_ANIMATION_SPEED 10.0f
#define MIN_ANIMATION_SPEED (1.0f / 128.0f)

void App::updateUI() {
    imGuiVirtualWindow->setCurrentImGuiContext();
    imGuiVirtualWindow->newFrame();

    updateMainMenu();

    if (ui.animationControlWindow.show) {
        updateAnimationControlWindow();
    }
    if (ui.debugWindow.show) {
        updateDebugWindow();
    }
    if (ui.showImGuiDemoWindow) {
        ImGui::ShowDemoWindow();
    }

    ImGui::Render();
}

void App::updateMainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("ファイル")) {
            if (ImGui::MenuItem("BVHファイルを開く")) {
                selectFileAndInitShapes();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("ウィンドウ")) {
            ImGui::Checkbox("デバッグウィンドウ", &ui.debugWindow.show);
            ImGui::Checkbox("アニメーションコントロールウィンドウ",
                            &ui.animationControlWindow.show);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("表示")) {
            ImGui::Checkbox("軸オブジェクト", &ui.showAxisObject);
            ImGui::Checkbox("床", &ui.showFloor);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void App::updateAnimationControlWindow() {
    if (!ui.animationControlWindow.sizeInitialized) {
        ImGui::SetNextWindowSize(ImVec2(800, 200));
        ImGui::SetNextWindowPos(ImVec2((mainWindow->getWidth() - 800) / 2,
                                       mainWindow->getHeight() - 300));
        ui.animationControlWindow.sizeInitialized = true;
    }

    ImGui::Begin("アニメーションコントロール");

    auto total = animator.getNumOfFrames();
    auto current = animator.getCurrentFrame();

    if (!modelLoaded) {
        ImGui::BeginDisabled();
    }

    // Seek bar ----------
    if (modelLoaded) {
        ImGui::Text("Frame: %d / %d", current + 1, total);
    } else {
        ImGui::Text("Frame: -- / --");
    }

    ImGui::PushItemWidth(-1);
    if (modelLoaded) {
        int seekBarValue = current + 1;
        int oldSeekBarValue = seekBarValue;

        ImGui::SliderInt("##seek_bar", &seekBarValue, 1,
                         animator.getNumOfFrames());
        if (oldSeekBarValue != seekBarValue) {
            animationTime = (seekBarValue - 1) * animator.getFrameRate();
        }
    } else {
        int unused = 0;
        ImGui::SliderInt("##seek_bar", &unused, 0, animator.getNumOfFrames());
    }

    // Main control ----------
    // Align
    float space = ImGui::GetStyle().ItemSpacing.x;
    float width = MAIN_CONTROL_BUTTON_SIZE_UNIT * 8 + space * 6;
    float avail = ImGui::GetWindowContentRegionWidth();
    float pos = (avail - width) / 2 + ImGui::GetCursorPosX();
    ImGui::SetCursorPosX(pos);

    // Jump to begin
    if (ImGui::Button("<<##jump_to_begin",
                      ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                             MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animationTime = 0;
    }
    ImGui::SameLine();

    // Prev frame
    if (ImGui::Button("-5##prev_5", ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                                           MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animationTime -= animator.getFrameRate() * 5;
    }
    ImGui::SameLine();

    if (ImGui::Button("-1##prev_1", ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                                           MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animationTime -= animator.getFrameRate();
    }
    ImGui::SameLine();

    // Play button
    const char *playButtonLabel = stopAnimation ? "Play" : "Stop";
    if (ImGui::Button(playButtonLabel, ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT * 2,
                                              MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        stopAnimation = !stopAnimation;
    }
    ImGui::SameLine();

    // Next frame
    if (ImGui::Button("+1##next_1", ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                                           MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animationTime += animator.getFrameRate();
    }
    ImGui::SameLine();

    if (ImGui::Button("+5##next_5", ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                                           MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animationTime += animator.getFrameRate() * 5;
    }
    ImGui::SameLine();

    // Jump to end
    if (ImGui::Button(">>##jump_to_end",
                      ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                             MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animationTime =
            animator.getFrameRate() * (animator.getNumOfFrames() - 1);
    }

    // Speed Control ----------
    ImGui::Text("Speed");

    ImGui::PushItemWidth(20);
    if (ImGui::Button("<<##speed_down")) {
        animationSpeed /= 2;
        animationSpeed = std::max({animationSpeed, MIN_ANIMATION_SPEED});
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("##speed_drag", &animationSpeed, 0.1, MIN_ANIMATION_SPEED,
                     MAX_ANIMATION_SPEED);

    ImGui::PopItemWidth();

    ImGui::SameLine();
    if (ImGui::Button(">>##speed_up")) {
        animationSpeed *= 2;
        animationSpeed = std::min({animationSpeed, MAX_ANIMATION_SPEED});
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset##speed_reset")) {
        animationSpeed = 1.0;
    }

    if (!modelLoaded) {
        ImGui::EndDisabled();
    }
    ImGui::End();
}

void App::updateDebugWindow() {
    if (!ui.debugWindow.sizeInitialized) {
        ImGui::SetNextWindowSize(ImVec2(300, 500));
        ui.debugWindow.sizeInitialized = true;
    }

    ImGui::Begin("デバッグ");
    ImGuiIO &io = ImGui::GetIO();

    ImGui::Checkbox("ImGui DemoWindowを表示する##show_imgui_window",
                    &ui.showImGuiDemoWindow);
    ImGui::Checkbox("軸オブジェクトを表示する##show_axis_object",
                    &ui.showAxisObject);
    ImGui::Checkbox("床を表示する##show_floor", &ui.showFloor);

    UI::makePadding(20);

    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Text("Joints: %d", animator.getNumOfJoints());
    ImGui::Text("Animation Time: %f", animationTime);

    UI::makePadding(10);

    // mouse input status
    ImGui::Text("Cursor Pos: (%.1f, %.1f)", mouse.currentX, mouse.currentY);
    ImGui::Text("DragStart: (%.1f, %.1f)", mouse.dragStartX, mouse.dragStartY);
    ImGui::Text("DragEnd: (%.1f, %.1f)", mouse.dragEndX, mouse.dragEndY);
    ImGui::Text("Button L/R/M: (%d / %d / %d)", mouse.leftButton,
                mouse.rightButton, mouse.middleButton);

    UI::makePadding(10);

    // camera status
    ImGui::Text("Camera rotation (degrees) H/V: (%.2f, %.2f)",
                glm::degrees(camera.hRotation), glm::degrees(camera.vRotation));
    ImGui::Text("Camera distance: %.2f", camera.distance);
    glm::vec3 cameraPos = camera.generatePos();
    ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y,
                cameraPos.z);
    ImGui::Text("Camera look-at position: (%.2f, %.2f, %.2f)", camera.center.x,
                camera.center.y, camera.center.z);

    UI::makePadding(10);

    // window status
    ImGui::Text("Window size: (%d, %d)", mainWindow->getWidth(),
                mainWindow->getHeight());
    ImGui::Text("IsWindowFocused: %d", ImGui::IsWindowFocused());

    ImGui::End();
}