#include "ImageTexture.h"

ImageTexture::ImageTexture(std::string file)
{
	this->imageData = loadTextureFile(file, &width, &height);
}

ImageTexture::ImageTexture(int width, int height, const std::vector<uint8_t>& pixelData)
{
	this->width = width;
	this->height = height;
	size_t dataSize = width * height * 4;
	this->imageData = (stbi_uc*)malloc(dataSize);
	memcpy(this->imageData, pixelData.data(), dataSize);
}

ImageTexture::~ImageTexture()
{
	this->freeImageData();
}

void ImageTexture::freeImageData()
{
	if (imageData) {
		stbi_image_free(imageData);
		imageData = nullptr;
	}
}
