#include "./glfwNativeWindow.hpp"

#include <easylogging++.h>

// Forward declearation of helper functions ----------
vk::SurfaceFormatKHR
chooseSwapChainFormat(const std::vector<vk::SurfaceFormatKHR> &formats);
vk::PresentModeKHR
chooseSwapChainPresentMode(const std::vector<vk::PresentModeKHR> &presentModes);
vk::Extent2D
chooseSwapChainExtent(const vk::SurfaceCapabilitiesKHR &capabilities,
                      GLFWwindow *window);

namespace ikura {
void GlfwNativeWindow::framebufferResizeCallback(GLFWwindow *window, int width,
                                                 int height) {
    auto nativeWindow =
        reinterpret_cast<GlfwNativeWindow *>(glfwGetWindowUserPointer(window));
    nativeWindow->frameBufferResized = true;
}

/**
 * @brief Constructs ikura::GlfwNativeWindow.
 * If surface is nullptr, this function creates new vk::SurfaceKHR object.
 * The other hand, if surface is NOT nullptr, this function constructs
 * GlfwNativeWindow object from passed surface. The passed surface will be
 * destroyed automaticaly.
 */
GlfwNativeWindow::GlfwNativeWindow(
    const std::shared_ptr<RenderEngine> renderEngine,
    GLFWwindow *window = nullptr, vk::SurfaceKHR surface = nullptr,
    std::string name = nullptr) {

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Creating GlfwNativeWindow '" << name << "'...";
    this->renderEngine = renderEngine;
    this->name = name;

    // Set GLFW Basic Properties
    this->window = window;
    glfwGetFramebufferSize(this->window, &width, &height);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    // Create Surface
    if (surface) {
        this->surface = surface;
    } else {
        VkSurfaceKHR vkSurface;
        if ((glfwCreateWindowSurface(renderEngine->getInstance(), this->window,
                                     nullptr, &vkSurface)) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create VkSurfaceKHR from "
                                     "glfwCreateWindowSurface().");
        }
        surface = vk::SurfaceKHR(vkSurface);

        VLOG(VLOG_LV_3_PROCESS_TRACKING)
            << "New Surface for '" << name << "' has been created.";
    }

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Surface for '" << name << "' has been setup.";

    createSwapChain();
    swapChainImages =
        renderEngine->getDevice().getSwapchainImagesKHR(swapChain);
}

GlfwNativeWindow::~GlfwNativeWindow() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Destroying GLFW Window for '" << name << "'...";
    glfwDestroyWindow(window);
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "GLFW Window for '" << name << "' has been destroyed.";
}

void GlfwNativeWindow::createSwapChain() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Creating SwapChain for '" << name << "'...";

    auto surfaceFormats =
        renderEngine->getPhysicalDevice().getSurfaceFormatsKHR(surface);
    auto surfacePresentModes =
        renderEngine->getPhysicalDevice().getSurfacePresentModesKHR(surface);
    auto surfaceCapabilities =
        renderEngine->getPhysicalDevice().getSurfaceCapabilitiesKHR(surface);

    if (VLOG_IS_ON(VLOG_LV_6_ITEM_ENUMERATION)) {
        VLOG(VLOG_LV_6_ITEM_ENUMERATION) << "Available SwapChain formats:";
        for (const auto &format : surfaceFormats) {
            VLOG(VLOG_LV_6_ITEM_ENUMERATION)
                << "\t" << vk::to_string(format.format) << " / "
                << vk::to_string(format.colorSpace);
        }

        VLOG(VLOG_LV_6_ITEM_ENUMERATION)
            << "Available SwapChain present modes:";
        for (const auto &mode : surfacePresentModes) {
            VLOG(VLOG_LV_6_ITEM_ENUMERATION) << "\t" << vk::to_string(mode);
        }
    }

    vk::SurfaceFormatKHR format = chooseSwapChainFormat(surfaceFormats);
    vk::PresentModeKHR presentMode =
        chooseSwapChainPresentMode(surfacePresentModes);
    vk::Extent2D extent = chooseSwapChainExtent(surfaceCapabilities, window);

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Chose SwapChain format: " << vk::to_string(format.format) << " / "
        << vk::to_string(format.colorSpace);
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Chose SwapChain present mode: " << vk::to_string(presentMode);
    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "SwapChain Extent: " << extent.width << "x" << extent.height;

    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 &&
        imageCount > surfaceCapabilities.maxImageCount) {

        imageCount = surfaceCapabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR swapChainCI;
    swapChainCI.surface = surface;
    swapChainCI.minImageCount = imageCount;

    swapChainCI.imageFormat = format.format;
    swapChainCI.imageColorSpace = format.colorSpace;
    swapChainCI.imageExtent = extent;
    swapChainCI.imageArrayLayers = 1;
    swapChainCI.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    swapChainCI.preTransform = surfaceCapabilities.currentTransform;
    swapChainCI.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    swapChainCI.presentMode = presentMode;
    swapChainCI.clipped = VK_TRUE;

    const QueueFamilyIndices queueFamilyIndices =
        renderEngine->getQueueFamilyIndices();
    uint32_t indices[] = {queueFamilyIndices.get(QueueFamilyIndices::GRAPHICS),
                          queueFamilyIndices.get(QueueFamilyIndices::PRESENT)};

    if (queueFamilyIndices.isShareingIndexBetweenGraphicsAndPresent()) {
        swapChainCI.imageSharingMode = vk::SharingMode::eExclusive;
        swapChainCI.queueFamilyIndexCount = 0;
    } else {
        swapChainCI.imageSharingMode = vk::SharingMode::eConcurrent;
        swapChainCI.queueFamilyIndexCount = 2;
        swapChainCI.pQueueFamilyIndices = indices;
    }

    swapChain = renderEngine->getDevice().createSwapchainKHR(swapChainCI);
    swapChainFormat = format.format;
    swapChainExtent = extent;

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "SwapChain for '" << name << "' has been created.";
}

