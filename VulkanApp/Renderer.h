#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "Utils.h"
#include <set>
#include <array>


struct RenderDevice {
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
};


class Renderer
{
private:
	GLFWwindow* m_window;
	VkInstance m_instance;
	RenderDevice m_renderDevice;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapChain;
	bool m_enableValidationLayers = false;
	std::vector<const char*> m_validationLayers;
	std::vector<SwapChainImage> m_swapChainImages;

	VkPipeline m_graphicsPipeline;
	VkPipelineLayout m_pipelineLayout;
	VkRenderPass m_renderPass;

	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

	// Core
	void createValidationLayers();
	void createInstance();
	void getPhysicalDevice();
	void createLogicalDevice();
	void createSurface();
	void createSwapChain();
	void createRenderPass();
	void createGraphicsPipeline(ShaderSourceCollection shaders);

	// Support
	bool checkValidationLayerSupport();
	bool checkInstanceExtensionSupport(const std::vector<const char*>* checkExtensions);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkDeviceSuitable(VkPhysicalDevice device);
	
	// Getters
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails getSwapChainSupport(VkPhysicalDevice device);

	// Helpers
	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	// Create functions
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);
	VkShaderModule createShaderModule(const std::vector<char>& code);
public:
	int init(GLFWwindow* window);
	void dispose();
	~Renderer();
};

