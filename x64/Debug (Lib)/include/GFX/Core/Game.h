#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../Graphics/Renderer.h"

/// <summary>
/// Abstract base class for a game application.
/// </summary>
class Game
{
private:
	/// <summary>
	/// Renderer instance responsible for graphics rendering.
	/// </summary>
	std::unique_ptr<Renderer> m_renderer;

	/// <summary>
	/// Last frame time in milliseconds.
	/// </summary>
	double m_lastFrameTime = 0.0;

	/// <summary>
	/// The target frames per second (FPS) for the game.
	/// </summary>
	int m_targetFPS = 144;

	/// <summary>
	/// Determines whether to enable FPS limiting.
	/// </summary>
	bool m_enableFPSLimit = true;

public:
	/// <summary>
	/// GLFW window handle for the game.
	/// </summary>
	GLFWwindow* window;

	Game() = default;
	~Game() = default;

	/// <summary>
	/// Starts the game with the specified window name and size.
	/// </summary>
	/// <param name="name"></param>
	/// <param name="windowSize"></param>
	void run(const std::string name, const glm::i32vec2 windowSize, const int targetFPS = 144, const bool useFpsLimit = true);

	/// <summary>
	/// Stops the game by signaling the window to close.
	/// </summary>
	void stop();

	/// <summary>
	/// Loads game content such as assets and resources.
	/// </summary>
	virtual void loadContent() = 0;

	/// <summary>
	/// Initializes the game with the given renderer.
	/// </summary>
	/// <param name="renderer"></param>
	virtual void init(Renderer* renderer) = 0;

	/// <summary>
	/// Updates the game state based on the elapsed time since the last frame.
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void update(float deltaTime) = 0;

	/// <summary>
	/// Renders the game using the provided renderer, command buffer, and current frame index.
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	virtual void render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame) = 0;

	/// <summary>
	/// Disposes of game resources using the given renderer.
	/// </summary>
	/// <param name="renderer"></param>
	virtual void dispose(Renderer* renderer) = 0;
};
	
