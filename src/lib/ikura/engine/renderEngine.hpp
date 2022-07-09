#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.hpp>

#include "../renderComponent/renderContent.hpp"
#include "../renderComponent/renderTarget.hpp"

struct RenderEngineInitConfig {
	const char* applicationName;
	uint32_t applicationVersion;

	bool enableValidationLayers;
	std::vector<std::string> validationLayerNames;
	// TODO: surface, debugUtilが無い場合にも対応させる
	std::vector<std::string> extensionNames;

	static RenderEngineInitConfig defaultDebugSetting() {
		RenderEngineInitConfig initConfig = defaultCommonSetting();

		initConfig.enableValidationLayers = true;
		initConfig.validationLayerNames.push_back("VK_LAYER_KHRONOS_validation");

		initConfig.extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		initConfig.extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

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
	bool enableValidationLayers;

	std::vector<std::string> validationLayerNames;
	std::vector<std::string> extensionNames;
public:
	RenderEngine(RenderEngineInitConfig initConfig);

	void draw(RenderContent content, RenderTarget target);
};