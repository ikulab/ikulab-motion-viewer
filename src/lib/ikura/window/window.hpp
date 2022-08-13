#pragma once

#include <string>

#include <vulkan/vulkan.hpp>

#include "../renderComponent/renderContent.hpp"
#include "../renderComponent/renderTarget.hpp"

namespace ikura {
	class Window {
	protected:
		Window() {}

		int width, height;
		std::string name;

		std::vector<RenderContent> renderContents;
		std::vector<RenderTarget> renderTargets;
	public:
		const std::vector<RenderContent>& getRenderContent() const;
		const std::vector<RenderTarget>& getRenderTarget() const;
	};
}