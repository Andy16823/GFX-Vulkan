#pragma once
#include <string>
#include "../Utils.h"
#include "../Graphics/Renderer.h"

class Scene;
class Entity;

/// <summary>
/// Entity Behavior Base Class
/// </summary>
class Behavior
{
public:
	/// <summary>
	/// The parent entity of this behavior
	/// </summary>
	Entity* parent = nullptr;
	virtual ~Behavior() = default;

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
	/// Get the unique identifier of this behavior
	/// </summary>
	/// <returns></returns>
	virtual std::string getIdentifier() = 0;
};

