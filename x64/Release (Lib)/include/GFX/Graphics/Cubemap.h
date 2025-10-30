#pragma once
#include <vector>
#include "CubemapBuffer.h"

class Cubemap
{
public:
	int bufferIndex = -1;
	CubemapFaceData faces;

	Cubemap(std::vector<std::string> faceFiles);
	~Cubemap();
};

