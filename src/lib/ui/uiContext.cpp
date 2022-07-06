#include "./uiContext.hpp"

UiWindowsManager::UiWindowsManager() {
	uiCtx = std::make_shared<UiContext>();
	uiCtx->windowSizeInitialized = false;
	uiCtx->showDemoWindow = false;
}

void UiWindowsManager::add_window(std::unique_ptr<UiWindow>& upWindow) {
	uiWindows.push_back(std::move(upWindow));
}