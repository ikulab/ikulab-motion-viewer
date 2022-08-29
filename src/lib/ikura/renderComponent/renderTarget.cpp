#include "./renderTarget.hpp"

#include <vulkan/vulkan.hpp>

namespace ikura {
	void RenderTarget::createDefaultRenderPass() {
		vk::AttachmentDescription colorAttachment{};
	}

	RenderTarget::RenderTarget() {

	}
}