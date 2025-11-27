#pragma once
#include <string>
#include <glm/vec2.hpp>
#include "Graphics/Font.h"

/// <summary>
/// Text alignment options
/// </summary>
enum TextAlignment {
	ALIGNMENT_LEFT = 1,
	ALIGNMENT_CENTER = 2,
	ALIGNMENT_RIGHT = 4,
	ALIGNMENT_TOP = 8,
	ALIGNMENT_MIDDLE = 16,
	ALIGNMENT_BOTTOM = 32
};


/// <summary>
/// Text measurement structure
/// </summary>
struct TextMeasurement {
	float width;
	float height;
	float lineHeight;
	int lines;
};

/// <summary>
/// String utility class
/// </summary>
class StringUtils
{
public:
	/// <summary>
	/// Measures the size of the given text when rendered with the specified font, scale, and line spacing.
	/// </summary>
	/// <param name="text"></param>
	/// <param name="font"></param>
	/// <param name="scale"></param>
	/// <param name="lineSpacing"></param>
	/// <returns></returns>
	static TextMeasurement measureText(const std::string& text, Font* font, float scale, float lineSpacing);

	/// <summary>
	/// Gets the text offset based on the measurement and alignment.
	/// </summary>
	/// <param name="measurement"></param>
	/// <param name="alignment"></param>
	/// <returns></returns>
	static glm::vec2 getTextOffset(const TextMeasurement& measurement, int alignment);
};

