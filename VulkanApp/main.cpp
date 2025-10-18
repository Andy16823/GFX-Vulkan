// TODO: Add dependancys to release builds
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Core/GFX.h"
#include <iostream>
#include "Graphics/Renderer.h"
#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/ImageTexture.h"
#include "Graphics/Mesh.h"
#include "Core/Sprite.h"
#include "Graphics/Camera3D.h"
#include "Core/Component.h"
#include "Components/RotationBehavior.h"
#include "Core/Model.h"
#include "Core/Scene.h"

GLFWwindow* window;
Renderer renderer;

int main() {
	window = GFX::createWindow("Vulkan App", 800, 600);

	auto scene = GFX::createScene();

	auto model = GFX::createModel("D:/3D Modele/GFX/Lion2/Lion.gltf", "Lion");
	model->setPosition(glm::vec3(0.0f, -0.75f, -1.0f));
	model->rotate(0.0f, 45.0f, 0.0f);
	scene->addEntity(std::move(model));

	auto sprite = GFX::createSprite("C:/Users/andy1/Downloads/giraffe.jpg", "GiraffeSprite");
	sprite->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	scene->addEntity(std::move(sprite));


	auto camera = new Camera3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec2(800.0f, 600.0f));

	// Mesh
	//auto sprite = new Sprite("C:/Users/andy1/Downloads/giraffe.jpg");
	//sprite->addComponent(std::make_unique<RotationBehavior>());

	//auto rotBehavior = sprite->findComponent<RotationBehavior>();

	renderer.addOnInitCallback([&scene](Renderer* renderer) {
		//sprite->init(renderer);
		scene->init(renderer);
		});

	renderer.addOnDisposeCallback([&scene](Renderer* renderer) {
		//sprite->destroy(renderer);
		scene->destroy(renderer);
		});

	renderer.addOnDrawCallback([&scene](Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame) {
		//sprite->render(renderer, currentFrame);
		scene->render(renderer, commandBuffer, currentFrame);
		});


	// Init the vulkan renderer
	if (renderer.init(window) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		scene->update(0.016f);

		renderer.setViewProjection(camera->getViewProjection());

		renderer.draw();
	}

	renderer.dispose();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}