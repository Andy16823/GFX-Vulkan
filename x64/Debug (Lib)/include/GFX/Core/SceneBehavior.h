#pragma once
#include <string>
#include "../Graphics/Renderer.h"

class Scene;

/// <summary>
/// Scene Behavior Interface
/// </summary>
class SceneBehavior
{
public:
	virtual ~SceneBehavior() = default;

	/// <summary>
	/// Initialize the behavior	
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	virtual void init(Scene* scene, Renderer* renderer) = 0;

	/// <summary>
	/// Update the behavior
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="dt"></param>
	virtual void update(Scene* scene, float dt) = 0;

	/// <summary>
	/// Destroy the behavior
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	virtual void destroy(Scene* scene, Renderer* renderer) = 0;

	/// <summary>
	/// Render the behavior
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	/// <param name="commandbuffer"></param>
	/// <param name="frame"></param>
	virtual void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandbuffer, int frame) = 0;

	/// <summary>
	/// Before swapchain recreation callback
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	virtual void beforeSwapchainRecreation(Scene* scene, Renderer* renderer) = 0;

	/// <summary>
	/// After swapchain recreation callback
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	/// <param name="size"></param>
	virtual void afterSwapchainRecreation(Scene* scene, Renderer* renderer, const glm::ivec2& size) = 0;
};

