#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "Utils.h"
#include <set>
#include <array>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Mesh.h"
#include "stb_image.h"


struct RenderDevice {
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
};


class Renderer
{
private:
	GLFWwindow* m_window;
	int m_currentFrame = 0;

	std::vector<Mesh*> m_meshes; // TODO: Remove and replace with an funtion

	struct UboViewProjection {
		glm::mat4 projection;
		glm::mat4 view;
	} m_uboViewProjection;

	VkInstance m_instance;
	RenderDevice m_renderDevice;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapChain;
	bool m_enableValidationLayers = false;
	std::vector<const char*> m_validationLayers;
	std::vector<SwapChainImage> m_swapChainImages;
	std::vector<VkFramebuffer> m_swapChainFramebuffers;

	VkPipeline m_graphicsPipeline;
	VkPipelineLayout m_pipelineLayout;
	VkRenderPass m_renderPass;

	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;
	std::vector<VkCommandBuffer> m_commandBuffers;

	// Depth resources
	VkFormat m_depthBufferFormat;
	VkImage m_depthBufferImage;
	VkDeviceMemory m_depthBufferImageMemory;
	VkImageView m_depthBufferImageView;

	VkDescriptorSetLayout m_descriptorSetLayout;
	VkDescriptorSetLayout m_samplerSetLayout;
	//VkPushConstantRange m_pushConstantRange;

	VkDescriptorPool m_descriptorPool;
	VkDescriptorPool m_samplerDescriptorPool;
	std::vector<VkDescriptorSet> m_descriptorSets;
	std::vector<VkDescriptorSet> m_samplerDescriptorSets;

	std::vector<UniformBuffer*> m_uniformBuffers;
	std::vector<UniformBuffer*> m_dynamicUniformBuffers;


	//VkDeviceSize m_minUniformBufferOffset;
	//size_t m_modelUniformAlignment;
	//Model* m_modelTransferSpace;

	// Assets TODO: Remove it out here
	VkSampler m_textureSampler;
	std::vector<VkImage> m_textureImages;
	std::vector<VkDeviceMemory> m_textureImageMemories;
	std::vector<VkImageView> m_textureImageViews;

	VkCommandPool m_commandPool;

	int m_numFramesInFlight = 0;
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
	std::vector<VkFence> m_imagesInFlight;


	// Core
	void createValidationLayers();
	void createInstance();
	void getPhysicalDevice();
	void createLogicalDevice();
	void createSurface();
	void createSwapChain();
	void createRenderPass();
	void createDescriptorSetLayout();
	//void createPushConstantRange();
	void createGraphicsPipeline(ShaderSourceCollection shaders);
	void createDepthBufferImage();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createTextureSampler();

	void updateUniformBuffer(uint32_t currentImage);

	// Record
	void recordCommands(uint32_t currentImage);

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
	VkFormat chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features);

	// Create functions
	VkImage createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory); // TODO CHANGE LATER
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);
	VkShaderModule createShaderModule(const std::vector<char>& code);
	int createTextureImage(std::string fileName);
	int createTexture(std::string fileName);
	int createTextureDescriptor(VkImageView textureImageView);

	// Loader functions
	stbi_uc* loadTextureFile(std::string fileName, int* width, int* height, VkDeviceSize* imageSize);

	// Allocate Functions
	//void allocateDynamicBufferTransferSpace();

public:
	int init(GLFWwindow* window);
	void setProjectionMatrix(glm::mat4 proj) { m_uboViewProjection.projection = proj; }
	void setViewMatrix(glm::mat4 view) { m_uboViewProjection.view = view; }
	void draw();
	void dispose();
	~Renderer();
};

