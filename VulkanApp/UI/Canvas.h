#pragma once
#include <vector>
#include <memory>
#include "../Graphics/Renderer.h"
#include "../Math/Transform.h"
#include "../Graphics/Camera2D.h"
#include "Widget.h"

/// <summary>
/// Canvas for UI rendering
/// </summary>
class Canvas
{
private:
	std::vector<std::unique_ptr<Widget>> m_widgets;
	std::unique_ptr<Camera2D> m_camera;
	int m_cameraIndex = -1;

	glm::vec4 m_clearColor;
	int m_renderTargetIndex = -1;
	Transform m_transform;

public:

	/// <summary>
	/// Creates a new Canvas instance for UI rendering.
	/// </summary>
	/// <param name="position"></param>
	/// <param name="rotation"></param>
	/// <param name="scale"></param>
	/// <param name="viewsize"></param>
	/// <param name="clearColor"></param>
	Canvas(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec2& viewsize, const glm::vec4& clearColor = glm::vec4(0.0f)) {
		// Initialize transform
		m_transform.setPosition(position);
		m_transform.setRotation(glm::quat(glm::radians(rotation)));
		m_transform.setScale(scale);

		// Create 2D camera
		m_camera = std::make_unique<Camera2D>(glm::vec3(0.0f), viewsize, 1.0f, -1.0f);

		// Set clear color
		m_clearColor = clearColor;
	}

	/// <summary>
	/// Destructor for Canvas.
	/// </summary>
	~Canvas() = default;

	/// <summary>
	/// Adds a widget to the canvas and returns a pointer to it.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="widget"></param>
	/// <returns></returns>
	template<typename T>
	T* addWidget(std::unique_ptr<Widget> widget) {
		T* widgetPtr = static_cast<T*>(widget.get());
		m_widgets.push_back(std::move(widget));
		return widgetPtr;
	}

	/// <summary>
	/// Finds a widget by name and returns a pointer to it.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* findWidget(const std::string& name) {
		for (const auto& widget : m_widgets) {
			if (widget->name == name) {
				return static_cast<T*>(widget.get());
			}
		}
	}

	/// <summary>
	/// Gets the clear color of the canvas.
	/// </summary>
	/// <returns></returns>
	glm::vec4 getClearColor() const { 
		return m_clearColor; 
	}

	/// <summary>
	/// Gets the render target index for the canvas
	/// </summary>
	/// <returns></returns>
	int getRenderTargetIndex() const { 
		return m_renderTargetIndex; 
	}

	/// <summary>
	/// Gets the camera used by the canvas.
	/// </summary>
	/// <returns></returns>
	Camera2D* getCamera() const { 
		return m_camera.get(); 
	}

	/// <summary>
	/// Gets the transform of the canvas.
	/// </summary>
	/// <returns></returns>
	Transform& getTransform() { 
		return m_transform; 
	}

	/// <summary>
	/// Initializes the canvas
	/// </summary>
	/// <param name="renderer"></param>
	virtual void init(Renderer* renderer);

	/// <summary>
	/// Renders the canvas
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	virtual void render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame);

	/// <summary>
	/// Update the canvas
	/// </summary>
	/// <param name="window"></param>
	/// <param name="deltaTime"></param>
	virtual void update(GLFWwindow* window, float deltaTime);

	/// <summary>
	/// Destroys the canvas
	/// </summary>
	/// <param name="renderer"></param>
	virtual void destroy(Renderer* renderer);

	/// <summary>
	/// Called before the swapchain is recreated.
	/// Useful for releasing resources that depend on the swapchain.
	/// </summary>
	/// <param name="renderer"></param>
	virtual void beforeSwapchainRecreation(Renderer* renderer);

	/// <summary>
	/// Called after the swapchain is recreated
	/// Useful for recreating resources that depend on the swapchain.
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="newSize"></param>
	virtual void afterSwapchainRecreation(Renderer* renderer, const glm::ivec2& newSize);
};

