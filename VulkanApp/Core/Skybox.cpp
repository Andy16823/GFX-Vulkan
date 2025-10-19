#include "Skybox.h"

void Skybox::init(Renderer* renderer)
{
	if (cubemap) {
		cubemap->bufferIndex = renderer->createCubemapBuffer(cubemap->faces);
	}
	else {
		throw std::runtime_error("Skybox has no cubemap to initialize!");
	}
	auto vertices = getSkyboxVertices();
	m_vertexBufferIndex = renderer->createVertexBuffer(&vertices);

	auto indices = getSkyboxIndices();
	m_indexBufferIndex = renderer->createIndexBuffer(&indices);
}

void Skybox::render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	renderer->drawSkybox(m_vertexBufferIndex, m_indexBufferIndex, cubemap->bufferIndex, currentFrame);
}

void Skybox::destroy(Renderer* renderer)
{
	// Nothing to destroy since buffers are managed by the renderer
}

std::vector<Vertex> Skybox::getSkyboxVertices()
{
	return std::vector<Vertex> {
		{{-1.0f, -1.0f, -1.0f}, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
		{ {1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
		{ {1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
		{ {-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },
		{ {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
		{ {1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
		{ {1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
		{ {-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },
	};
}

std::vector<uint32_t> Skybox::getSkyboxIndices()
{
	return std::vector<uint32_t> {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		4, 5, 1, 1, 0, 4,
		7, 6, 2, 2, 3, 7,
		4, 0, 3, 3, 7, 4,
		5, 1, 2, 2, 6, 5
	};
}
