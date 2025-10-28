#pragma once
#include <freetype/freetype.h>
#include <string>
#include <map>
#include <vector>

struct Character {
	float uvX, uvY; // UV coordinates in the atlas
	uint32_t uvWidth, uvHeight; // Size of glyph
	uint32_t  bearingX, bearingY; // Offset from baseline to left/top of glyph
	uint32_t  advance; // Offset to advance to next glyph
};

class FontAtlas
{
private:
    uint32_t nextPowerOfTwo(uint32_t n) {
        n--;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n++;
        return n;
    }

public:
	FontAtlas() = default;
	~FontAtlas() = default;
	std::map<char, Character> characters;
	uint32_t atlasWidth;
	uint32_t atlasHeight;
	std::vector<unsigned char> pixelData; // Grayscale pixel data for the atlas

	bool loadFont(const std::string& fontPath, int fontSize);
};

