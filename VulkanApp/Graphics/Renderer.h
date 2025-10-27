#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "../Utils.h"
#include <set>
#include <array>
#include "PipelineManager.h"
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Mesh.h"
#include "ImageBuffer.h"
#include "ImageTexture.h"
#include "CubemapBuffer.h"
#include <functional>
#include "Material.h"
#include "RenderPass.h"
#include "RenderPassManager.h"
#include "DefaultRenderPass.h"
#include "OffscreenRenderPass.h"
#include "RenderTarget.h"

struct RenderDevice {
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
};

enum class PipelineType {
	PIPELINE_TYPE_GRAPHICS_3D,
	PIPELINE_TYPE_GRAPHICS_2D,
	PIPELINE_TYPE_SKYBOX,
	PIPELINE_TYPE_OFFSCREN_3D_TEST,
	PIPELINE_TYPE_RENDER_TARGET_PRESENT
};

inline const char* ToString(PipelineType type) {
	switch (type) {
	case PipelineType::PIPELINE_TYPE_GRAPHICS_3D: return "pipeline_3D";
	case PipelineType::PIPELINE_TYPE_GRAPHICS_2D: return "pipeline_2D";
	case PipelineType::PIPELINE_TYPE_SKYBOX: return "pipeline_skybox";
	case PipelineType::PIPELINE_TYPE_OFFSCREN_3D_TEST: return "pipeline_offscreen_3D_test";
	case PipelineType::PIPELINE_TYPE_RENDER_TARGET_PRESENT: return "pipeline_render_target_present";
	default: return "unknown";
	}
}

class Renderer
{
private:
	GLFWwindow* m_window;
	int m_currentFrame = 0;

	// CURRENT VIEW PROJECTION
	UboViewProjection m_uboViewProjection;



	// CORE VULKAN STUFF
	VkInstance m_instance;
	RenderDevice m_renderDevice;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
	VkSurfaceKHR m_surface;
	bool m_enableValidationLayers = false;
	std::vector<const char*> m_validationLayers;

	// RENDER TARGETS
	std::vector<std::unique_ptr<RenderTarget>> m_renderTargets;

	// PIPELINE & COMMAND BUFFERS
	std::unique_ptr<PipelineManager> m_pipelineManager;
	Pipeline* m_currentPipeline;
	VkCommandPool m_commandPool;

	// RENDER PASS
	RenderPassManager m_renderPassManager;
	int m_mainRenderPassIndex = -1;
	int m_offscreenRenderPassIndex = -1;

	// SWAP CHAIN STUFF
	int m_numFramesInFlight = 0;
	VkSwapchainKHR m_swapChain;
	VkExtent2D m_swapChainExtent;
	VkFormat m_swapChainImageFormat;
	std::vector<SwapChainImage> m_swapChainImages;
	std::vector<VkFramebuffer> m_swapChainFramebuffers;
	std::vector<VkCommandBuffer> m_commandBuffers;
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
	std::vector<VkFence> m_imagesInFlight;

	// Depth resources
	VkFormat m_depthBufferFormat;
	VkImage m_depthBufferImage;
	VkDeviceMemory m_depthBufferImageMemory;
	VkImageView m_depthBufferImageView;

	// UNIFORM BUFFERS & DESCRIPTORS
	std::vector<UniformBuffer*> m_uniformBuffers;
	std::vector<UniformBuffer*> m_dynamicUniformBuffers;
	VkDescriptorPool m_descriptorPool;
	VkDescriptorSetLayout m_descriptorSetLayout;
	std::vector<VkDescriptorSet> m_descriptorSets;

	// TEXTURE STUFF
	std::vector<std::unique_ptr<ImageBuffer>> m_imageBuffers;
	std::vector<VkDescriptorSet> m_samplerDescriptorSets;
	VkSampler m_textureSampler;
	VkDescriptorSetLayout m_samplerSetLayout;
	VkDescriptorPool m_samplerDescriptorPool;

	// CUBEMAP STUFF
	std::vector<std::unique_ptr<CubemapBuffer>> m_cubemaps;
	std::vector<VkDescriptorSet> m_cubemapDescriptorSets;
	VkSampler m_cubemapSampler;
	VkDescriptorSetLayout m_cubemapSetLayout;
	VkDescriptorPool m_cubemapDescriptorPool;
	
