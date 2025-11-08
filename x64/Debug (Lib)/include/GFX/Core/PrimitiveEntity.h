#pragma once
#include "Entity.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Primitive.h"

/// <summary>
/// Represents a primitive entity (cube, sphere, quad, etc.)
/// with a solid color.
/// </summary>
class PrimitiveEntity : public Entity
{
private:
	/// <summary>
	/// The type of primitive
	/// </summary>
	PrimitiveType m_primitiveType;

	/// <summary>
	/// The color of the primitive
	/// </summary>
	glm::vec4 m_color = glm::vec4(1.0f);

public:
	/// <summary>
	/// Creates a primitive entity
	/// </summary>
	/// <param name="name"></param>
	/// <param name="primitiveType"></param>
	PrimitiveEntity(const std::string& name, PrimitiveType primitiveType);

	/// <summary>
	/// Creates a primitive entity with a specified color
	/// </summary>
	/// <param name="name"></param>
	/// <param name="primitiveTypem"></param>
	/// <param name="color"></param>
	PrimitiveEntity(const std::string& name, PrimitiveType primitiveTypem, const glm::vec4& color);

	/// <summary>
	/// Destructor
	/// </summary>
	~PrimitiveEntity() = default;

	/// <summary>
	/// Initialize the primitive entity
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void init(Scene* scene, Renderer* renderer) override;

	/// <summary>
	/// Destroy the primitive entity
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void destroy(Scene* scene, Renderer* renderer) override;

	/// <summary>
	/// Creates the axis-aligned bounding box for the primitive entity
	/// </summary>
	void createAABB() override;

	/// <summary>
	/// Render the primitive entity
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame) override;
};
