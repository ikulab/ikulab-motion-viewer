#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.hpp>

#include "../renderComponent/renderContent.hpp"
#include "../renderComponent/renderTarget.hpp"

struct RenderEngineInitConfig {
	const char* applicationName;
	uint32_t applicationVersion;

	bool enableValidationLayer;
	std::vector<std::string> validationLayerNames;

	static RenderEngineInitConfig defaultDebugSetting() {
		RenderEngineInitConfig initConfig = defaultCommonSetting();
		initConfig.enableValidationLayer = true;
		initConfig.validationLayerNames.push_back("VK_LAYER_KHRONOS_validation");

		return initConfig;
	}

private:
	static RenderEngineInitConfig defaultCommonSetting() {
		RenderEngineInitConfig initConfig;
		initConfig.applicationName = "Ikura Application";
		initConfig.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

		return initConfig;
	}
};

struct RenderEngineSupportInfo {
	bool isGlfwSupported;
};


class RenderEngine {
	VkDevice device;
	VkInstance instance;

	RenderEngineSupportInfo supportInfo;
public:
	RenderEngine(RenderEngineInitConfig renderConfig);

	void draw(RenderContent content, RenderTarget target);
};