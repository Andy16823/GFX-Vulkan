#pragma once
#include "Renderer.h"

/// <summary>
/// Base class for lights
/// </summary>
class Light
{
public: 
	Light() = default;
	virtual ~Light() = default;

	/// <summary>
	/// Initialize the light
	/// </summary>
	/// <param name="renderer"></param>
	virtual void init(Renderer* renderer) = 0;

	/// <summary>
	/// Updates the light buffers for the current frame
	/// </summary>
	/// <param name="rendderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="frame"></param>
	virtual void updateBuffers(Renderer* renderer, VkCommandBuffer commandBuffer, int frame) = 0;

	/// <summary>
	/// Binds the light against the current pipeline layout
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="frame"></param>
	/// <param name="firstSet"></param>
	virtual void bind(Renderer* renderer, VkCommandBuffer commandBuffer, int frame, const std::string& pipeline) = 0;

	/// <summary>
	/// dipose the light
	/// </summary>
	/// <param name="renderer"></param>
	virtual void dispose(Renderer* renderer) = 0;
};

