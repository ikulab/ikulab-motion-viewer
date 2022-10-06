#include "./app.hpp"

void App::updateUI() {
    imGuiVirtualWindow->setCurrentImGuiContext();
    imGuiVirtualWindow->newFrame();

    updateMainMenu();

    if (ui.showAnimationControlWindow) {
        updateAnimationControlWindow();
    }
    if (ui.showDebugWindow) {
        updateDebugWindow();
    }
    if (ui.showDemoWindow) {
        ImGui::ShowDemoWindow();
    }

    ImGui::Render();

    if (!ui.windowSizeInitialized) {
        ui.windowSizeInitialized = true;
    }
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
            if (ImGui::MenuItem("デバッグウィンドウ", NULL,
                                ui.showDebugWindow)) {
                ui.showDebugWindow = !ui.showDebugWindow;
            }
            if (ImGui::MenuItem("アニメーションコントロールウィンドウ", NULL,
                                ui.showAnimationControlWindow)) {
                ui.showAnimationControlWindow = !ui.showAnimationControlWindow;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void App::updateAnimationControlWindow() {
    if (!ui.windowSizeInitialized) {
        ImGui::SetNextWindowSize(ImVec2(800, 150));
        ImGui::SetNextWindowPos(ImVec2((mainWindow->getWidth() - 800) / 2,
                                       mainWindow->getHeight() - 300));
    }

    ImGui::Begin("アニメーションコントロール");

    auto total = animator.getNumOfFrames();
    auto current = animator.getCurrentFrame();

    // Num of Frame ----------
    if (modelLoaded) {
        ImGui::Text("Frame: %d / %d", current, total);
    } else {
        ImGui::Text("Frame: - / -");
    }

    // Percentage and progressbar ----------
    if (modelLoaded) {
        ImGui::Text("Position: %.1f%%", (float)current / total * 100);
    } else {
        ImGui::Text("Position: - %%");
    }
    ImGui::ProgressBar((float)current / total, ImVec2(-1.0, 0.0), "");

    // Speed Control ----------
    if (ImGui::Button("Reset")) {
        animationSpeed = 1.0;
    }

    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x + 5.0);
    ImGui::PushItemWidth(200);
    ImGui::SliderFloat("Speed", &animationSpeed, -2.0, 2.0);
    ImGui::PopItemWidth();

    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x + 5.0);
    ImGui::Checkbox("Stop", &stopAnimation);

    // Play button
    // auto fontGlobalScale = ImGui::GetIO().FontGlobalScale;
    ImGui::Button("");

    ImGui::End();
}

void App::updateDebugWindow() {
    if (!ui.windowSizeInitialized) {
        ImGui::SetNextWindowSize(ImVec2(300, 700));
    }

    ImGui::Begin("デバッグ");
    ImGuiIO &io = ImGui::GetIO();

    ImGui::Checkbox("ImGui DemoWindowを表示する", &ui.showDemoWindow);
    ImGui::Text("IsWindowFocused: %d", ImGui::IsWindowFocused());
    UI::makePadding(20);

    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Text("Joints: %d", animator.getNumOfJoints());
    ImGui::Text("Animation Time: %f", animationTime);

    UI::makePadding(30);

    // mouse input status
    ImGui::Text("Cursor Pos: (%.1f, %.1f)", mouse.currentX, mouse.currentY);
    ImGui::Text("Scroll offset: (%.1f, %.1f)", mouse.scrollOffsetX,
                mouse.scrollOffsetY);
    ImGui::Text("DragStart: (%.1f, %.1f)", mouse.dragStartX, mouse.dragStartY);
    ImGui::Text("DragEnd: (%.1f, %.1f)", mouse.dragEndX, mouse.dragEndY);
    ImGui::Text("Delta: (%.1f, %.1f)", mouse.deltaX, mouse.deltaY);
    ImGui::Text("Button L/R/M: (%d / %d / %d)", mouse.leftButton,
                mouse.rightButton, mouse.middleButton);
    // camera status
    ImGui::Text("Camera rotation (degrees) H/V: (%.2f, %.2f)",
                glm::degrees(camera.hRotation), glm::degrees(camera.vRotation));
    ImGui::Text("Camera distance: %.2f", camera.distance);
    glm::vec3 cameraPos = camera.generatePos();
    ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y,
                cameraPos.z);
    ImGui::Text("Camera look-at position: (%.2f, %.2f, %.2f)", camera.center.x,
                camera.center.y, camera.center.z);
    UI::makePadding(20);

    // window status
    ImGui::Text("Window size: (%d, %d)", mainWindow->getWidth(),
                mainWindow->getHeight());
    UI::makePadding(20);

    ImGui::End();
}