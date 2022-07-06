#include "./uiWindow.hpp"

void UiWindow::drawWindow() const {}

UiWindow::UiWindow(std::shared_ptr<UiContext> uiCtx) : uiCtx(uiCtx) {}
