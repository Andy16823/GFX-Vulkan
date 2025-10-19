#pragma once
#include "../Graphics/Renderer.h"
#include "../Graphics/Cubemap.h"
#include <vector>
#include <string>

class Skybox
{
private:
	int m_vertexBufferIndex = -1;
	int m_indexBufferIndex = -1;

public:
	std::unique_ptr<Cubemap> cubemap;

	Skybox(std::vector<std::string> faceFiles) {
		cubemap = std::make_unique<Cubemap>(faceFiles);
	}

	void init(Renderer* renderer);
	void render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame);
	void destroy(Renderer* renderer);
	static std::vector<Vertex> getSkyboxVertices();
	static std::vector<uint32_t> getSkyboxIndices();
};

