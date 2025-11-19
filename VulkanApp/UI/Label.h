#pragma once
#include "Widget.h"
#include "Canvas.h"
#include "../Graphics/Renderer.h"

/// <summary>
/// Label UI Widget
/// </summary>
class Label : public Widget
{
private:
	std::string m_text;
	int m_fontIndex = -1;
	float m_scale = 1.0f;
	float m_lineSpacing = 1.2f;
	glm::vec2 m_position = glm::vec2(0.0f);
	std::vector<int> m_vertexBuffers;
	int m_alignment = TextAlignment::ALIGNMENT_CENTER | TextAlignment::ALIGNMENT_MIDDLE;
public:

	/// <summary>
	/// Create a label at a specific position with a name
	/// </summary>
	/// <param name="position"></param>
	/// <param name="n"></param>
	Label(const glm::vec2& position, const std::string& n) 
		: Widget(n), m_position(position) {}

	/// <summary>
	/// Gets the position of the label.
	/// </summary>
	/// <returns></returns>
	glm::vec2 getPosition() const { return m_position; }

	/// <summary>
	/// Sets the position of the label.
	/// </summary>
	/// <param name="position"></param>
	void setPosition(const glm::vec2& position) { m_position = position; }

	/// <summary>
	/// Gets the scale of the label.
	/// </summary>
	/// <returns></returns>
	float getScale() const { return m_scale; }

	/// <summary>
	/// Sets the scale of the label.
	/// </summary>
	/// <param name="scale"></param>
	void setScale(float scale) { m_scale = scale; }

	/// <summary>
	/// Get the line spacing of the label.
	/// </summary>
	/// <returns></returns>
	float getLineSpacing() const { return m_lineSpacing; }

	/// <summary>
	/// Sets the line spacing of the label.
	/// </summary>
	/// <param name="lineSpacing"></param>
	void setLineSpacing(float lineSpacing) { m_lineSpacing = lineSpacing; }

	/// <summary>
	/// Sets the font index of the label.
	/// </summary>
	/// <param name="fontIndex"></param>
	void setFontIndex(int fontIndex) { m_fontIndex = fontIndex; }

	/// <summary>
	/// Sets the text alignment of the label.
	/// </summary>
	/// <param name="alignment"></param>
	void setAlignment(int alignment) { m_alignment = alignment; }

	/// <summary>
	/// Prepare the label with text, font, scale and line spacing
	/// </summary>
	/// <param name="text"></param>
	/// <param name="fontIndex"></param>
	/// <param name="scale"></param>
	/// <param name="lineSpacing"></param>
	void prepare(const std::string& text, int fontIndex, float scale = 1.0f, float lineSpacing = 1.2f);

	/// <summary>
	/// Initialize the label
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	void init(Canvas* canvas, Renderer* renderer) override;

	/// <summary>
	/// Render the label
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="frame"></param>
	void draw(Canvas* canvas, Renderer* renderer, VkCommandBuffer commandBuffer, int frame) override;

	/// <summary>
	/// Update the label
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="window"></param>
	/// <param name="deltaTime"></param>
	void update(Canvas* canvas, GLFWwindow* window, float deltaTime) override;

	/// <summary>
	/// Destroy the label
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	void destroy(Canvas* canvas, Renderer* renderer) override;

	/// <summary>
	/// React to before swapchain recreation
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	void beforeSwapchainRecreation(Canvas* canvas, Renderer* renderer) override;

	/// <summary>
	/// React to after swapchain recreation
	/// </summary>
	/// <param name="canvas"></param>
	/// <param name="renderer"></param>
	/// <param name="newSize"></param>
	void afterSwapchainRecreation(Canvas* canvas, Renderer* renderer, const glm::ivec2& newSize) override;
};

