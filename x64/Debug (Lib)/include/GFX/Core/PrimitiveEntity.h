#pragma once
#include "Entity.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Primitive.h"

class PrimitiveEntity : public Entity
{
private:
	PrimitiveType m_primitiveType;

public:
	PrimitiveEntity(const std::string& name, PrimitiveType primitiveType);
	~PrimitiveEntity() = default;

	void init(Scene* scene, Renderer* renderer) override;
	void destroy(Scene* scene, Renderer* renderer) override;
	void createAABB() override;
	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame) override;

};

