#pragma once
#include "../Graphics/VertexBuffer.h"

/// <summary>
/// The data for a primitive
/// </summary>
struct PrimitiveData
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

/// <summary>
/// Primitive Types
/// </summary>
enum class PrimitiveType {
	PRIMITIVE_TYPE_TRIANGLE,
	PRIMITIVE_TYPE_QUAD,
	PRIMITIVE_TYPE_CUBE,
	PRIMITIVE_TYPE_SPHERE
};

/// <summary>
/// Primitive Generator
/// </summary>
class Primitive
{
private:
	/// <summary>
	/// Create a quad primitive
	/// </summary>
	/// <returns></returns>
	static PrimitiveData createQuad();

	/// <summary>
	/// Create a triangle primitive
	/// </summary>
	/// <returns></returns>
	static PrimitiveData createTriangle();

	/// <summary>
	/// Create a cube primitive
	/// </summary>
	/// <returns></returns>
	static PrimitiveData createCube();

	/// <summary>
	/// Create a sphere primitive
	/// </summary>
	/// <param name="latitudeSegments"></param>
	/// <param name="longitudeSegments"></param>
	/// <param name="radius"></param>
	/// <returns></returns>
	static PrimitiveData createSphere(int latitudeSegments = 16, int longitudeSegments = 16, float radius = 0.5f);
public:

	/// <summary>
	/// Create a primitive of a specific type
	/// </summary>
	/// <param name="primitiveType"></param>
	/// <returns></returns>
	static PrimitiveData create(PrimitiveType primitiveType);
};

