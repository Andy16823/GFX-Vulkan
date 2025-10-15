#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "../Utils.h"
#include <set>
#include <array>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Mesh.h"
#include "ImageBuffer.h"
#include "ImageTexture.h"
#include <functional>
#include "Material.h"

struct RenderDevice {
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
};


class Renderer
{
private:
	GLFWwindow* m_window;
	int m_currentFrame = 0;

	UboViewProjection m_uboViewProjection;

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

	// Image Textures
	VkSampler m_textureSampler;

	// Buffer
	std::vector<std::unique_ptr<VertexBuffer>> m_vertexBuffers;
	std::vector<std::unique_ptr<IndexBuffer>> m_indexBuffers;
	std::vector<std::unique_ptr<ImageBuffer>> m_imageBuffers;

	VkCommandPool m_commandPool;

	int m_numFramesInFlight = 0;
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
	std::vector<VkFence> m_imagesInFlight;

	// Callbacks
	std::vector<std::function<void(Renderer*, VkCommandBuffer, uint32_t)>> m_drawCallbacks;
	std::vector<std::function<void(Renderer*)>> m_initCallbacks;
	std::vector<std::function<void(Renderer*)>> m_disposeCallbacks;


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
	VkShaderModule createShaderModule(const std::vector<char>& code);
	int createTextureDescriptor(VkImageView textureImageView);

	// Allocate Functions
	//void allocateDynamicBufferTransferSpace();

public:
	int init(GLFWwindow* window);
	void setViewProjection(const UboViewProjection& vp);
	void disposeImageTexture(int imageTexture);
	int createVertexBuffer(std::vector<Vertex>* vertices);
	int createImageBuffer(ImageTexture* imageTexture);

	VertexBuffer* getVertexBuffer(int index);
	IndexBuffer* getIndexBuffer(int index);
	ImageBuffer* getImageBuffer(int index);
	VkDescriptorSet getDescriptorSet(int index);
	VkDescriptorSet getSamplerDescriptorSet(int index);
	VkCommandBuffer getCommandBuffer(int index);
	VkPipelineLayout getPipelineLayout();
	int createIndexBuffer(std::vector<uint32_t>* indices);
	void draw();

	void addOnDrawCallback(std::function<void(Renderer*, VkCommandBuffer, uint32_t)> callback);
	void addOnInitCallback(std::function<void(Renderer*)> callback);
	void addOnDisposeCallback(std::function<void(Renderer*)> callback);
	void drawMesh(Mesh* mesh, int bufferIndex, UboModel model, int frame);
	void drawMesh(Mesh* mesh, Material* material, UboModel model, int frame);

	void dispose();
	~Renderer();
};

