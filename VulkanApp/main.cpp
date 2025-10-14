// TODO: Add dependancys to release builds
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
#include "Sprite.h"
#include "Camera3D.h"
#include "Component.h"
#include "RotationBehavior.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"

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

	auto model = new Model("D:/3D Modele/GFX/Lion2/Lion.gltf");
	model->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));

	auto camera = new Camera3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec2(800.0f, 600.0f));

	// Mesh
	auto sprite = new Sprite("C:/Users/andy1/Downloads/giraffe.jpg");
	sprite->addComponent(std::make_unique<RotationBehavior>());

	auto rotBehavior = sprite->findComponent<RotationBehavior>();

	renderer.addOnInitCallback([sprite, model](Renderer* renderer) {
		sprite->init(renderer);
		model->init(renderer);
		});

	renderer.addOnDisposeCallback([sprite, model](Renderer* renderer) {
		sprite->destroy(renderer);
		model->destroy(renderer);
		});

	renderer.addOnDrawCallback([sprite, model](Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame) {
		sprite->render(renderer, currentFrame);
		model->render(renderer, currentFrame);
		});


	// Init the vulkan renderer
	if (renderer.init(window) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		model->rotate(0.0f, 0.1f, 0.0f);
		sprite->update(0.016f);
		model->update(0.016f);

		renderer.setViewProjection(camera->getViewProjection());

		renderer.draw();
	}

	renderer.dispose();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}