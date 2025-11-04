#pragma once
#include "../Graphics/Renderer.h"
#include <vector>
#include "Entity.h"
#include "Skybox.h"

/// <summary>
/// Base class for all scenes
/// Initializes a render target for rendering the scene
/// Call base init in derived classes to create the render target
/// </summary>
class Scene
{
private:
	/// <summary>
	/// The render target index for this scene
	/// </summary>
	int m_renderTargetIndex = -1;

public:
	/// <summary>
	/// init the scene
	/// </summary>
	/// <param name="renderer"></param>
	virtual void init(Renderer* renderer);

	/// <summary>
	/// update the scene
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void update(float deltaTime) = 0;

	/// <summary>
	/// render the scene
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	virtual void render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame) = 0;

	/// <summary>
	/// Bind the scene descriptor sets to the current pipeline layout
	/// Get called from entities when rendering
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	/// <param name="currentPipeline"></param>
	virtual void bindSceneDescriptorSets(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame, const std::string& currentPipeline) = 0;

	/// <summary>
	/// Called before the swapchain is recreated
	/// </summary>
	/// <param name="renderer"></param>
	virtual void beforeSwapchainRecreate(Renderer* renderer);

	/// <summary>
	/// Called after the swapchain is recreated
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="newSize"></param>
	virtual void afterSwapchainRecreate(Renderer* renderer, const glm::ivec2& newSize);

	/// <summary>
	/// destroy the scene
	/// </summary>
	/// <param name="renderer"></param>
	virtual void destroy(Renderer* renderer) = 0;

	Scene() = default;
	~Scene() = default;

	/// <summary>
	/// Returns the render target index for this scene
	/// </summary>
	/// <returns></returns>
	int getRenderTargetIndex() const {
		return m_renderTargetIndex;
	}
};