	// VERTEX BUFFERS
	std::vector<std::unique_ptr<VertexBuffer>> m_vertexBuffers;
	std::vector<std::unique_ptr<IndexBuffer>> m_indexBuffers;

	// Callbacks
	std::vector<std::function<void(Renderer*, VkCommandBuffer, uint32_t)>> m_drawCallbacks;
	std::vector<std::function<void(Renderer*)>> m_initCallbacks;
	std::vector<std::function<void(Renderer*)>> m_disposeCallbacks;
	std::vector<std::function<void(Renderer*, VkCommandBuffer, uint32_t)>> m_offscreenCallbacks;

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
	void createGraphicsPipelines();
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
	void validateCurrentPipeline();

	// Create functions
	VkShaderModule createShaderModule(const std::vector<char>& code);
	int createTextureDescriptor(VkImageView textureImageView);
	int createCubemapDescriptor(VkImageView cubemapImageView);

	// Allocate Functions
	//void allocateDynamicBufferTransferSpace();

public:
	// Main functions
	int init(GLFWwindow* window);
	void draw();
	void dispose();

	// Setters
	void setViewProjection(const UboViewProjection& vp);
	void disposeImageTexture(int imageTexture);

	// Callbacks
	void addOnDrawCallback(std::function<void(Renderer*, VkCommandBuffer, uint32_t)> callback);
	void addOnInitCallback(std::function<void(Renderer*)> callback);
	void addOnDisposeCallback(std::function<void(Renderer*)> callback);

	// Getters
	VkDevice getDevice();
	VkQueue getGraphicsQueue();
	VkDescriptorSet getDescriptorSet(int index);
	VkDescriptorSet getSamplerDescriptorSet(int index);
	VkDescriptorSet getSamplerDescriptorSetFromImageBuffer(int imageBufferIndex);
	VkDescriptorSet getCubemapDescriptorSet(int index);
	VkCommandBuffer getCommandBuffer(int index);
	VkPipelineLayout getPipelineLayout(std::string pipelineName);
	VkPipelineLayout getCurrentPipelineLayout();
	VkFramebuffer getSwapchainFramebuffer(int index);
	RenderTarget* getRenderTarget(int index);
	
	// Create buffer functions
	int createVertexBuffer(std::vector<Vertex>* vertices);
	int createImageBuffer(ImageTexture* imageTexture);
	int createCubemapBuffer(CubemapFaceData faces);
	int createIndexBuffer(std::vector<uint32_t>* indices);
	int createRenderTarget();

	// Get buffer functions
	VertexBuffer* getVertexBuffer(int index);
	IndexBuffer* getIndexBuffer(int index);
	ImageBuffer* getImageBuffer(int index);
	CubemapBuffer* getCubemapBuffer(int index);

	// Create functions
	VkViewport getSwapchainViewport();
	VkRect2D getSwapchainBounds();

	// Pipeline functions
	void createPipeline(std::string name, PipelineCreateInfos infos, std::function<void(Pipeline*, Renderer*)> creationCallback = nullptr);
	void bindPipeline(VkCommandBuffer commandBuffer, std::string pipelineName);

	// Descriptor & Push constant functions
	void bindDescriptorSets(std::vector<VkDescriptorSet> descriptorSets, int frame);
	void bindPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);

	// Beginn / End functions
	int getMainRenderPass() { return m_mainRenderPassIndex; }
	int getOffscreenRenderPass() { return m_offscreenRenderPassIndex; }
	void beginnRenderPass(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, glm::vec4 clearColor, int renderPassIndex);
	void endRenderPass(VkCommandBuffer commandBuffer);

	// Draw functions
	void drawBuffer(int vertexBufferIndex, int indexBufferIndex, VkCommandBuffer commandBuffer);
	void drawMesh(Mesh* mesh, int bufferIndex, UboModel model, int frame);
	void drawMesh(Mesh* mesh, Material* material, UboModel model, int frame);
	void drawSkybox(uint32_t vertexBufferIndex, uint32_t indexBufferIndex, uint32_t cubemapBufferIndex, int frame);

	~Renderer();
};

