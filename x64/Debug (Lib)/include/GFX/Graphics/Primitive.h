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

enum class PrimitiveType {
	PRIMITIVE_TYPE_TRIANGLE,
	PRIMITIVE_TYPE_QUAD,
	PRIMITIVE_TYPE_CUBE,
	PRIMITIVE_TYPE_SPHERE
};

class Primitive
{
private:
	static PrimitiveData createQuad();
	static PrimitiveData createTriangle();
	static PrimitiveData createCube();
	static PrimitiveData createSphere(int latitudeSegments = 16, int longitudeSegments = 16, float radius = 0.5f);
public:
	static PrimitiveData create(PrimitiveType primitiveType);
};

