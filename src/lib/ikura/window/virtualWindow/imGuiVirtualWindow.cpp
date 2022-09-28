#include "./imGuiVirtualWindow.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

namespace ikura {
void ImGuiVirtualWindow::initImGuiResources() {
    ImGui::SetCurrentContext(imGuiContext);

    // create descriptor pool
    vk::DescriptorPoolSize poolSizes[] = {
        {vk::DescriptorType::eSampler, 1000},
        {vk::DescriptorType::eCombinedImageSampler, 1000},
        {vk::DescriptorType::eSampledImage, 1000},
        {vk::DescriptorType::eStorageImage, 1000},
        {vk::DescriptorType::eUniformTexelBuffer, 1000},
        {vk::DescriptorType::eStorageTexelBuffer, 1000},
        {vk::DescriptorType::eUniformBuffer, 1000},
        {vk::DescriptorType::eStorageBuffer, 1000},
        {vk::DescriptorType::eUniformBufferDynamic, 1000},
        {vk::DescriptorType::eStorageBufferDynamic, 1000},
        {vk::DescriptorType::eInputAttachment, 1000}};

    vk::DescriptorPoolCreateInfo poolCI{};
    poolCI.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
    poolCI.maxSets = 1000;
    poolCI.poolSizeCount = std::size(poolSizes);
    poolCI.pPoolSizes = poolSizes;

    imGuiDescriptorPool =
        renderEngine->getDevice().createDescriptorPool(poolCI);

    ImGui_ImplGlfw_InitForVulkan(nativeWindow->getGLFWWindow(), true);

    ImGui_ImplVulkan_InitInfo initInfo{};
    initInfo.Instance = renderEngine->getInstance();
    initInfo.PhysicalDevice = renderEngine->getPhysicalDevice();
    initInfo.Device = renderEngine->getDevice();
    initInfo.Queue = renderEngine->getQueues().graphicsQueue;
    initInfo.DescriptorPool = imGuiDescriptorPool;
    initInfo.MinImageCount = 3;
    initInfo.ImageCount = 3;
    initInfo.MSAASamples = (VkSampleCountFlagBits)renderEngine->getEngineInfo()
                               .limit.maxMsaaSamples;

    ImGui_ImplVulkan_Init(&initInfo,
                          nativeWindow->getRenderTarget()->getRenderPass());

    // font
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("./fonts/NotoSansJP-Medium.otf", 18.0f,
                                 nullptr, io.Fonts->GetGlyphRangesJapanese());

    auto cmd = renderEngine->beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(cmd);
    renderEngine->endSingleTimeCommands(cmd);

    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void ImGuiVirtualWindow::destroyImGuiResources() {
    ImGui::SetCurrentContext(imGuiContext);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    renderEngine->getDevice().destroyDescriptorPool(imGuiDescriptorPool);
}

ImGuiVirtualWindow::ImGuiVirtualWindow(
    std::shared_ptr<RenderEngine> renderEngine,
    std::shared_ptr<GlfwNativeWindow> nativeWindow)
    : VirtualWindow(renderEngine) {

    this->nativeWindow = nativeWindow;

    imGuiContext = ImGui::CreateContext();
    initImGuiResources();
}

ImGuiVirtualWindow::~ImGuiVirtualWindow() { destroyImGuiResources(); }

void ImGuiVirtualWindow::recordCommandBuffer(vk::CommandBuffer cmdBuffer) {
    ImDrawData *drawData = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(drawData, cmdBuffer);
}

bool ImGuiVirtualWindow::isFocused() const {
    setCurrentImGuiContext();
    return ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
}

void ImGuiVirtualWindow::setCurrentImGuiContext() const {
    ImGui::SetCurrentContext(imGuiContext);
}

void ImGuiVirtualWindow::newFrame() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
} // namespace ikura