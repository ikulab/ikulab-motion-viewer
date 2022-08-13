#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "./nativeWindow.hpp"

namespace ikura {
	class GlfwNativeWindow : public NativeWindow {
		GLFWwindow* window;

		bool frameBufferResized = false;

		void createSwapChain();
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	public:
		GlfwNativeWindow(const RenderEngine& renderEngine, GLFWwindow* window, vk::SurfaceKHR surface, std::string name);
		~GlfwNativeWindow();
	};
}