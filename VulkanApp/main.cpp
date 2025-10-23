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

	std::vector<std::string> cubemapFaces = {
		"C:/Users/andy1/Documents/Cubemaps/Test/px.png", // +X (right)
		"C:/Users/andy1/Documents/Cubemaps/Test/nx.png", // -X (left)
		"C:/Users/andy1/Documents/Cubemaps/Test/py.png", // +Y (top)
		"C:/Users/andy1/Documents/Cubemaps/Test/ny.png", // -Y (bottom)
		"C:/Users/andy1/Documents/Cubemaps/Test/pz.png", // +Z (front)
		"C:/Users/andy1/Documents/Cubemaps/Test/nz.png"  // -Z (back)
	};
	scene->skybox = std::make_unique<Skybox>(cubemapFaces);

	auto model = GFX::createModel("D:/3D Modele/GFX/Lion2/Lion.gltf", "Lion");
	model->setPosition(glm::vec3(0.0f, -0.75f, -1.0f));
	model->rotate(0.0f, 45.0f, 0.0f);
	scene->addEntity(std::move(model));

	auto sprite = GFX::createSprite("C:/Users/andy1/Downloads/giraffe.jpg", "GiraffeSprite");
	sprite->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	scene->addEntity(std::move(sprite));


	auto camera = new Camera3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec2(800.0f, 600.0f));

	renderer.addOnInitCallback([&scene](Renderer* renderer) {
		scene->init(renderer);
		});

	renderer.addOnDisposeCallback([&scene](Renderer* renderer) {
		scene->destroy(renderer);
		});

	renderer.addOnDrawCallback([&scene](Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame) {
		renderer->beginnRenderPass(commandBuffer, renderer->getSwapchainFramebuffer(currentFrame), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		scene->render(renderer, commandBuffer, currentFrame);
		renderer->endRenderPass(commandBuffer);
		});


	// Init the vulkan renderer
	if (renderer.init(window) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	glm::vec2 mousePosLastFrame = glm::vec2(0.0f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_W)) {
			camera->moveForward(0.05f);
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			camera->moveForward(-0.05f);
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			camera->moveRight(-0.05f);
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			camera->moveRight(0.05f);
		}

		if (glfwGetKey(window, GLFW_KEY_UP)) {
			camera->turn(-0.01f, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			camera->turn(0.01f, 0.0f, 0.0f);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			camera->turn(0.0f, -0.01f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			camera->turn(0.0f, 0.01f, 0.0f);
		}

		if (glfwGetKey(window, GLFW_KEY_PAGE_UP)) {
			camera->moveUp(0.05f);
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN)) {
			camera->moveUp(-0.05f);
		}

		camera->transform.towards(glm::vec3(0.0f, -0.75f, -1.0f));
		scene->update(0.016f);
		renderer.setViewProjection(camera->getViewProjection());
		renderer.draw();
	}

	renderer.dispose();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}