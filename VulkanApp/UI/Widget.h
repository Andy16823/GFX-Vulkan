#pragma once
#include "../Graphics/Renderer.h"
#include <string>

class Canvas;

/// <summary>
/// Widget base class
/// </summary>
class Widget
{
public:
	/// <summary>
	/// The name of the widget
	/// </summary>
	std::string name;

	/// <summary>
	/// Creates a new widget with a specific name
	/// </summary>
	/// <param name="name"></param>
	Widget(const std::string& name) : name(name) {}
	virtual ~Widget() = default;

	/// <summary>
	/// Initialize the widget
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	virtual void init(Canvas* canvas, Renderer* renderer) = 0;

	/// <summary>
	/// Draw the widget
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="frame"></param>
	virtual void draw(Canvas* canvas, Renderer* renderer, VkCommandBuffer commandBuffer, int frame) = 0;

	/// <summary>
	/// update the widget
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="window"></param>
	/// <param name="deltaTime"></param>
	virtual void update(Canvas* canvas, GLFWwindow* window, float deltaTime) = 0;

	/// <summary>
	/// Destroy the widget
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	virtual void destroy(Canvas* canvas, Renderer* renderer) = 0;

	/// <summary>
	/// Called before and after swapchain recreation
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	virtual void beforeSwapchainRecreation(Canvas* canvas, Renderer* renderer) = 0;

	/// <summary>
	/// Called after swapchain recreation
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	/// <param name="newSize"></param>
	virtual void afterSwapchainRecreation(Canvas* canvas, Renderer* renderer, const glm::ivec2& newSize) = 0;

	/// <summary>
	/// Gets the bounds of the widget
	/// X, Y, Width, Height
	/// </summary>
	/// <returns></returns>
	virtual glm::vec4 getBounds() = 0;

	/// <summary>
	/// Checks if the widget contains the specified point
	/// </summary>
	/// <param name="point"></param>
	/// <returns></returns>
	virtual bool containsPoint(const glm::vec2& point) = 0;

	/// <summary>
	/// Mouse over event
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="window"></param>
	/// <param name="mousePos"></param>
	virtual void mouseOver(Canvas* canvas, GLFWwindow* window, const glm::vec2& mousePos) = 0;
};

