#pragma once
#include <fstream>


/// <summary>
/// Required device extensions
/// </summary>
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


/// <summary>
/// Indies (location) of queue families
/// </summary>
struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentationFamily = -1;

	bool isValid() {
		return graphicsFamily >= 0 && presentationFamily >= 0;
	}

	bool gpShared() {
		return graphicsFamily == presentationFamily;
	}
};

/// <summary>
/// Swap chain support details
/// </summary>
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;

	bool isValid() {
		return !formats.empty() && !presentModes.empty();
	}
};

struct SwapChainImage {
	VkImage image;
	VkImageView imageView;
};

struct ShaderSourceCollection {
	std::string vert;
	std::string frag;
	std::string entryPoint = "main";
};

struct ShaderCollection {
	VkShaderModule vert;
	VkShaderModule frag;
};

static std::vector<char> readFile(const std::string& filename) 
{
	std::ifstream file;
	file.open(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file: " + filename);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}