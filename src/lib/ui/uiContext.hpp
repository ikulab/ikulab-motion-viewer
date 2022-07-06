#pragma once

#include <memory>
#include <vector>

#include "./uiWindow/uiWindow.hpp"

class UiWindow;

class UiContext {
public:
	bool windowSizeInitialized;
	bool showDemoWindow;
};

class UiWindowsManager {
	std::vector<std::unique_ptr<UiWindow>> uiWindows;
	std::shared_ptr<UiContext> uiCtx;
public:
	UiWindowsManager();

	/// passed Window will be moved
	void add_window(std::unique_ptr<UiWindow>& upWindow);
};
