#pragma once

#include <string>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "../renderComponent/renderContent.hpp"
#include "../renderComponent/renderTarget.hpp"

namespace ikura {
	class Window {
	protected:
		Window() {}

		int width, height;
		std::string name;

		std::vector<std::unique_ptr<RenderContent>> renderContents;
		std::vector<std::unique_ptr<RenderTarget>> renderTargets;
	public:
		virtual ~Window() {}
		virtual void addDefaultRenderTarget();
	};
}