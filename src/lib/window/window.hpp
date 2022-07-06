#pragma once

#include <vulkan/vulkan.hpp>

#include "../renderTarget.hpp"
#include "../renderContent.hpp"

class Window {
protected:
	Window() {}
	std::vector<RenderContent> renderContents;
	std::vector<RenderTarget> renderTargets;
public:
	const std::vector<RenderContent>& getRenderContent() const;
	const std::vector<RenderTarget>& getRenderTarget() const;
};