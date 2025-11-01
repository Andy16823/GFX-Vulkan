#pragma once
#include "../Graphics/Renderer.h"
#include <map>
#include <string>
#include "AssetRessource.h"
#include "../Graphics/Renderer.h"

class AssetManager
{
private:
	std::map<std::string, std::unique_ptr<AssetRessource>> m_assets;
public:
	AssetManager() = default;
	~AssetManager() = default;

	/// <summary>
	/// Add an asset to the manager and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <param name="asset"></param>
	/// <returns></returns>
	template<typename T>
	T* addAsset(const std::string& name, std::unique_ptr<AssetRessource> asset) {
		m_assets[name] = std::move(asset);
		auto it = m_assets.find(name);
		if (it != m_assets.end()) {
			T* casted = dynamic_cast<T*>(it->second.get());
			if (!casted) {
				throw std::runtime_error("Failed to add asset: invalid type cast");
			}
			return casted;
		}
		return nullptr;
	}

	/// <summary>
	/// Add an asset to the manager by creating a new instance and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* addAsset(const std::string& name) {
		m_assets[name] = std::make_unique<T>();
		auto it = m_assets.find(name);
		if (it != m_assets.end()) {
			T* casted = dynamic_cast<T*>(it->second.get());
			if (!casted) {
				throw std::runtime_error("Failed to add asset: invalid type cast");
			}
			return casted;
		}
		return nullptr;
	}

	/// <summary>
	/// Get an asset by name and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* getAsset(const std::string& name) const {
		auto it = m_assets.find(name);
		if (it != m_assets.end()) {
			T* casted = dynamic_cast<T*>(it->second.get());
			if (!casted) {
				throw std::runtime_error("Failed to get asset: invalid type cast");
			}
			return casted;
		}
		return nullptr;
	}

	/// <summary>
	/// Initialize all assets for rendering
	/// </summary>
	/// <param name="renderer"></param>
	void init(Renderer* renderer);

	/// <summary>
	/// Dispose of all assets and free resources
	/// </summary>
	void dispose(Renderer* renderer);
};

