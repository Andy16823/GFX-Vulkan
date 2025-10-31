#pragma once
#include <vector>
#include <string>
#include "../Graphics/Mesh.h"
class ModelLoader
{
public: 
	static std::vector<std::unique_ptr<Mesh>> loadModelFromFile(const std::string& file);
};

