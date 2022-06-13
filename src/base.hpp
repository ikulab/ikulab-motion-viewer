#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <optional>
#include <functional>
#include <chrono>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "./definition/vertex.hpp"
#include "./definition/common.hpp"
#include "./definition/descriptor.hpp"
#include "./animator.hpp"

#ifdef NODEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VkResult CreateDebugUtilsMessengerEXT(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateinfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator);

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() &&
			presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct ModelMatUBO {
	alignas(16) glm::mat4 model[MAX_ID];
};

struct SceneMatUBO {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct MouseInputContext {
	bool leftButton = false;
	bool rightButton = false;
	bool middleButton = false;
	double dragStartX = 0.0;
	double dragStartY = 0.0;
	double dragEndX = 0.0;
	double dragEndY = 0.0;
	double currentX = 0.0;
	double currentY = 0.0;
};
class Base {
	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;

	bool framebufferResized = false;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImageView> swapChainImageViews;

	VkSurfaceKHR surface;

	int windowWidth = 0;
	int windowHeight = 0;

	std::chrono::_V2::system_clock::time_point startTime;
	std::chrono::_V2::system_clock::time_point currentTime;
	float secondsFromStart;

	float fps = 60.0;

	// ImGui status
	bool showDemoWindow = false;
	bool windowSizeInitialized = false;

	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	void createLogicalDevice();
	void createSurface();
	void createSwapChain();
	void recreateSwapChain();
	void createImageViews();
	void createGraphicsPipeline();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createRenderPass();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();
	void createBuffer(
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory
	);
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSets();
	void createImage(
		uint32_t width,
		uint32_t height,
		uint32_t mipLevels,
		VkSampleCountFlagBits numSamples,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage& image,
		VkDeviceMemory& imageMemory
	);
	VkImageView createImageView(
		VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels
	);
	void createDepthResources();
	void createColorResource();

	void cleanupSwapChain();

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyBufferToImage(
		VkBuffer buffer,
		VkImage image,
		uint32_t width,
		uint32_t height
	);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	std::array<VkDescriptorSetLayout, NUM_OF_DESCRIPTOR_SETS> descriptorSetLayouts;

	std::vector<VkFramebuffer> swapChainFrameBuffers;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	uint32_t currentFrame = 0;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	std::array<std::vector<VkBuffer>, MAX_FRAMES_IN_FLIGHT> uniformBuffers;
	std::array<std::vector<VkDeviceMemory>, MAX_FRAMES_IN_FLIGHT> uniformBufferMemories;
	uint32_t mipLevels;

	VkDescriptorPool descriptorPool;
	std::array<std::vector<VkDescriptorSet>, MAX_FRAMES_IN_FLIGHT> descriptorSets;
	VkDescriptorPool imguiDescriptorPool;


	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void updateUniformBuffer(uint32_t currentImage);

	void transitionImageLayout(
		VkImage image,
		VkFormat format,
		VkImageLayout oldLayout,
		VkImageLayout newLayout,
		uint32_t mipLevels
	);

	VkFormat findSupportedFormat(
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features
	);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);

	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t txtWidth, int32_t txtHeight, uint32_t mipLevels);

	VkSampleCountFlagBits getMaxUsableSampleCount();

	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	void initWindow();
	void createInstance();
	void cleanup();

	static std::vector<char> readFile(const std::string& fileName);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<Base*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	std::shared_ptr<Animator> anim;

	MouseInputContext mouseCtx;

	// GLFW event callbacks ---
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void registerInputEvents();
	// ---
public:
	Base() {
		initWindow();
		registerInputEvents();
	}

	~Base() {
		vkDeviceWaitIdle(device);
		cleanup();
	}

	void initVulkan();
	void initImGui();

	void pollWindowEvent();
	int windowShouldClose();
	void drawImGuiFrame();
	void drawFrame();

	void vSync();
	/// call this after vSync()
	void updateClock();

	void addVertex(Vertex vertex);
	void addVertices(const std::vector<Vertex>& vertices);
	void addIndex(uint32_t index);
	void addindices(const std::vector<uint32_t>& indices);

	void setAnimator(std::shared_ptr<Animator> anim);
};