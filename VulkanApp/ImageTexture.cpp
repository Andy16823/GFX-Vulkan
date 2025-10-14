#include "ImageTexture.h"

ImageTexture::ImageTexture(std::string file)
{
	this->imageData = loadTextureFile(file, &width, &height);
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
