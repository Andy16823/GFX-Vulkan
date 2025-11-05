#include "Primitive.h"
#include <numbers>
#include <cmath>


PrimitiveData Primitive::createQuad()
{
	PrimitiveData quadData;

	quadData.vertices = std::vector<Vertex>{
		{{0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }}, // Bottom Right
		{ {0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} }, // Top Right
		{ {-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} }, // Top Left
		{ {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} }, // Bottom Left
	};

	quadData.indices = std::vector<uint32_t>{
		0, 1, 2, 2, 3, 0
	};

	return quadData;
}

PrimitiveData Primitive::createTriangle()
{
	PrimitiveData triangleData;
	triangleData.vertices = std::vector<Vertex>{
		{{0.0f, 0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }, { 0.5f, 0.0f }}, // Top
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} }, // Bottom Left
		{ {0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }, // Bottom Right
	};
	triangleData.indices = std::vector<uint32_t>{
		0, 1, 2
	};
	return triangleData;
}

PrimitiveData Primitive::createCube()
{
	PrimitiveData cubeData;

	cubeData.vertices = std::vector<Vertex>{
		// Front face
		{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		// Back face
		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
	};

	cubeData.indices = std::vector<uint32_t>{
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		4, 0, 3, 3, 7, 4,
		// Right face
		1, 5, 6, 6, 2, 1,
		// Top face
		3, 2, 6, 6, 7, 3,
		// Bottom face
		4, 5, 1, 1, 0, 4
	};

	return cubeData;
}


PrimitiveData Primitive::createSphere(int latitudeSegments /*= 16*/, int longitudeSegments /*= 16*/, float radius /*= 0.5f*/)
{
	PrimitiveData sphereData;
	auto& vertices = sphereData.vertices;
	auto& indices = sphereData.indices;

	int latitudeBands = latitudeSegments;
	int longitudeBands = longitudeSegments;


	for (int lat = 0; lat <= latitudeBands; lat++)
	{
		float theta = lat * std::numbers::pi / latitudeBands;
		float sinTheta = std::sin(theta);
		float cosTheta = std::cos(theta);

		for (int lon = 0; lon <= longitudeBands; lon++)
		{
			float phi = lon * 2 * std::numbers::pi / longitudeBands;
			float sinPhi = std::sin(phi);
			float cosPhi = std::cos(phi);

			float x = cosPhi * sinTheta;
			float y = cosTheta;
			float z = sinPhi * sinTheta;

			glm::vec3 position(x * radius, y * radius, z * radius);
			glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
			glm::vec2 texCoord((float)lon / longitudeBands, (float)lat / latitudeBands);

			Vertex vertex = {};
			vertex.pos = position;
			vertex.normal = normal;
			vertex.texCoord = texCoord;
			vertex.color = glm::vec3(1.0f);
			vertices.push_back(vertex);
		}
	}

	for (int lat = 0; lat < latitudeBands; lat++)
	{
		for (int lon = 0; lon < longitudeBands; lon++)
		{
			int first = (lat * (longitudeBands + 1)) + lon;
			int second = first + longitudeBands + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	return sphereData;
}

PrimitiveData Primitive::create(PrimitiveType primitiveType)
{
	switch (primitiveType)
	{
	case PrimitiveType::PRIMITIVE_TYPE_TRIANGLE:
		return createTriangle();
	case PrimitiveType::PRIMITIVE_TYPE_QUAD:
		return createQuad();
	case PrimitiveType::PRIMITIVE_TYPE_CUBE:
		return createCube();
	case PrimitiveType::PRIMITIVE_TYPE_SPHERE:
		return createSphere();
	default:
		throw std::runtime_error("Unknown primitive type!");
	}
}

