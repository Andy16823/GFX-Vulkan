#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include "FontAtlas.h"


class Font
{
private:
	int m_textureBufferIndex;
	int m_vertexBufferIndex;
	int m_vertexBufferCapcity;

	FontAtlas m_fontAtlas;
	std::string m_fontName;
	int m_fontSize;
public:
	
	Font(const std::string& fontPath, int fontSize, int textureIndex, int vertexIndex, const FontAtlas fontAtlas) :
		m_textureBufferIndex(textureIndex), m_vertexBufferIndex(vertexIndex), m_fontSize(fontSize),	m_fontName(fontPath), m_fontAtlas(fontAtlas), m_vertexBufferCapcity(0) {}

	void dispose(VkDevice device);
	int getTextureBufferIndex() const { return m_textureBufferIndex; }
	int getVertexBufferIndex() const { return m_vertexBufferIndex; }
	int getVertexBufferCapacity() const { return m_vertexBufferCapcity; }
	const FontAtlas& getFontAtlas() const { return m_fontAtlas; }

	void setVertexBufferCapacity(int capacity) { m_vertexBufferCapcity = capacity; }
	const std::string& getFontName() const { return m_fontName; }
	int getFontSize() const { return m_fontSize; }

};

