#include "Cubemap.h"
#include "../Utils.h"

Cubemap::Cubemap(std::vector<std::string> faceFiles)
{
	faces.faceData.resize(6);
	for (size_t i = 0; i < faceFiles.size(); i++)
	{
		// Load each face image
		int width, height, channels;
		faces.faceData[i] = loadTextureFile(faceFiles[i], &width, &height);

		// Check if loading was successful
		if (!faces.faceData[i]) {
			throw std::runtime_error("Failed to load cubemap face image: " + faceFiles[i]);
		}

		// Ensure all faces have the same dimensions
		if (i == 0) {
			faces.width = width;
			faces.height = height;
		}
		else {
			if (faces.width != width || faces.height != height) {
				throw std::runtime_error("Cubemap face images must have the same dimensions.");
			}
		}
	}
}

Cubemap::~Cubemap()
{
	for (size_t i = 0; i < faces.faceData.size(); i++)
	{
		if (faces.faceData[i]) {
			stbi_image_free(faces.faceData[i]);
		}
	}
}
