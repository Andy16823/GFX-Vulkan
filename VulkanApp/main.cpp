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
#include "Core/Behavior.h"
#include "Components/RotationBehavior.h"
#include "Core/Model.h"
#include "Core/Scene.h"

GLFWwindow* window;
Renderer renderer;

int main() {
	window = GFX::createWindow("Vulkan App", 800, 600);

	auto scene = GFX::createScene();
	auto scene2 = GFX::createScene();

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
	scene->addEntity<Model>(std::move(model));

	auto sprite = GFX::createSprite("C:/Users/andy1/Downloads/giraffe.jpg", "GiraffeSprite");
	sprite->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	scene2->addEntity<Sprite>(std::move(sprite));

	auto camera = new Camera3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec2(800.0f, 600.0f));

	renderer.addOnInitCallback([&scene, &scene2](Renderer* renderer) {
		scene->init(renderer);
		scene2->init(renderer);
		});

	renderer.addOnDisposeCallback([&scene, &scene2](Renderer* renderer) {
		scene->destroy(renderer);
		scene2->destroy(renderer);
		});

	renderer.addOnDrawCallback([&scene, &scene2, &camera](Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame) {
		renderer->updateCamera(0, currentFrame, camera->getViewProjection());
		renderer->setActiveCamera(0);	// Bind camera 0
		scene->render(renderer, commandBuffer, currentFrame);
		scene2->render(renderer, commandBuffer, currentFrame);
		});

	renderer.addOnOffscreenCallback([](Renderer* renderer, VkCommandBuffer commandBuffers, uint32_t currentFrame) {
		// Render the sprite to the offscreen render target
		//auto renderTarget = renderer->getRenderTarget(0);
		//renderTarget->startRecord(renderer->getDevice());
		//renderer->beginnRenderPass(renderTarget->getCommandBuffer(), renderTarget->getFramebuffer(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), renderer->getOffscreenRenderPass());
		//renderer->bindPipeline(renderTarget->getCommandBuffer(), ToString(PipelineType::PIPELINE_TYPE_OFFSCREN_3D_TEST));
		//renderer->drawBuffer(spritePtr->getVertexBufferIndex(), spritePtr->getIndexBufferIndex(), renderTarget->getCommandBuffer());
		//renderer->endRenderPass(renderTarget->getCommandBuffer());
		//renderTarget->endRecord(renderer->getDevice());
		//renderTarget->submitAndWait(renderer->getGraphicsQueue(), renderer->getDevice());
		});



	// Init the vulkan renderer
	if (renderer.init(window) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	int cameraIndex = renderer.createCamera();

	// TODO Complete render target system
	//auto renderTargetIndex = renderer.createRenderTarget();
	//auto renderTarget = renderer.getRenderTarget(renderTargetIndex);
	//renderTarget->startRecord(renderer.getDevice());

	//renderer.beginnRenderPass(renderTarget->getCommandBuffer(), renderTarget->getFramebuffer(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), renderer.getOffscreenRenderPass());
	//renderer.bindPipeline(renderTarget->getCommandBuffer(), ToString(PipelineType::PIPELINE_TYPE_OFFSCREN_3D_TEST));
	//renderer.drawBuffer(spritePtr->getVertexBufferIndex(), spritePtr->getIndexBufferIndex(), renderTarget->getCommandBuffer());
	//renderer.endRenderPass(renderTarget->getCommandBuffer());

	//renderTarget->endRecord(renderer.getDevice());
	//renderTarget->submitAndWait(renderer.getGraphicsQueue(), renderer.getDevice());


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
		scene2->update(0.016f);
		renderer.draw();
	}

	renderer.dispose();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}