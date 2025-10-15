#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/// <summary>
/// Represents the state of a buffer
/// </summary>
enum BufferState
{
	GFX_BUFFER_STATE_NONE = 0,
	GFX_BUFFER_STATE_INITIALIZED = 1,
	GFX_BUFFER_STATE_DISPOSED = 2
};

/// <summary>
/// Base buffer class
/// </summary>
class Buffer
{
public:
	BufferState state = GFX_BUFFER_STATE_NONE;

	virtual void dispose(VkDevice device) = 0;
	virtual ~Buffer() = default;
};

