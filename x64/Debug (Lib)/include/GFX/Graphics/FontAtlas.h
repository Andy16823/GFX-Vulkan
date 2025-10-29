#pragma once
#include <freetype/freetype.h>
#include <string>
#include <map>
#include <vector>

struct Character {
	float uvX, uvY;              // UV coordinates in the atlas (0.0 - 1.0)
	float uvWidth, uvHeight;     // UV size (0.0 - 1.0) <-- WICHTIG: float!
	int32_t bearingX, bearingY;  // Offset from baseline
	uint32_t advance;            // Horizontal advance
	uint32_t width, height;      // Pixel size (für Debugging)
};

class FontAtlas {
private:
	uint32_t nextPowerOfTwo(uint32_t n) {
		if (n == 0) return 1;
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
	std::vector<unsigned char> pixelData;

	bool loadFont(const std::string& fontPath, int fontSize);
};