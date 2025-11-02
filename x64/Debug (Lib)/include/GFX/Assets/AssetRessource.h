#pragma once
#include <string>
#include "../Graphics/Renderer.h"

/// <summary>
/// Base class for all asset resources
/// </summary>
class AssetRessource
{
public:
	virtual ~AssetRessource() = default;

	/// <summary>
	/// Initialize the asset for rendering
	/// </summary>
	/// <param name="renderer"></param>
	virtual void init(Renderer* renderer) = 0;

	/// <summary>
	/// Dispose of the asset and free resources
	/// </summary>
	virtual void dispose(Renderer* renderer) = 0;
};

