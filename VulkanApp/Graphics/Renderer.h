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
#include "StorageBuffer.h"
#include "Mesh.h"
#include "ImageBuffer.h"
#include "ImageTexture.h"
#include "CubemapBuffer.h"
#include "FontAtlas.h"
#include <functional>
#include "Material.h"
#include "RenderPass.h"
#include "RenderPassManager.h"
#include "DefaultRenderPass.h"
#include "OffscreenRenderPass.h"
#include "RenderTarget.h"
#include "Font.h"

/// <summary>
/// Renderer configuration structure
/// </summary>
struct RenderConfig {
	size_t maxObjects = 1000;
	size_t maxCameras = 256;
	size_t maxStorageBuffers = 256;
	size_t maxUniformBuffers = 2048;
};

/// <summary>
/// Render device structure
/// </summary>
struct RenderDevice {
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
};

/// <summary>
/// Buffer indices for a primitive
/// </summary>
struct PrimitiveBuffer {
	int vertexBufferIndex;
	int indexBufferIndex;
};

/// <summary>
/// Camera resources for multiple cameras
/// </summary>
struct CameraResources {
	std::vector<std::unique_ptr<UniformBuffer>> uniformBuffers;
	std::vector<VkDescriptorSet> descriptorSets;
};

/// <summary>
/// Pipeline types
/// </summary>
enum class PipelineType {
	PIPELINE_TYPE_GRAPHICS_3D,
	PIPELINE_TYPE_GRAPHICS_3D_INSTANCED,
	PIPELINE_TYPE_GRAPHICS_3D_UNLIT,
	PIPELINE_TYPE_GRAPHICS_3D_UNLIT_INSTANCED,
	PIPELINE_TYPE_GRAPHICS_2D,
	PIPELINE_TYPE_SKYBOX,
	PIPELINE_TYPE_FONT_RENDERING,
	PIPELINE_TYPE_RENDER_TARGET_PRESENT
};

/// <summary>
/// Convert PipelineType to string
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
inline const char* ToString(PipelineType type) {
	switch (type) {
	case PipelineType::PIPELINE_TYPE_GRAPHICS_3D: return "pipeline_3D";
	case PipelineType::PIPELINE_TYPE_GRAPHICS_3D_INSTANCED: return "pipeline_3D_instanced";
	case PipelineType::PIPELINE_TYPE_GRAPHICS_3D_UNLIT: return "pipeline_3D_unlit";
	case PipelineType::PIPELINE_TYPE_GRAPHICS_3D_UNLIT_INSTANCED: return "pipeline_3D_unlit_instanced";
	case PipelineType::PIPELINE_TYPE_GRAPHICS_2D: return "pipeline_2D";
	case PipelineType::PIPELINE_TYPE_SKYBOX: return "pipeline_skybox";
	case PipelineType::PIPELINE_TYPE_FONT_RENDERING: return "pipeline_font_rendering";
	case PipelineType::PIPELINE_TYPE_RENDER_TARGET_PRESENT: return "pipeline_render_target_present";
	default: return "unknown";
	}
}

/// <summary>
/// Text alignment options
/// </summary>
enum TextAlignment {
	ALIGNMENT_LEFT = 1,
	ALIGNMENT_CENTER = 2,
	ALIGNMENT_RIGHT = 4,
	ALIGNMENT_TOP = 8,
	ALIGNMENT_MIDDLE = 16,
	ALIGNMENT_BOTTOM = 32
};

/// <summary>
/// Primitive types
/// </summary>
enum class PrimitiveType {
	PRIMITVE_TYPE_QUAD
};

class Renderer
{
private:
	GLFWwindow* m_window;
	int m_currentFrame = 0;

	RenderConfig m_renderConfig;

	// CORE VULKAN STUFF
	VkInstance m_instance;
	RenderDevice m_renderDevice;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
	VkSurfaceKHR m_surface;
	bool m_enableValidationLayers = false;
	std::vector<const char*> m_validationLayers;

	// RENDERER PRIMIVES
	std::map<PrimitiveType, PrimitiveBuffer> m_rendererPrimitives;

	std::vector<CameraResources> m_cameraResources;
	int m_activeCamera = -1;

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

	// Camera Uniform Buffers
	VkDescriptorPool m_cameraDescriptorPool;
	VkDescriptorSetLayout m_cameraDescriptorSetLayout;

