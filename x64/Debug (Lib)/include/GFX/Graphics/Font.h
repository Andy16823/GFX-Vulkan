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

	FontAtlas m_fontAtlas;
	std::string m_fontName;
	int m_fontSize;
public:
	
	Font(const std::string& fontPath, int fontSize, int textureIndex, const FontAtlas fontAtlas) :
		m_textureBufferIndex(textureIndex), m_fontSize(fontSize),	m_fontName(fontPath), m_fontAtlas(fontAtlas) {}

	void dispose(VkDevice device);
	int getTextureBufferIndex() const { return m_textureBufferIndex; }
	const FontAtlas& getFontAtlas() const { return m_fontAtlas; }
	const std::string& getFontName() const { return m_fontName; }
	int getFontSize() const { return m_fontSize; }
};