int GlfwNativeWindow::windowShouldClose() {
    return glfwWindowShouldClose(window);
}

void GlfwNativeWindow::draw() {
    // Wait for previous frame to complete
    auto result = renderEngine->getDevice().waitForFences(
        renderTarget->getRenderingFence(currentFrame), VK_TRUE, UINT64_MAX);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Error occurred while waiting fence.");
    }
    renderEngine->getDevice().resetFences(
        renderTarget->getRenderingFence(currentFrame));

    // Acquire swapChain image
    auto nextImage = renderEngine->getDevice().acquireNextImageKHR(
        swapChain, UINT64_MAX,
        renderTarget->getImageAvailableSemaphore(currentFrame), VK_NULL_HANDLE);

    if (nextImage.result == vk::Result::eErrorOutOfDateKHR) {
        // TODO: Recreate SwapChain
        return;
    } else if (nextImage.result != vk::Result::eSuccess &&
               nextImage.result != vk::Result::eSuboptimalKHR) {
        throw std::runtime_error("Failed to acquire next SwapChain image.");
    }

    // Prepare command buffer
    renderTarget->getRenderCommandBuffer(currentFrame).reset();
    recordCommandBuffer(nextImage.value);

    // Submit queue
    // つかれた

    currentFrame = (currentFrame + 1) % numOfFrames;
}

void GlfwNativeWindow::recordCommandBuffer(uint32_t imageIndex) {
    vk::CommandBufferBeginInfo beginInfo{};

    renderTarget->getRenderCommandBuffer(currentFrame).begin(beginInfo);

    vk::RenderPassBeginInfo renderPassInfo{};
    renderPassInfo.renderPass = renderTarget->getRenderPass();
    renderPassInfo.framebuffer = renderTarget->getFramebuffer(imageIndex);
    renderPassInfo.renderArea.offset = vk::Offset2D(0, 0);
    renderPassInfo.renderArea.extent = swapChainExtent;

    std::array<vk::ClearValue, 2> clearValues{};
    clearValues[0].color =
        vk::ClearColorValue(std::array<uint32_t, 4>{0, 0, 0, 0});
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    renderTarget->getRenderCommandBuffer(currentFrame)
        .beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    renderTarget->getRenderCommandBuffer(currentFrame)
        .bindPipeline(vk::PipelineBindPoint::eGraphics,
                      renderTarget->getGraphicsPipeline());

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            pipelineLayout, 0, NUM_OF_DESCRIPTOR_SETS,
                            descriptorSets[currentFrame].data(), 0, nullptr);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0,
                     0, 0);

    ImDrawData *drawData = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffers[currentFrame]);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer.");
    }
}
} // namespace ikura

vk::SurfaceFormatKHR
chooseSwapChainFormat(const std::vector<vk::SurfaceFormatKHR> &formats) {
    for (const auto &format : formats) {
        if (format.format == vk::Format::eR8G8B8A8Srgb &&
            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {

            return format;
        }
    }

    return formats[0];
}

vk::PresentModeKHR chooseSwapChainPresentMode(
    const std::vector<vk::PresentModeKHR> &presentModes) {
    for (const auto &presentMode : presentModes) {
        if (presentMode == vk::PresentModeKHR::eMailbox) {
            return presentMode;
        }
    }

    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D
chooseSwapChainExtent(const vk::SurfaceCapabilitiesKHR &capabilities,
                      GLFWwindow *window) {
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        vk::Extent2D actualExtent = {static_cast<uint32_t>(width),
                                     static_cast<uint32_t>(height)};

        actualExtent.width =
            std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                       capabilities.maxImageExtent.width);
        actualExtent.height =
            std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height);
        return actualExtent;
    }
}