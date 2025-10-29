#include "FontAtlas.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>

bool FontAtlas::loadFont(const std::string& fontPath, int fontSize) {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "[FONT ERROR]: Could not init FreeType Library" << std::endl;
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cerr << "[FONT ERROR]: Failed to load font: " << fontPath << std::endl;
		FT_Done_FreeType(ft);
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	// Erster Pass: Finde maximale Glyph-Größe
	uint32_t maxGlyphWidth = 0;
	uint32_t maxGlyphHeight = 0;

	for (unsigned char c = 32; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}
		maxGlyphWidth = std::max(maxGlyphWidth, face->glyph->bitmap.width);
		maxGlyphHeight = std::max(maxGlyphHeight, face->glyph->bitmap.rows);
	}

	// Padding zwischen Glyphen
	const uint32_t padding = 2;
	uint32_t cellWidth = maxGlyphWidth + padding;
	uint32_t cellHeight = maxGlyphHeight + padding;

	// Grid-Layout berechnen (z.B. 16x6 für 96 Zeichen)
	const uint32_t glyphsPerRow = 16;
	const uint32_t numGlyphs = 96; // ASCII 32-127
	const uint32_t numRows = (numGlyphs + glyphsPerRow - 1) / glyphsPerRow;

	atlasWidth = nextPowerOfTwo(glyphsPerRow * cellWidth);
	atlasHeight = nextPowerOfTwo(numRows * cellHeight);

	std::cout << "[FONT]: Creating atlas " << atlasWidth << "x" << atlasHeight << std::endl;
	std::cout << "[FONT]: Grid: " << glyphsPerRow << "x" << numRows
		<< " (Cell: " << cellWidth << "x" << cellHeight << ")" << std::endl;
	std::cout << "[FONT]: Max glyph: " << maxGlyphWidth << "x" << maxGlyphHeight << std::endl;

	// Atlas Buffer erstellen
	pixelData.resize(atlasWidth * atlasHeight, 0);

	// Zweiter Pass: Glyphen in Grid kopieren
	uint32_t currentGlyph = 0;

	for (unsigned char c = 32; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "[FONT WARNING]: Failed to load char '" << c << "'" << std::endl;
			continue;
		}

		FT_GlyphSlot g = face->glyph;
		FT_Bitmap& bitmap = g->bitmap;

		// Berechne Grid-Position
		uint32_t gridX = currentGlyph % glyphsPerRow;
		uint32_t gridY = currentGlyph / glyphsPerRow;

		uint32_t offsetX = gridX * cellWidth;
		uint32_t offsetY = gridY * cellHeight;

		// Kopiere Bitmap in Atlas
		for (uint32_t row = 0; row < bitmap.rows; row++) {
			for (uint32_t col = 0; col < bitmap.width; col++) {
				uint32_t atlasX = offsetX + col;
				uint32_t atlasY = offsetY + row;
				uint32_t atlasIndex = atlasY * atlasWidth + atlasX;
				uint32_t bitmapIndex = row * bitmap.width + col;

				if (atlasIndex < pixelData.size()) {
					pixelData[atlasIndex] = bitmap.buffer[bitmapIndex];
				}
			}
		}

		// Speichere Character Info
		Character character;
		character.uvX = (float)offsetX / atlasWidth;
		character.uvY = (float)offsetY / atlasHeight;
		character.uvWidth = (float)bitmap.width / atlasWidth;
		character.uvHeight = (float)bitmap.rows / atlasHeight;
		character.width = bitmap.width;
		character.height = bitmap.rows;
		character.bearingX = g->bitmap_left;
		character.bearingY = g->bitmap_top;
		character.advance = g->advance.x >> 6;

		characters[c] = character;
		currentGlyph++;
	}

	std::cout << "[FONT]: Loaded " << characters.size() << " characters" << std::endl;

	// Debug: Überprüfe Atlas
	int nonZeroPixels = 0;
	for (auto pixel : pixelData) {
		if (pixel > 0) nonZeroPixels++;
	}
	std::cout << "[FONT]: Non-zero pixels: " << nonZeroPixels
		<< " (" << (100.0f * nonZeroPixels / pixelData.size()) << "%)" << std::endl;

	// Debug: Zeige ein paar Characters
	if (characters.find('A') != characters.end()) {
		const Character& ch = characters['A'];
		std::cout << "[FONT]: Character 'A' - UV: (" << ch.uvX << ", " << ch.uvY
			<< ") Size: " << ch.uvWidth << "x" << ch.uvHeight
			<< " Pixels: " << ch.width << "x" << ch.height << std::endl;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return true;
}