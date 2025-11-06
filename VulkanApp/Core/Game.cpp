#include "Game.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "GFX.h"
#include <thread>
#include <chrono>

void Game::run(const std::string name, const glm::i32vec2 windowSize, const int targetFPS, const bool useFpsLimit)
{
	assetManager = std::make_unique<AssetManager>();
	m_targetFPS = targetFPS;
	m_enableFPSLimit = useFpsLimit;

	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW!");
	}
	m_renderer = std::make_unique<Renderer>();

	// Create the game window
	this->window = GFX::instance().createWindow(name, windowSize.x, windowSize.y);

	// Create the renderer and set up the callbacks
	m_renderer->addOnInitCallback(
		[this](Renderer* renderer)
		{
			// Initialize the asset manager and the game
			this->assetManager->init(renderer);
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
			this->assetManager->dispose(renderer);
		}
	);
	m_renderer->addBeforeSwapchainRecreateCallback(
		[this](Renderer* renderer)
		{
			this->beforeSwapchainRecreation(renderer);
		}
	);
	m_renderer->addAfterSwapchainRecreateCallback(
		[this](Renderer* renderer, const glm::ivec2& dimensions)
		{
			this->afterSwapchainRecreation(renderer, dimensions);
		}
	);

	this->loadContent();

	if (m_renderer->init(this->window) == EXIT_FAILURE) {
		throw std::runtime_error("Failed to initialize renderer!");
	}

	const double targetFrameTime = 1.0 / static_cast<double>(m_targetFPS);

	// Main loop
	while (!glfwWindowShouldClose(this->window)) {
		auto frameStart = std::chrono::high_resolution_clock::now();
		glfwPollEvents();

		// Get the frame time and update the game
		double currentTime = glfwGetTime();
		float deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
		m_lastFrameTime = currentTime;

		this->update(deltaTime);
		m_renderer->draw();

		if (m_enableFPSLimit) {
			auto frameEnd = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = frameEnd - frameStart;
			double sleepTime = targetFrameTime - elapsed.count();
			if (sleepTime > 0.0) {
				std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
			}
		}
	}

	// Dispose the renderer
	m_renderer->dispose();
	GFX::instance().destroyWindows();
	glfwTerminate();
	GFX::instance().clearServices();
}

void Game::stop()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}
