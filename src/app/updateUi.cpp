#include "./app.hpp"

void App::updateUI() {
    imGuiVirtualWindow->setCurrentImGuiContext();
    imGuiVirtualWindow->newFrame();

    ImGuiIO &io = ImGui::GetIO();

    // ImGui windows
    // Indicator window
    if (!ui.windowSizeInitialized) {
        ui.windowSizeInitialized = true;
#ifndef NODEBUG
        ImGui::SetNextWindowSize(ImVec2(300, 700));
#else
        ImGui::SetNextWindowSize(ImVec2(300, 250));
#endif
    }
    ImGui::Begin("インジケーター");

#ifndef NODEBUG
    ImGui::Checkbox("ImGui DemoWindowを表示する", &ui.showDemoWindow);
    ImGui::Text("IsWindowFocused: %d", ImGui::IsWindowFocused());
    UI::makePadding(20);
#endif

    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Text("Joints: %d", animator.getNumOfJoints());
    ImGui::Text("Animation Time: %f", animationTime);

    auto total = animator.getNumOfFrames();
    auto current = animator.getCurrentFrame();
    ImGui::Text("Frame: %d / %d", current, total);

    ImGui::ProgressBar((float)current / total, ImVec2(0.0, 0.0), "");
    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x + 5.0);
    ImGui::Text("%.1f%%", (float)current / total * 100);

    ImGui::SliderFloat("Speed", &animationSpeed, -2.0, 2.0);
    if (ImGui::Button("Reset")) {
        animationSpeed = 1.0;
    }
    ImGui::Checkbox("Stop", &stopAnimation);

    UI::makePadding(30);

#ifndef NODEBUG
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
#endif

    if (ImGui::Button("ファイルを開く...")) {
        selectFileAndInitShapes();
    }

    ImGui::End();

    // Demo window
    if (ui.showDemoWindow) {
        ImGui::ShowDemoWindow();
    }

    ImGui::Render();
}