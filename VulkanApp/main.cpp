#define STB_IMAGE_IMPLEMENTATION
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <iostream>
#include "Renderer.h"
#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ImageTexture.h"
#include "Mesh.h"

GLFWwindow* window;
Renderer renderer;

GLFWwindow& CreateWindow(std::string wName = "GFX C++", const int width = 800, const int height = 600) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	auto window =  glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
	return *window;
}

int main() {
	window = &CreateWindow();
	
	// Define assets here
	auto imageTexture = new ImageTexture("C:/Users/andy1/Downloads/giraffe.jpg");
	auto imagetexture2 = new ImageTexture("C:/Users/andy1/Downloads/panda.jpg");

	// Mesh
	// Create Mesh TODO: Replace with function
	std::vector<Vertex> vertices = {
		// First Square
		{{-0.1f, -0.4f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{-0.1f, 0.4f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{-0.9f, 0.4f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-0.9f, -0.4f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

		// Second Square
		{{ 0.9f, -0.4f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{0.9f, 0.4f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{0.1f, 0.4f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{0.1f, -0.4f, 0.0f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f }}
	};

	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	std::vector<uint32_t> indices2 = {
		4, 5, 6, 6, 7, 4
	};

	// Init the vulkan renderer
	if (renderer.init(window) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	renderer.addImageTexture(imageTexture);
	renderer.addImageTexture(imagetexture2);
	int vbo = renderer.createVertexBuffer(&vertices);
	int ibo = renderer.createIndexBuffer(&indices);
	int ibo2 = renderer.createIndexBuffer(&indices2);

	Mesh* mesh = new Mesh();
	mesh->textureIndex = imageTexture->descriptorIndex;	
	mesh->vertexBufferIndex = vbo;
	mesh->indexBufferIndex = ibo;
	renderer.addMesh(mesh);

	Mesh* mesh2 = new Mesh();
	mesh2->vertexBufferIndex = vbo;
	mesh2->indexBufferIndex = ibo2;
	mesh2->textureIndex = imagetexture2->descriptorIndex;
	renderer.addMesh(mesh2);

	//glm::mat4 meshModelMatrix = mesh->getModel().model;
	//meshModelMatrix = glm::rotate(meshModelMatrix, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//meshModelMatrix = glm::translate(meshModelMatrix, glm::vec3(0.0f, 0.0f, 0.2f));
	//mesh->setModelMatrix(meshModelMatrix);
	//mesh2->setModelMatrix(meshModelMatrix);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		renderer.draw();
	}

	renderer.dispose();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}