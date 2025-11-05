#pragma once
#include "Entity.h"
#include "Scene.h"
#include "../Graphics/Mesh.h"
#include <vector>
#include "../Graphics/VertexBuffer.h"

/// <summary>
/// Sprite Entity
/// </summary>
class Sprite :
    public Entity
{
private:
	/// <summary>
	/// An internal mesh for the sprite quad
	/// </summary>
	std::unique_ptr<Mesh> m_mesh;

	/// <summary>
	/// The texture image for the sprite
	/// </summary>
	std::unique_ptr<ImageTexture> m_textureImage;

public:

	/// <summary>
	/// Create a sprite from a texture file
	/// </summary>
	/// <param name="name"></param>
	/// <param name="file"></param>
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

	void createAABB() override;
};

