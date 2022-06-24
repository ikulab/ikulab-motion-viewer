#include <iostream>

#include "./indicatorWindow.hpp"
#include "./uiWindow.hpp"

void IndicatorWindow::drawWindow() const {
	if (!uiCtx->windowSizeInitialized) {
#ifndef NODEBUG
        ImGui::SetNextWindowSize(ImVec2(300, 500));
#else
        ImGui::SetNextWindowSize(ImVec2(300, 200));
#endif
    }
    ImGui::Begin("インジケーター");

#ifndef NODEBUG
    ImGui::Checkbox("ImGui DemoWindowを表示する", &(uiCtx->showDemoWindow));
    ImGui::Text("IsWindowFocused: %d", ImGui::IsWindowFocused());
    PADDING(20);
#endif

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("FPS: %.1f", io.Framerate);
    // ImGui::Text("Joints: %d", anim->getNumOfJoints());

    // auto total = anim->getNumOfFrames();
    // auto current = anim->getCurrentFrame();
    // ImGui::Text("Frame: %d / %d", current, total);

    // ImGui::ProgressBar((float)current / total, ImVec2(0.0, 0.0), "");
    // ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x + 5.0);
    // ImGui::Text("%.1f%%", (float)current / total * 100);

    PADDING(30);

#ifndef NODEBUG
    // // mouse input status
    // ImGui::Text("Cursor Pos: (%.1f, %.1f)", mouseCtx.currentX, mouseCtx.currentY);
    // ImGui::Text("Scroll offset: (%.1f, %.1f)", mouseCtx.scrollOffsetX, mouseCtx.scrollOffsetY);
    // ImGui::Text("DragStart: (%.1f, %.1f)", mouseCtx.dragStartX, mouseCtx.dragStartY);
    // ImGui::Text("DragEnd: (%.1f, %.1f)", mouseCtx.dragEndX, mouseCtx.dragEndY);
    // ImGui::Text("Delta: (%.1f, %.1f)", mouseCtx.deltaX, mouseCtx.deltaY);
    // ImGui::Text("Button L/R/M: (%d / %d / %d)", mouseCtx.leftButton, mouseCtx.rightButton, mouseCtx.middleButton);
    // ImGui::Text(
    //     "Camera rotation (degrees) H/V: (%.2f, %.2f)",
    //     glm::degrees(cameraCtx.hRotation),
    //     glm::degrees(cameraCtx.vRotation)
    // );
    // ImGui::Text("Camera distance: %.2f", cameraCtx.distance);
    // glm::vec3 cameraPos = cameraCtx.generatePos();
    // ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
    // PADDING(20);

    // // window status
    // ImGui::Text("Window size: (%d, %d)", windowWidth, windowHeight);
    // PADDING(20);
#endif

    if (ImGui::Button("ファイルを開く...")) {
        std::cout << "TODO: implement!!" << std::endl;
    }
    ImGui::Text("未実装です m(_ _)m");

    ImGui::End();
}