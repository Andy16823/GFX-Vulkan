#include "AssetManager.h"

void AssetManager::init(Renderer* renderer)
{
	for (const auto& [name, asset] : m_assets) {
		asset->init(renderer);
	}
}

void AssetManager::dispose(Renderer* renderer)
{
	for (const auto& [name, asset] : m_assets) {
		asset->dispose(renderer);
	}
}