	// Uniform Buffers
	std::vector<std::unique_ptr<UniformBuffer>> m_uniformBuffers;
	std::vector<VkDescriptorSet> m_uniformBufferDescriptorSets; 
	VkDescriptorSetLayout m_uniformBufferSetLayout;
	VkDescriptorPool m_uniformBufferDescriptorPool;

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

	// STORAGE BUFFERS
	std::vector<std::unique_ptr<StorageBuffer>> m_storageBuffers;
	std::vector<VkDescriptorSet> m_storageBufferDescriptorSets;
	VkDescriptorSetLayout m_storageBufferSetLayout; // done
	VkDescriptorPool m_storageBufferDescriptorPool; // done

	// OTHER RESOURCES
	std::vector<std::unique_ptr<Font>> m_loadedFonts;

	// Callbacks
	std::vector<std::function<void(Renderer*, VkCommandBuffer, uint32_t)>> m_drawCallbacks;
	std::vector<std::function<void(Renderer*)>> m_initCallbacks;
	std::vector<std::function<void(Renderer*)>> m_disposeCallbacks;
	std::vector<std::function<void(Renderer*, VkCommandBuffer, uint32_t)>> m_offscreenCallbacks;
	std::vector<std::function<void(Renderer*)>> m_beforeSwapchainRecreateCallbacks;
	std::vector<std::function<void(Renderer*, const glm::ivec2&)>> m_afterSwapchainRecreateCallbacks;

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
	void createDescriptorPool();
	void createSampler();
	void createRendererPrimitives();

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

	// Swapchain Recreation
	void recreateSurface();
	void recreateSwapChain();
	void cleanupSwapChain();

	// Helpers
	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkFormat chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features);
	void validateCurrentPipeline();

	// Create functions
	VkShaderModule createShaderModule(const std::vector<char>& code);
	int createUniformBufferDescriptor(VkBuffer uniformBuffer, VkDeviceSize bufferSize);
	int createTextureDescriptor(VkImageView textureImageView);
	int createCubemapDescriptor(VkImageView cubemapImageView);
	int createStorageBufferDescriptor(VkBuffer storageBuffer, VkDeviceSize bufferSize);

	// Allocate Functions
	//void allocateDynamicBufferTransferSpace();

