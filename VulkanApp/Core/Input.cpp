#include "Input.h"

bool Input::isKeyPressed(int key, GLFWwindow* window)
{
	if (glfwGetKey(window, key) == GLFW_PRESS) {
		return true;
	}
    return false;
}

bool Input::isMouseButtonPressed(int button, GLFWwindow* window)
{
	if (glfwGetMouseButton(window, button) == GLFW_PRESS) {
		return true;
	}
	return false;
}

glm::vec2 Input::getMousePosition(GLFWwindow* window)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

glm::vec2 Input::getMouseDelta(GLFWwindow* window, glm::vec2& lastMousePosition)
{
	glm::vec2 currentMousePosition = getMousePosition(window);
	glm::vec2 delta = currentMousePosition - lastMousePosition;
	lastMousePosition = currentMousePosition;
	return delta;
}
