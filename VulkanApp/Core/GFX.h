#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include "Scene.h"
#include "Sprite.h"
#include "Model.h"
#include <map>
#include <type_traits>  

struct GFXService {
	void* ptr;
};

class GFX
{
private:
	GFX() = default;
	~GFX() = default;
	std::map<std::string, GFXService> m_services;

public:
	/// <summary>
	/// Singleton instance accessor
	/// </summary>
	/// <returns></returns>
	static GFX& instance()
	{
		static GFX instance;
		return instance;
	}

	/// <summary>
	/// Registers a service with the GFX service locator
	/// </summary>
	/// <param name="name"></param>
	/// <param name="servicePtr"></param>
	void registerService(const std::string& name, void* servicePtr)
	{
		GFXService service;
		service.ptr = servicePtr;
		m_services[name] = service;
	}

	/// <summary>
	/// Gets a service from the GFX service locator
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	void* getService(const std::string& name)
	{
		if (m_services.find(name) != m_services.end()) {
			return m_services[name].ptr;
		}
		return nullptr;
	}

	/// <summary>
	/// Gets a service from the GFX service locator and casts it to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* getServiceAs(const std::string& name)
	{
		void* servicePtr = getService(name);
		return static_cast<T*>(servicePtr);
	}

	/// <summary>
	/// Factory method to create a window
	/// </summary>
	/// <param name="wName"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	static GLFWwindow* createWindow(std::string wName = "GFX C++", const int width = 800, const int height = 600);

	/// <summary>
	/// Returns the current GLFW window context
	/// </summary>
	/// <returns></returns>
	static GLFWwindow* getCurrentWindow();

	// Delete copy and move constructors and assign operators
	GFX(const GFX&) = delete;
	GFX& operator=(const GFX&) = delete;
	GFX(GFX&&) = delete;
	GFX& operator=(GFX&&) = delete;
};

