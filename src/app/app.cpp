#include "./app.hpp"

App::App() {}

void App::init() {
	// Initialize Ikura
	initIkura();

	// Initialize RenderEngine
	RenderEngineInitConfig renderConfig = RenderEngineInitConfig::defaultDebugSetting();
	renderConfig.applicationName = "IkulabMotionViewer";
	renderConfig.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	renderEngine = std::make_unique<RenderEngine>(renderConfig);

	// Initialize AppEngine
	appEngine = std::make_unique<AppEngine>();

	// Create Window 
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    int xpos, ypos, windowW, windowH;
    glfwGetMonitorWorkarea(primaryMonitor, &xpos, &ypos, &windowW, &windowH);
    auto window = glfwCreateWindow(windowW, windowH, "ikulab 2022", nullptr, nullptr);

	auto mainWindow = std::make_shared<ikura::GlfwNativeWindow>(window, *renderEngine);
	appEngine->addWindow(mainWindow);
}