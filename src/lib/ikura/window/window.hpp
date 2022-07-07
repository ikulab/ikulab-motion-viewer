#pragma once

#include <vulkan/vulkan.hpp>

#include "../renderComponent/renderContent.hpp"
#include "../renderComponent/renderTarget.hpp"

class Window {
protected:
	Window() {}
	std::vector<RenderContent> renderContents;
	std::vector<RenderTarget> renderTargets;
public:
	const std::vector<RenderContent>& getRenderContent() const;
	const std::vector<RenderTarget>& getRenderTarget() const;
};