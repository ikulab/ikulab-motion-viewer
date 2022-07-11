#include "./renderEngine.hpp"

#include <easylogging++.h>

#include "../../ikura.hpp"


RenderEngine::RenderEngine(RenderEngineInitConfig initConfig) {
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Creating Vulkan Instance...";
	createInstance(initConfig);
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Instance has been created.";

	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Setting up Extensions...";
	setupExtensions(initConfig);
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Extensions have been setup.";
}

RenderEngine::~RenderEngine() {
	destroyExtensions();
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Extensions have been destroyed.";

	vkDestroyInstance(instance, nullptr);
	VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Vulkan Instance has been destroyed.";
}
