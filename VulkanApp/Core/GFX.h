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
#include <vector>

struct ServiceData {
	void* ptr;
};

class GFX
{
private:
	GFX() = default;
	~GFX() = default;

	/// <summary>
	/// Services registered with GFX
	/// GFX takes NO ownership of the services
	/// its just a registry
	/// </summary>
	std::map < std::string, ServiceData> m_services;

	/// <summary>
	/// Window contexts managed by GFX
	/// </summary>
	std::vector<GLFWwindow*> m_windows;

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
	/// Registers a service with GFX
	/// </summary>
	/// <param name="name"></param>
	/// <param name="servicePtr"></param>
	void registerAsService(const std::string& name, void* servicePtr)
	{
		m_services[name] = ServiceData{ servicePtr };
	}

	/// <summary>
	/// Gets a registered service from GFX
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* getService(const std::string& name)
	{
		auto it = m_services.find(name);
		if (it != m_services.end()) {
			return static_cast<T*>(it->second.ptr);
		}
		return nullptr;
	}

	/// <summary>
	/// Checks if a service is registered with GFX
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool hasService(const std::string& name)
	{
		return m_services.find(name) != m_services.end();
	}

	/// <summary>
	/// Removes a registered service from GFX
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool removeService(const std::string& name)
	{
		return m_services.erase(name) > 0;
	}

	/// <summary>
	/// Shutdown all registered services
	/// </summary>
	/// <returns></returns>
	bool shutdownServices()
	{
		bool result = true;

		for (auto& [name, service] : m_services) {
			auto servicePtr = service.ptr;
			if (servicePtr != nullptr) {
				std::cerr << "Service '" << name << "' was not properly disposed before shutdown." << std::endl;
				result = false;
			}
		}
		m_services.clear();
		return result;
	}

	/// <summary>
	/// Factory method to create a window
	/// </summary>
	/// <param name="wName"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	GLFWwindow* createWindow(std::string wName = "GFX C++", const int width = 800, const int height = 600);

	/// <summary>
	/// Returns the current GLFW window context
	/// </summary>
	/// <returns></returns>
	GLFWwindow* getWindow(int index);

	/// <summary>
	/// Destroys all created windows
	/// </summary>
	void destroyWindows()
	{
		for (auto window : m_windows) {
			glfwDestroyWindow(window);
		}
		m_windows.clear();
	}

	/// <summary>
	/// Destroys a specific window
	/// </summary>
	/// <param name="window"></param>
	void destroyWindow(GLFWwindow* window)
	{
		auto it = std::find(m_windows.begin(), m_windows.end(), window);
		if (it != m_windows.end()) {
			glfwDestroyWindow(window);
			m_windows.erase(it);
		}
	}

	// Delete copy and move constructors and assign operators
	GFX(const GFX&) = delete;
	GFX& operator=(const GFX&) = delete;
	GFX(GFX&&) = delete;
	GFX& operator=(GFX&&) = delete;
};

