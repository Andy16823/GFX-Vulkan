#include "FontAtlas.h"
#include <stdexcept>
#include <iostream>

bool FontAtlas::loadFont(const std::string& fontPath, int fontSize)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "Could not init FreeType Library" << std::endl;
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		FT_Done_FreeType(ft);
		std::cerr << "Failed to load font: " << fontPath << std::endl;
		return false;
	}
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	// Calculate atlas dimensions
	atlasWidth = 0;
	atlasHeight = 0;

	for (unsigned char c = 32; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}
		atlasWidth += face->glyph->bitmap.width + 1; // +1 for spacing
		atlasHeight = std::max(atlasHeight, face->glyph->bitmap.rows);
	}

	atlasWidth = nextPowerOfTwo(atlasWidth);
	atlasHeight = nextPowerOfTwo(atlasHeight);

	// Create pixel data for the atlas
	pixelData.resize(atlasWidth * atlasHeight, 0);
	uint32_t xOffset = 0;
	for (unsigned char c = 32; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}
		FT_GlyphSlot g = face->glyph;
		FT_Bitmap& bitmap = g->bitmap;

		for (uint32_t row = 0; row < bitmap.rows; row++) {
			for (uint32_t col = 0; col < bitmap.width; col++) {
				uint32_t atlasIndex = (row * atlasWidth) + (xOffset + col);
				uint32_t bitmapIndex = (row * bitmap.width) + col;
				pixelData[atlasIndex] = bitmap.buffer[bitmapIndex];
			}
		}

		Character character = {
			character.uvX = static_cast<float>(xOffset) / atlasWidth,
			character.uvY = 0.0f,
			character.uvWidth = (float)bitmap.width / atlasWidth,
			character.uvHeight = (float)bitmap.rows / atlasHeight,
			character.bearingX = g->bitmap_left,
			character.bearingY = g->bitmap_top,
			character.advance = g->advance.x >> 6
		};
		characters[c] = character;

		xOffset += bitmap.width + 1; // +1 for spacing
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return true;
}
