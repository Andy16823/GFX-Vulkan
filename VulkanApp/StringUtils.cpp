#include "StringUtils.h"

TextMeasurement StringUtils::measureText(const std::string& text, Font* font, float scale, float lineSpacing)
{
	TextMeasurement result;
	result.width = 0.0f;
	result.height = 0.0f;
	result.lineHeight = 0.0f;
	result.lines = 1;

	if (text.empty()) {
		result.lines = 0;
		return result;
	}

	float currentWidth = 0.0f;
	const FontAtlas& atlas = font->getFontAtlas();

	for (char c : text) {
		if (c == '\n') {
			if (currentWidth > result.width) {
				result.width = currentWidth;
			}
			currentWidth = 0.0f;
			result.lines++;
		}
		else {
			if (atlas.characters.find(c) == atlas.characters.end()) continue;
			const Character& ch = atlas.characters.at(c);
			currentWidth += ch.advance * scale;
			float h = ch.height * scale;
			if (h > result.lineHeight) {
				result.lineHeight = h;
			}
		}
	}

	if (currentWidth > result.width) {
		result.width = currentWidth;
	}

	result.height = result.lineHeight * result.lines;
	if (result.lines > 1) {
		result.height += result.lineHeight * (lineSpacing - 1.0f) * (result.lines - 1);
	}
	return result;
}

glm::vec2 StringUtils::getTextOffset(const TextMeasurement& measurement, int alignment)
{
	float currentX = 0;
	float currentY = 0;
	if (TextAlignment::ALIGNMENT_TOP & alignment) {
		currentY -= measurement.lineHeight;
	}
	else if (TextAlignment::ALIGNMENT_MIDDLE & alignment) {
		currentY += (measurement.height - measurement.lineHeight) / 2.0f;
	}
	else if (TextAlignment::ALIGNMENT_BOTTOM & alignment) {
		currentY += measurement.height - measurement.lineHeight;
	}

	if (TextAlignment::ALIGNMENT_CENTER & alignment) {
		currentX -= measurement.width / 2.0f;
	}
	if (TextAlignment::ALIGNMENT_RIGHT & alignment) {
		currentX -= measurement.width;
	}
	return glm::vec2(currentX, currentY);
}
