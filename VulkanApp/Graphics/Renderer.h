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
/// Maximum number of objects supported by the renderer
/// </summary>
const int MAX_OBJECTS = 1000;
const int DEFAULT_DYNAMIC_BUFFER_SIZE = 1200;
const int MAX_CAMERAS = 256;

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
	PIPELINE_TYPE_GRAPHICS_2D,
	PIPELINE_TYPE_SKYBOX,
	PIPELINE_TYPE_FONT_RENDERING,
	PIPELINE_TYPE_RENDER_TARGET_PRESENT
};

inline const char* ToString(PipelineType type) {
	switch (type) {
	case PipelineType::PIPELINE_TYPE_GRAPHICS_3D: return "pipeline_3D";
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

	// UNIFORM BUFFERS & DESCRIPTORS
	//std::vector<UniformBuffer*> m_uniformBuffers;
	VkDescriptorPool m_descriptorPool;
	VkDescriptorSetLayout m_descriptorSetLayout;

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

	// OTHER RESOURCES
	std::vector<std::unique_ptr<Font>> m_loadedFonts;

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
	void disposeImageTexture(int imageTexture);

	// Callbacks
	void addOnDrawCallback(std::function<void(Renderer*, VkCommandBuffer, uint32_t)> callback);
	void addOnInitCallback(std::function<void(Renderer*)> callback);
	void addOnDisposeCallback(std::function<void(Renderer*)> callback);
	void addOnOffscreenCallback(std::function<void(Renderer*, VkCommandBuffer, uint32_t)> callback);

	// Getters
	VkDevice getDevice();
	VkQueue getGraphicsQueue();		
	VkDescriptorSet getSamplerDescriptorSet(int index);
	VkDescriptorSet getSamplerDescriptorSetFromImageBuffer(int imageBufferIndex);
	VkDescriptorSet getCubemapDescriptorSet(int index);
	VkDescriptorSet getCameraDescriptorSet(int cameraIndex, uint32_t frame);
	VkCommandBuffer getCommandBuffer(int index);
	VkPipelineLayout getPipelineLayout(std::string pipelineName);
	VkPipelineLayout getCurrentPipelineLayout();
	VkFramebuffer getSwapchainFramebuffer(int index);
	RenderTarget* getRenderTarget(int index);
	Font* getFont(int index);
	int getActiveCamera();
	
	// Create buffer functions
	int createVertexBuffer(std::vector<Vertex>* vertices, const VertexBufferType vertexBufferType = VertexBufferType::VERTEX_BUFFER_TYPE_STATIC);
	int createVertexBuffer(const int size, const VertexBufferType vertexBufferType = VertexBufferType::VERTEX_BUFFER_TYPE_DYNAMIC);
	int createImageBuffer(ImageTexture* imageTexture);
	int createImageBuffer(const FontAtlas& fontAtlas);
	int createCubemapBuffer(CubemapFaceData faces);
	int createIndexBuffer(std::vector<uint32_t>* indices);
	int createRenderTarget(const bool presentOnScreen = false);
	int createCamera();

	// Loader functions
	int loadFont(const std::string& fontPath, int fontSize);

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

	// Bind functions
	void bindDescriptorSets(std::vector<VkDescriptorSet> descriptorSets, int frame);
	void bindPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
	void setActiveCamera(int cameraIndex);


	// Beginn / End functions
	int getMainRenderPass() { return m_mainRenderPassIndex; }
	int getOffscreenRenderPass() { return m_offscreenRenderPassIndex; }
	void beginnRenderPass(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, glm::vec4 clearColor, int renderPassIndex);
	void endRenderPass(VkCommandBuffer commandBuffer);

	// Update functions
	void updateCamera(int cameraIndex, uint32_t frame, const UboViewProjection& vp);


	// Draw functions
	void drawBuffers(int vertexBufferIndex, int indexBufferIndex, VkCommandBuffer commandBuffer);
	void drawMesh(Mesh* mesh, Material* material, UboModel model, int frame);
	void drawSkybox(uint32_t vertexBufferIndex, uint32_t indexBufferIndex, uint32_t cubemapBufferIndex, int frame);
	void drawRenderTargetQuad(RenderTarget* rendertarget, VkCommandBuffer commandBuffer, int frame);
	void drawTexture(int textureBufferIndex, VkCommandBuffer commandBuffer, int frame, glm::vec2 position, glm::vec2 size);
	void drawText(const std::string& text, const int fontIndex, const int vertexBufferIndex, VkCommandBuffer commandBuffer, int frame, glm::vec2 position, float scale, int textalignment = TextAlignment::ALIGNMENT_CENTER | TextAlignment::ALIGNMENT_MIDDLE);

	~Renderer();
};

