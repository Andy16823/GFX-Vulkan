#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Input
{
public:
	/// <summary>
	/// Checks if a specific key is currently pressed in the given window.
	/// </summary>
	/// <param name="key"></param>
	/// <param name="window"></param>
	/// <returns></returns>
	static bool isKeyPressed(int key, GLFWwindow* window);

	/// <summary>
	/// Checks if a specific mouse button is currently pressed in the given window.
	/// </summary>
	/// <param name="button"></param>
	/// <param name="window"></param>
	/// <returns></returns>
	static bool isMouseButtonPressed(int button, GLFWwindow* window);

	/// <summary>
	/// Gets the current mouse position in the given window.
	/// </summary>
	/// <param name="window"></param>
	/// <returns></returns>
	static glm::vec2 getMousePosition(GLFWwindow* window);

	/// <summary>
	/// Gets the change in mouse position (delta) since the last call, updating the lastMousePosition.
	/// </summary>
	/// <param name="window"></param>
	/// <param name="lastMousePosition"></param>
	/// <returns></returns>
	static glm::vec2 getMouseDelta(GLFWwindow* window, glm::vec2& lastMousePosition);
};

