#pragma once

#include <vector>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "../window.hpp"
#include "../windowInputContext.hpp"
#include "../../engine/renderEngine/renderEngine.hpp"

namespace ikura {
	class NativeWindow : public Window {
	protected:
		std::shared_ptr<RenderEngine> renderEngine;

		WindowInputContext inputCtx;
		vk::SurfaceKHR surface;
		vk::SwapchainKHR swapChain;
		vk::Format swapChainFormat;

		std::vector<vk::Image> swapChainImages;

		NativeWindow() {}
	public:
		virtual ~NativeWindow();
		void addDefaultRenderTarget() override;
	};
}