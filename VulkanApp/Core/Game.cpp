#include "Game.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "GFX.h"

void Game::run(const std::string name, const glm::i32vec2 windowSize)
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW!");
	}
	m_renderer = std::make_unique<Renderer>();

	// Create the game window
	this->window = GFX::createWindow(name, windowSize.x, windowSize.y);

	// Create the renderer and set up the callbacks
	m_renderer->addOnInitCallback(
		[this](Renderer* renderer)
		{
			this->init(renderer);
		}
	);
	m_renderer->addOnDrawCallback(
		[this](Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame)
		{
			this->render(renderer, commandBuffer, currentFrame);
		}
	);
	m_renderer->addOnDisposeCallback(
		[this](Renderer* renderer)
		{
			this->dispose(renderer);
		}
	);

	this->loadContent();

	if (m_renderer->init(this->window) == EXIT_FAILURE) {
		throw std::runtime_error("Failed to initialize renderer!");
	}

	// Main loop
	while (!glfwWindowShouldClose(this->window)) {
		glfwPollEvents();

		// Get the frame time and update the game
		float frametime = glfwGetTime() * 1000; // Convert to milliseconds
		this->update(frametime);
		m_renderer->draw();

		// Reset the timer
		glfwSetTime(0.0);
	}

	// Dispose the renderer
	m_renderer->dispose();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Game::stop()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}
