#include "Label.h"

void Label::prepare(Renderer* renderer, const std::string& text, int fontIndex, float scale /*= 1.0f*/, float lineSpacing /*= 1.2f*/)
{
	// Measure the text to get the bounds
	auto font = renderer->getFont(fontIndex);
	auto fontMetrics = measureText(text, font, scale, lineSpacing);
	m_boundsSize = glm::vec2(fontMetrics.width, fontMetrics.height);

	// Set the properties
	this->m_text = text;
	m_fontIndex = fontIndex;
	m_scale = scale;
	m_lineSpacing = lineSpacing;
}

void Label::init(Canvas* canvas, Renderer* renderer)
{
	m_vertexBuffers.resize(renderer->numSwapChainImages());
	for(size_t i = 0; i < m_vertexBuffers.size(); i++)
	{
		m_vertexBuffers[i] = renderer->createVertexBuffer(2400, VertexBufferType::VERTEX_BUFFER_TYPE_DYNAMIC);
	}
}

void Label::draw(Canvas* canvas, Renderer* renderer, VkCommandBuffer commandBuffer, int frame)
{
	int vertexBufferIndex = m_vertexBuffers[frame];
	renderer->drawText(m_text, m_fontIndex, vertexBufferIndex, commandBuffer, frame, m_position, m_scale, m_lineSpacing, m_alignment);
}

void Label::update(Canvas* canvas, GLFWwindow* window, float deltaTime)
{
	
}

void Label::destroy(Canvas* canvas, Renderer* renderer)
{
	
}

void Label::beforeSwapchainRecreation(Canvas* canvas, Renderer* renderer)
{
	
}

void Label::afterSwapchainRecreation(Canvas* canvas, Renderer* renderer, const glm::ivec2& newSize)
{
	
}

bool Label::containsPoint(const glm::vec2& point)
{
	if (containsPoint2(point, m_position, m_boundsSize)) {
		return true;
	}
	return false;
}

glm::vec4 Label::getBounds()
{
	return glm::vec4(m_position, m_boundsSize);
}

void Label::mouseOver(Canvas* canvas, GLFWwindow* window, const glm::vec2& mousePos)
{
	std::cout << "Mouse over label: " << name << std::endl;
}
