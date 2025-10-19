#include "Sprite.h"
#include "../Graphics/Renderer.h"

Sprite::Sprite(std::string name, std::string file) : Entity(name)
{
	m_textureImage = std::make_unique<ImageTexture>(file);
	m_mesh = std::make_unique<Mesh>();
}

void Sprite::update(float dt)
{
	Entity::update(dt);
}

void Sprite::init(Renderer* renderer)
{
	m_textureImage->bufferIndex = renderer->createImageBuffer(m_textureImage.get());
	m_textureImage->freeImageData();

	auto spriteVertices = getSpriteVertices();
	m_mesh->vertexBufferIndex = renderer->createVertexBuffer(&spriteVertices);

	auto spriteIndices = getSpriteIndices();
	m_mesh->indexBufferIndex = renderer->createIndexBuffer(&spriteIndices);
}

void Sprite::render(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	renderer->bindPipeline(commandBuffer, ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_2D));
	renderer->drawMesh(m_mesh.get(), m_textureImage->bufferIndex, this->getModelMatrix(), currentFrame);
}


void Sprite::destroy(Renderer* renderer)
{

}

std::vector<Vertex> Sprite::getSpriteVertices()
{
	return std::vector<Vertex> {
		{{0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }}, // Bottom Right
		{{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} }, // Top Right
		{ {-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} }, // Top Left
		{ {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} }, // Bottom Left
	};
}

std::vector<uint32_t> Sprite::getSpriteIndices()
{
	return std::vector<uint32_t> {
		0, 1, 2, 2, 3, 0
	};
}
