#pragma once
#include "Entity.h"
#include "Scene.h"
#include "../Graphics/Mesh.h"
#include <vector>
#include "../Graphics/VertexBuffer.h"

class Sprite :
    public Entity
{
private:
	std::unique_ptr<Mesh> m_mesh;
	std::unique_ptr<ImageTexture> m_textureImage;

public:
	Sprite(std::string name, std::string file);
	~Sprite() = default;
	void update(Scene* scene, float dt) override;
	void init(Scene* scene, Renderer* renderer) override;
	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame) override;
	void destroy(Scene* scene, Renderer* renderer) override;

	int getTextureBufferIndex() const { return m_textureImage->bufferIndex; }
	int getVertexBufferIndex() const { return m_mesh->vertexBufferIndex; }
	int getIndexBufferIndex() const { return m_mesh->indexBufferIndex; }
	
	// Statics
	static std::vector<Vertex> getSpriteVertices();
	static std::vector<uint32_t> getSpriteIndices();
};

