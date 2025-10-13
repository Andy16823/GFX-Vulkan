#pragma once
#include "Entity.h"
#include "Mesh.h"
#include <vector>
#include "VertexBuffer.h"

class Sprite :
    public Entity
{
private:
	Mesh* m_mesh;
	ImageTexture* m_texture;


public:
	Sprite(std::string file);
	~Sprite();
	void update(float dt) override;
	void init(Renderer* renderer) override;
	void render(Renderer* renderer, int32_t currentFrame) override;
	void destroy(Renderer* renderer) override;
	
	// Statics
	static std::vector<Vertex> getSpriteVertices();
	static std::vector<uint32_t> getSpriteIndices();
};

