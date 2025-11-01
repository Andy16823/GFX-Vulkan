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

