#include "Sprite.h"
#include "../Graphics/Renderer.h"

Sprite::Sprite(std::string name, std::string file) : Entity(name)
{
	this->pipelineType = ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_2D);
	m_textureImage = std::make_unique<ImageTexture>(file);
	m_mesh = std::make_unique<Mesh>();
}

void Sprite::update(Scene* scene, float dt)
{
	Entity::update(scene, dt);
}

void Sprite::init(Scene* scene, Renderer* renderer)
{
	m_textureImage->bufferIndex = renderer->createImageBuffer(m_textureImage.get());
	m_textureImage->freeImageData();

	auto spriteVertices = getSpriteVertices();
	m_mesh->vertexBufferIndex = renderer->createVertexBuffer(&spriteVertices);

	auto spriteIndices = getSpriteIndices();
	m_mesh->indexBufferIndex = renderer->createIndexBuffer(&spriteIndices);
}

void Sprite::render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	if (this->pipelineType.empty()) {
		throw std::runtime_error("failed to render sprite: pipeline type is not set!");
	}

	int currentCamera = renderer->getActiveCamera();
	auto modelMatrix = this->getModelMatrix();
	auto descriptorSet = renderer->getCameraDescriptorSet(currentCamera, currentFrame);
	auto imageDescriptorSet = renderer->getSamplerDescriptorSetFromImageBuffer(m_textureImage->bufferIndex);

	std::vector<VkDescriptorSet> descriptorSets = {
		descriptorSet,
		imageDescriptorSet
	};
	renderer->bindPipeline(commandBuffer, this->pipelineType);
	scene->bindSceneDescriptorSets(renderer, commandBuffer, currentFrame, this->pipelineType);
	renderer->bindPushConstants(
		commandBuffer,
		renderer->getCurrentPipelineLayout(),
		VK_SHADER_STAGE_VERTEX_BIT,
		0,
		sizeof(UboModel),
		&modelMatrix
	);
	renderer->bindDescriptorSets(descriptorSets, currentFrame);
	renderer->drawBuffers(m_mesh->vertexBufferIndex, m_mesh->indexBufferIndex, commandBuffer);
}


void Sprite::destroy(Scene* scene, Renderer* renderer)
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
