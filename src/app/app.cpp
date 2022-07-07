#include "./app.hpp"

App::App() {}

void App::init() {
	// Initialize RenderEngine
	RenderEngineInitConfig renderConfig = RenderEngineInitConfig::defaultDebugSetting();
	renderConfig.applicationName = "IkulabMotionViewer";
	renderConfig.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	renderEngine = std::make_unique<RenderEngine>(renderConfig);
}