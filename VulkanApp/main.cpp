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

	auto camera = new Camera3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec2(800.0f, 600.0f));


	// Mesh
	auto sprite = new Sprite("C:/Users/andy1/Downloads/giraffe.jpg");
	sprite->addComponent(std::make_unique<RotationBehavior>());

	auto rotBehavior = sprite->findComponent<RotationBehavior>();

	renderer.addOnInitCallback([sprite](Renderer* renderer) {
		sprite->init(renderer);
		});

	renderer.addOnDisposeCallback([sprite](Renderer* renderer) {
		sprite->destroy(renderer);
		});

	renderer.addOnDrawCallback([sprite](Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame) {
		sprite->render(renderer, currentFrame);
		});


	// Init the vulkan renderer
	if (renderer.init(window) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		sprite->update(0.016f);
		renderer.setViewProjection(camera->getViewProjection());

		renderer.draw();
	}

	renderer.dispose();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}