public:
	// Main functions
	int init(GLFWwindow* window);
	void draw();
	void dispose();

	// Setters
	void disposeImageTexture(int imageTexture);

	// Callbacks
	void addOnDrawCallback(std::function<void(Renderer*, VkCommandBuffer, uint32_t)> callback);
	void addOnInitCallback(std::function<void(Renderer*)> callback);
	void addOnDisposeCallback(std::function<void(Renderer*)> callback);
	void addOnOffscreenCallback(std::function<void(Renderer*, VkCommandBuffer, uint32_t)> callback);
	void addBeforeSwapchainRecreateCallback(std::function<void(Renderer*)> callback);
	void addAfterSwapchainRecreateCallback(std::function<void(Renderer*, glm::ivec2)> callback);

	// Getters
	VkDevice getDevice();
	VkQueue getGraphicsQueue();	
	VkDescriptorSet getUniformBufferDescriptorSet(int index);
	VkDescriptorSet getSamplerDescriptorSet(int index);
	VkDescriptorSet getSamplerDescriptorSetFromImageBuffer(int imageBufferIndex);
	VkDescriptorSet getCubemapDescriptorSet(int index);
	VkDescriptorSet getStorageBufferDescriptorSet(int index);
	VkDescriptorSet getCameraDescriptorSet(int cameraIndex, uint32_t frame);
	VkCommandBuffer getCommandBuffer(int index);
	VkPipelineLayout getPipelineLayout(std::string pipelineName);
	VkPipelineLayout getCurrentPipelineLayout();
	VkFramebuffer getSwapchainFramebuffer(int index);
	RenderTarget* getRenderTarget(int index);
	Font* getFont(int index);
	int getActiveCamera();
	size_t numSwapChainImages();

	// Setters
	void setConfig(RenderConfig config);
	
	// Create buffer functions
	int createVertexBuffer(std::vector<Vertex>* vertices, const VertexBufferType vertexBufferType = VertexBufferType::VERTEX_BUFFER_TYPE_STATIC);
	int createVertexBuffer(const int size, const VertexBufferType vertexBufferType = VertexBufferType::VERTEX_BUFFER_TYPE_DYNAMIC);
	int createUniformBuffer(const VkDeviceSize bufferSize);
	int createImageBuffer(ImageTexture* imageTexture);
	int createImageBuffer(const FontAtlas& fontAtlas);
	int createCubemapBuffer(CubemapFaceData faces);
	int createIndexBuffer(std::vector<uint32_t>* indices);
	int createRenderTarget(const bool presentOnScreen = false);
	int createStorageBuffer(VkDeviceSize size);
	int createCamera();

	// Loader functions
	int loadFont(const std::string& fontPath, int fontSize);

	// Get buffer functions
	VertexBuffer* getVertexBuffer(int index);
	IndexBuffer* getIndexBuffer(int index);
	UniformBuffer* getUniformBuffer(int index);
	ImageBuffer* getImageBuffer(int index);
	CubemapBuffer* getCubemapBuffer(int index);
	StorageBuffer* getStorageBuffer(int index);

	// Create functions
	VkViewport getSwapchainViewport();
	VkRect2D getSwapchainBounds();

	// Pipeline functions
	void createPipeline(std::string name, PipelineCreateInfos infos, std::function<void(Pipeline*, Renderer*)> creationCallback = nullptr);
	void bindPipeline(VkCommandBuffer commandBuffer, std::string pipelineName);
	Pipeline* getPipeline(const std::string& pipelineName);

	// Bind functions
	void bindDescriptorSet(Pipeline* pipeline, VkDescriptorSet descriptorSet, int firstSet, int frame);
	void bindDescriptorSet(VkDescriptorSet descriptorSet, int firstSet, int frame);
	void bindDescriptorSets(Pipeline* pipeline, const std::vector<VkDescriptorSet>& descriptorSets, int firstSet, int frame);
	void bindDescriptorSets(const std::vector<VkDescriptorSet>& descriptorSets, int frame);
	void bindDescriptorSets(const std::vector<VkDescriptorSet>& descriptorSets, int firstSet, int frame);
	template<typename C>
	void bindDescriptorSets(const C& descriptorSets, int firstSet, int frame);
	void bindPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
	void setActiveCamera(int cameraIndex);


	// Beginn / End functions
	int getMainRenderPass() { return m_mainRenderPassIndex; }
	int getOffscreenRenderPass() { return m_offscreenRenderPassIndex; }
	void beginnRenderPass(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, glm::vec4 clearColor, int renderPassIndex);
	void endRenderPass(VkCommandBuffer commandBuffer);

	// Update functions
	void updateCamera(int cameraIndex, uint32_t frame, const UboViewProjection& vp);
	void updateUniformBuffer(int uniformBufferIndex, const void* data, VkDeviceSize size, VkDeviceSize offset = 0);
	void updateStorageBuffer(int storageBufferIndex, const void* data, VkDeviceSize size, VkDeviceSize offset = 0);
	void updateTextureDescriptor(int descriptorIndex, VkImageView textureImageView);

	// Update functions for RenderTarget
	void cleanupRenderTarget(int renderTargetIndex);
	void recreateRenderTarget(int renderTargetIndex);
	void recreateRenderTarget(int renderTargetIndex, VkExtent2D newSize);
	void recreateRenderTarget(int renderTargetIndex, const glm::vec2& newSize);

	// Draw functions
	void drawBuffers(int vertexBufferIndex, int indexBufferIndex, VkCommandBuffer commandBuffer, int instances = 1);
	void drawSkybox(uint32_t vertexBufferIndex, uint32_t indexBufferIndex, uint32_t cubemapBufferIndex, int frame);
	void drawRenderTargetQuad(RenderTarget* rendertarget, VkCommandBuffer commandBuffer, int frame);
	void drawTexture(int textureBufferIndex, VkCommandBuffer commandBuffer, int frame, glm::vec2 position, glm::vec2 size);
	void drawText(const std::string& text, const int fontIndex, const int vertexBufferIndex, VkCommandBuffer commandBuffer, int frame, glm::vec2 position, float scale, float lineSpacing = 1.2, int textalignment = TextAlignment::ALIGNMENT_CENTER | TextAlignment::ALIGNMENT_MIDDLE);

	~Renderer();
};

template<typename C>
void Renderer::bindDescriptorSets(const C& descriptorSets, int firstSet, int frame)
{
	validateCurrentPipeline();
	VkCommandBuffer commandBuffer = this->getCommandBuffer(frame);
	VkPipelineLayout pipelineLayout = m_currentPipeline->getPipelineLayout();

	vkCmdBindDescriptorSets(
		commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipelineLayout,
		static_cast<uint32_t>(firstSet),
		static_cast<uint32_t>(descriptorSets.size()),
		descriptorSets.data(),
		0,
		nullptr
	);
}

