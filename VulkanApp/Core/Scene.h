#pragma once
#include "../Graphics/Renderer.h"
#include <vector>
#include "Entity.h"
#include "Skybox.h"
#include "SceneBehavior.h"


/// <summary>
/// Base class for all scenes
/// Initializes a render target for rendering the scene
/// Call base init in derived classes to create the render target
/// </summary>
class Scene
{
private:
	/// <summary>
	/// The render target index for this scene
	/// </summary>
	int m_renderTargetIndex = -1;

	/// <summary>
	/// Vector of scene behaviors
	/// </summary>
	std::vector<std::unique_ptr<SceneBehavior>> m_sceneBehaviors;
public:
	/// <summary>
	/// init the scene
	/// </summary>
	/// <param name="renderer"></param>
	virtual void init(Renderer* renderer);

	/// <summary>
	/// update the scene
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void update(float deltaTime);

	/// <summary>
	/// render the scene
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	virtual void render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame);

	/// <summary>
	/// Bind the scene descriptor sets to the current pipeline layout
	/// Get called from entities when rendering
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	/// <param name="currentPipeline"></param>
	virtual void bindSceneDescriptorSets(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame, const std::string& currentPipeline) = 0;

	/// <summary>
	/// Called before the swapchain is recreated
	/// </summary>
	/// <param name="renderer"></param>
	virtual void beforeSwapchainRecreate(Renderer* renderer);

	/// <summary>
	/// Called after the swapchain is recreated
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="newSize"></param>
	virtual void afterSwapchainRecreate(Renderer* renderer, const glm::ivec2& newSize);

	/// <summary>
	/// destroy the scene
	/// </summary>
	/// <param name="renderer"></param>
	virtual void destroy(Renderer* renderer);

	Scene() = default;
	~Scene() = default;

	/// <summary>
	/// Returns the render target index for this scene
	/// </summary>
	/// <returns></returns>
	int getRenderTargetIndex() const {
		return m_renderTargetIndex;
	}

	/// <summary>
	/// Add a scene behavior of a specific type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="behavior"></param>
	/// <returns></returns>
	template<typename T>
	T* addSceneBehavior(std::unique_ptr<T> behavior) {
		T* ptr = dynamic_cast<T*>(behavior.get());
		if (ptr == nullptr) {
			throw std::runtime_error("failed to add scene behavior: invalid type cast!");
		}
		m_sceneBehaviors.push_back(std::move(behavior));
		return ptr;
	}

	/// <summary>
	/// Get a scene behavior of a specific type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	T* getSceneBehavior() {
		for (const auto& behavior : m_sceneBehaviors) {
			T* casted = dynamic_cast<T*>(behavior.get());
			if (casted != nullptr) {
				return casted;
			}
		}
		return nullptr;
	}
};