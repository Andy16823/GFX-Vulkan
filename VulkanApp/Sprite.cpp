#include "Sprite.h"
#include "Renderer.h"

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

void Sprite::render(Renderer* renderer, int32_t currentFrame)
{
	//auto vertexBuffer = renderer->getVertexBuffer(m_mesh->vertexBufferIndex);
	//auto indexBuffer = renderer->getIndexBuffer(m_mesh->indexBufferIndex);
	//auto imageBuffer = renderer->getImageBuffer(m_textureImage->bufferIndex);

	//VkBuffer vertexBuffers[] = { vertexBuffer->getVertexBuffer() };
	//VkDeviceSize offsets[] = { 0 };
	//uint32_t indexCount = static_cast<uint32_t>(indexBuffer->getIndexCount());

	//auto uniformDescriptorSet = renderer->getDescriptorSet(currentFrame);

	//std::array<VkDescriptorSet, 2> descriptorSets = {
	//	renderer->getDescriptorSet(currentFrame),
	//	renderer->getSamplerDescriptorSet(imageBuffer->descriptorIndex)
	//};

	//auto commandBuffer = renderer->getCommandBuffer(currentFrame);
	//auto pipelineLayout = renderer->getPipelineLayout();

	////uint32_t dynamicOffset = static_cast<uint32_t>(m_modelUniformAlignment) * j;
	//vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
	//	0, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);

	//auto thisModel = this->getModelMatrix();
	//vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(UboModel), &thisModel);
	//vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	//vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
	//vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);

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
