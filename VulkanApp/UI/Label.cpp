#include "Label.h"

void Label::prepare(const std::string& text, int fontIndex, float scale /*= 1.0f*/, float lineSpacing /*= 1.2f*/)
{
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
