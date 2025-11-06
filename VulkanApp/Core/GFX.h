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
#include <any>

struct ServiceData {
	std::any data;
	bool isPointer; // true if it's a unique_ptr, false if it's a value
};


class GFX
{
private:
	GFX() = default;
	~GFX() = default;

	/// <summary>
	/// Services registered with GFX
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
	/// Register service with OWNERSHIP (for objects)
	/// takes ownership of the service pointer
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <param name="service"></param>
	template<typename T>
	void registerServicePtr(const std::string& name, std::unique_ptr<T> service)
	{
		ServiceData data;
		data.data = std::move(service);
		data.isPointer = true;
		m_services[name] = std::move(data);
	}

	/// <summary>
	/// Register service with OWNERSHIP and return raw pointer (for objects)
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <param name="service"></param>
	/// <returns></returns>
	template<typename T>
	T* registerServicePtr(const std::string& name, std::unique_ptr<T> service) {
		registerServicePtr<T>(name, std::move(service));
		return getService<T>(name);
	}

	/// <summary>
	/// Register service VALUE (for primitive types)
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <param name="service"></param>
	template<typename T>
	void registerServiceValue(const std::string& name, T service)
	{
		ServiceData data;
		data.data = service;
		data.isPointer = false;
		m_services[name] = std::move(data);
	}

	/// <summary>
	/// Gets a service from the GFX service locator
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* getService(const std::string& name)
	{
		auto it = m_services.find(name);
		if (it != m_services.end() && it->second.isPointer) {
			try {
				auto& ptr = std::any_cast<std::unique_ptr<T>&>(it->second.data);
				return ptr.get();
			}
			catch (const std::bad_any_cast&) {
				return nullptr;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Gets a service value from the GFX service locator
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <param name="defaultValue"></param>
	/// <returns></returns>
	template<typename T>
	T getServiceValue(const std::string& name, const T& defaultValue = T{}) {
		auto it = m_services.find(name);
		if (it != m_services.end() && !it->second.isPointer) {
			try {
				return std::any_cast<T>(it->second.data);
			}
			catch (const std::bad_any_cast&) {
				return defaultValue;
			}
		}
		return defaultValue;
	}

	/// <summary>
	/// Gets a reference to a service value from the GFX service locator
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* getServiceValueRef(const std::string& name) {
		auto it = m_services.find(name);
		if (it != m_services.end() && !it->second.isPointer) {
			try {
				return &std::any_cast<T&>(it->second.data);
			}
			catch (const std::bad_any_cast&) {
				return nullptr;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Checks if a service is registered
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool hasService(const std::string& name) {
		return m_services.find(name) != m_services.end();
	}

	/// <summary>
	/// Unregisters a service
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool unregisterService(const std::string& name) {
		auto it = m_services.find(name);
		if (it != m_services.end()) {
			m_services.erase(it);
			return true;
		}
		return false;
	}

	/// <summary>
	/// Clears all registered services
	/// </summary>
	void clearServices() {
		m_services.clear();
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

