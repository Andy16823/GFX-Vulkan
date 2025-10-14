#pragma once
#include "Entity.h"
#include "Mesh.h"
#include <vector>
#include "VertexBuffer.h"

class Sprite :
    public Entity
{
private:
	std::unique_ptr<Mesh> m_mesh;
	std::unique_ptr<ImageTexture> m_textureImage;

public:
	Sprite(std::string file);
	~Sprite() = default;
	void update(float dt) override;
	void init(Renderer* renderer) override;
	void render(Renderer* renderer, int32_t currentFrame) override;
	void destroy(Renderer* renderer) override;
	
	// Statics
	static std::vector<Vertex> getSpriteVertices();
	static std::vector<uint32_t> getSpriteIndices();
};

