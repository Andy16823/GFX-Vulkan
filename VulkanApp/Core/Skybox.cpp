#include "Skybox.h"

void Skybox::init(Renderer* renderer)
{
	if (cubemap) {
		cubemap->bufferIndex = renderer->createCubemapBuffer(cubemap->faces);
		cubemap->faces.faceData.clear(); // Free face data after uploading to GPU
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
    return {
        // Front (+Z)
        {{-1.0f, -1.0f,  1.0f}}, // 0
        {{ 1.0f, -1.0f,  1.0f}}, // 1
        {{ 1.0f,  1.0f,  1.0f}}, // 2
        {{-1.0f,  1.0f,  1.0f}}, // 3

        // Back (-Z)
        {{-1.0f, -1.0f, -1.0f}}, // 4
        {{ 1.0f, -1.0f, -1.0f}}, // 5
        {{ 1.0f,  1.0f, -1.0f}}, // 6
        {{-1.0f,  1.0f, -1.0f}}, // 7
    };
}

std::vector<uint32_t> Skybox::getSkyboxIndices()
{
    return {
        // Front (+Z)
        0, 2, 1, 0, 3, 2,
        // Back (-Z)
        5, 6, 4, 4, 6, 7,
        // Left (-X)
        4, 7, 0, 0, 7, 3,
        // Right (+X)
        1, 2, 5, 5, 2, 6,
        // Top (+Y)
        3, 7, 2, 2, 7, 6,
        // Bottom (-Y)
        4, 0, 5, 5, 0, 1
    };
}
