#include "Renderer.h"
#include <algorithm>

void Renderer::createValidationLayers()
{
	m_validationLayers.push_back("VK_LAYER_KHRONOS_validation");
	m_enableValidationLayers = true;
}

/// <summary>
/// Create a Vulkan instance
/// </summary>
void Renderer::createInstance()
{
	// Check if the requestet validation layers are supported
	if (m_enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}
	else {
		printf("[GFX]: Validation layers are enabled.\n");
	}

	// Setup the application info
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "GFX";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "GFX 6";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	// Creation info for a Vulkan instance
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Get required extensions from GLFW
	std::vector<const char*> extensions;
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	// Add required extension from GLFW
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (size_t i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	// Check if extensions are supported
	if (!checkInstanceExtensionSupport(&extensions)) {
		throw std::runtime_error("Vulkan extensions not supported!");
	}

	// Pass extensions to the instance creation info
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	
	// Validation layers
	if (m_enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
		createInfo.ppEnabledLayerNames = m_validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
	}

	// Create the Vulkan instance
	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
	else {
		std::cout << "Vulkan Instance created successfully!" << std::endl;
	}
}

/// <summary>
/// Get a physical device (GPU) that supports Vulkan
/// </summary>
void Renderer::getPhysicalDevice()
{
	// Get the number of physical devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	// Check if there are any devices with Vulkan support
	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	// Get the physical devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

	// Pick the first suitable device
	for (const auto& device : devices) {
		if (checkDeviceSuitable(device)) {
			m_renderDevice.physicalDevice = device;
			break;
		}
	}

	// Check if a suitable device was found
	if (m_renderDevice.physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
	else {
		std::cout << "Physical device selected successfully!" << std::endl;
	}

	// Get the properties of the physical device
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(m_renderDevice.physicalDevice, &deviceProperties);
	//m_minUniformBufferOffset = deviceProperties.limits.minUniformBufferOffsetAlignment;
	std::cout << "Selected GPU: " << deviceProperties.deviceName << std::endl;
}

void Renderer::createLogicalDevice()
{
	// Get the queue family indices for the physical device
	QueueFamilyIndices indices = getQueueFamilies(m_renderDevice.physicalDevice);

	// Specify the queues to be created
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentationFamily };
	for (int queueFamilyIndex : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = 1;
		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Specify device features (none for now) TODO: Specify device features
	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE; // Enable anisotropic filtering TODO Add 

	// Create the logical device
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	// Create the logical device
	if (vkCreateDevice(m_renderDevice.physicalDevice, &createInfo, nullptr, &m_renderDevice.logicalDevice) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}
	else {
		std::cout << "Logical device created successfully!" << std::endl;
	}

	// Get the graphics queue
	vkGetDeviceQueue(m_renderDevice.logicalDevice, indices.graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_renderDevice.logicalDevice, indices.presentationFamily, 0, &m_presentQueue);
}

/// <summary>
/// Create a window surface for rendering
/// </summary>
void Renderer::createSurface()
{
	if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) == VK_SUCCESS) {
		std::cout << "Window surface created successfully!" << std::endl;
	}
	else {
		throw std::runtime_error("failed to create window surface!");
	}
}

void Renderer::createSwapChain()
{
	// Get swap chain support details from the physical device
	SwapChainSupportDetails swapChainDetails = getSwapChainSupport(m_renderDevice.physicalDevice);

	// Get the best settings for the swap chain
	VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormat(swapChainDetails.formats);
	VkPresentModeKHR presentMode = chooseBestPresentationMode(swapChainDetails.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainDetails.capabilities);

	uint32_t imageCount = swapChainDetails.capabilities.minImageCount + 1;
	if (swapChainDetails.capabilities.maxImageCount > 0 && swapChainDetails.capabilities.maxImageCount < imageCount) {
		imageCount = swapChainDetails.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.presentMode = presentMode;
	createInfo.imageExtent = extent;
	createInfo.minImageCount = imageCount;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = swapChainDetails.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.clipped = VK_TRUE;

	// Get the queue family indices
	QueueFamilyIndices indices = getQueueFamilies(m_renderDevice.physicalDevice);
	if (!indices.gpShared()) {
		// If not shared, we need to specify the queue family indices
		uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentationFamily };
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		// If shared, we can use exclusive mode
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_renderDevice.logicalDevice, &createInfo, nullptr, &m_swapChain) == VK_SUCCESS) {
		std::cout << "Swap chain created successfully!" << std::endl;
	}
	else {
		throw std::runtime_error("failed to create swap chain!");
	}

	m_swapChainImageFormat = surfaceFormat.format;
	m_swapChainExtent = extent;

	// Get the swap chain images
	uint32_t swapChainImageCount;
	vkGetSwapchainImagesKHR(m_renderDevice.logicalDevice, m_swapChain, &swapChainImageCount, nullptr);
	
	std::vector<VkImage> images(swapChainImageCount);
	vkGetSwapchainImagesKHR(m_renderDevice.logicalDevice, m_swapChain, &swapChainImageCount, images.data());

	for (VkImage image : images) {
		SwapChainImage swapChainImage = {};
		swapChainImage.image = image;
		swapChainImage.imageView = createImageView(m_renderDevice.logicalDevice, image, m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		m_swapChainImages.push_back(swapChainImage);
	}

}

void Renderer::createRenderPass()
{
	// Color attachment
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = m_swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;						// No multisampling
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;					// Clear the framebuffer to a color at the start of the render pass
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;					// Rendered contents will be stored in memory and can be read later
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;		// We don't use stencil buffer
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;		// We don't use stencil buffer
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;				// We don't care about the initial layout of the image (Input Layout)
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;			// Image will be presented in the swap chain (Output Layout)

	// Color attachment reference
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;										// This refers to the index of the attachment in the pAttachments array
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;	// Optimal layout for a color attachment

	// Depth attachment
	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = m_depthBufferFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Depth attachment reference
	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Subpass
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;			// This subpass is a graphics subpass
	subpass.colorAttachmentCount = 1;										// Number of color attachments
	subpass.pColorAttachments = &colorAttachmentRef;						// Reference to the color attachment
	subpass.pDepthStencilAttachment = &depthAttachmentRef;					// Reference to the depth attachment

	// Implicit layout transitions
	std::array<VkSubpassDependency, 2> dependencies;
	
	// Conversion from VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	// Transition must happen after ...
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	// But must happen before ...
	dependencies[0].dstSubpass = 0;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = 0;


	// Conversion from VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	// Transition must happen after ...
	dependencies[1].srcSubpass = 0;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	// But must happen before ...
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = 0;

	// Create the render pass
	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	if (vkCreateRenderPass(m_renderDevice.logicalDevice, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
	else {
		std::cout << "Render pass created successfully!" << std::endl;
	}
}

void Renderer::createDescriptorSetLayout()
{
	// UNIFORM VALUES DESCRIPTOR SET LAYOUT
	VkDescriptorSetLayoutBinding vpLayoutBinding = {};
	vpLayoutBinding.binding = 0;
	vpLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vpLayoutBinding.descriptorCount = 1;
	vpLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	vpLayoutBinding.pImmutableSamplers = nullptr;

	/*VkDescriptorSetLayoutBinding modelLayoutBinding = {};
	modelLayoutBinding.binding = 1;
	modelLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	modelLayoutBinding.descriptorCount = 1;
	modelLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	modelLayoutBinding.pImmutableSamplers = nullptr;*/

	std::array<VkDescriptorSetLayoutBinding, 1> bindings = { vpLayoutBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(m_renderDevice.logicalDevice, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create uniform descriptor set layout!");
	}
	else {
		std::cout << "Descriptor set layout created successfully!" << std::endl;
	}

	// SAMPLER DESCRIPTOR SET LAYOUT
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 0;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo samplerLayoutInfo = {};
	samplerLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	samplerLayoutInfo.bindingCount = 1;
	samplerLayoutInfo.pBindings = &samplerLayoutBinding;

	if (vkCreateDescriptorSetLayout(m_renderDevice.logicalDevice, &samplerLayoutInfo, nullptr, &m_samplerSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create sampler descriptor set layout!");
	}
	else {
		std::cout << "Sampler descriptor set layout created successfully!" << std::endl;
	}
}

//void Renderer::createPushConstantRange()
//{
//	m_pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//	m_pushConstantRange.offset = 0;
//	m_pushConstantRange.size = sizeof(Model);
//}

void Renderer::createGraphicsPipeline(ShaderSourceCollection shaders)
{
	// TODO: Replace with params
	auto vertShaderCode = readFile(shaders.vert);
	auto fragShaderCode = readFile(shaders.frag);

	// Create shader modules
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	// Shader stage creation
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = shaders.entryPoint.c_str();

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = shaders.entryPoint.c_str();

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
	
	// Vertex input
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Viewport and scissor
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_swapChainExtent.width;
	viewport.height = (float)m_swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = m_swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// Dynamic state
	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	// Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	// Color blending
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	// Pipeline layout
	VkPushConstantRange pushConstantRange = {};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(Model);

	std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts = { m_descriptorSetLayout, m_samplerSetLayout };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	// Create the pipeline layout
	if (vkCreatePipelineLayout(m_renderDevice.logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	else {
		std::cout << "Pipeline layout created successfully!" << std::endl;
	}

	// Depth and stencil testing
	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	// Graphics pipeline creation

	// Pipeline creation info
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pDynamicState = nullptr; //&dynamicState; 
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = m_renderPass;
	pipelineInfo.subpass = 0;

	// Piplines derivatives
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Existing pipeline to derive from
	pipelineInfo.basePipelineIndex = -1; // Index of the pipeline to derive from

	if (vkCreateGraphicsPipelines(m_renderDevice.logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
	else {
		std::cout << "Graphics pipeline created successfully!" << std::endl;
	}

	vkDestroyShaderModule(m_renderDevice.logicalDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(m_renderDevice.logicalDevice, vertShaderModule, nullptr);
}

void Renderer::createDepthBufferImage()
{
	// Find the best supported depth format
	m_depthBufferFormat = chooseSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	// Create the depth image
	m_depthBufferImage = createImage(
		m_renderDevice.physicalDevice,
		m_renderDevice.logicalDevice,
		m_swapChainExtent.width,
		m_swapChainExtent.height,
		m_depthBufferFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&m_depthBufferImageMemory);

	// Create the depth image view
	m_depthBufferImageView = createImageView(m_renderDevice.logicalDevice, m_depthBufferImage, m_depthBufferFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Renderer::createFramebuffers()
{
	m_swapChainFramebuffers.resize(m_swapChainImages.size());
	for (size_t i = 0; i < m_swapChainImages.size(); i++) {
		std::array<VkImageView, 2> attachments = {
			m_swapChainImages[i].imageView,
			m_depthBufferImageView
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data(); // must match the layout of the render pass e.g. color attachment is first attachment
		framebufferInfo.width = m_swapChainExtent.width;
		framebufferInfo.height = m_swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(m_renderDevice.logicalDevice, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void Renderer::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = getQueueFamilies(m_renderDevice.physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Allow command buffers to be rerecorded individually
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

	if (vkCreateCommandPool(m_renderDevice.logicalDevice, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void Renderer::createCommandBuffers()
{
	m_commandBuffers.resize(m_swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //  Primary can be submitted to a queue, secondary cannot and must be called from a primary command buffer
	allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

	if (vkAllocateCommandBuffers(m_renderDevice.logicalDevice, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void Renderer::createSyncObjects()
{
	// Set the number of frames in flight to the number of swap chain images
	m_numFramesInFlight = m_swapChainImages.size();
	m_imagesInFlight.resize(m_swapChainImages.size(), VK_NULL_HANDLE);

	m_imageAvailableSemaphores.resize(m_numFramesInFlight);
	m_renderFinishedSemaphores.resize(m_numFramesInFlight);
	m_inFlightFences.resize(m_numFramesInFlight);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Initialize the fence in a signaled state so that the first frame can be rendered without waiting

	for (size_t i = 0; i < m_numFramesInFlight; i++) {
		if (vkCreateSemaphore(m_renderDevice.logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(m_renderDevice.logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS || 
			vkCreateFence(m_renderDevice.logicalDevice, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores or fences for a frame!");
		}
	}
}

void Renderer::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(m_uboViewProjection);
	m_uniformBuffers.resize(m_swapChainImages.size());

	//VkDeviceSize dynamicBufferSize = m_modelUniformAlignment * MAX_OBJECTS;
	//m_dynamicUniformBuffers.resize(m_swapChainImages.size());

	for (size_t i = 0; i < m_swapChainImages.size(); i++) {
		m_uniformBuffers[i] = new UniformBuffer(m_renderDevice.physicalDevice, m_renderDevice.logicalDevice, bufferSize);
		//m_dynamicUniformBuffers[i] = new UniformBuffer(m_renderDevice.physicalDevice, m_renderDevice.logicalDevice, dynamicBufferSize);
	}
}

void Renderer::createDescriptorPool()
{
	// CREATE UNIFORM DESCRIPTOR POOL
	// Type of descriptors, not descriptor sets
	VkDescriptorPoolSize vpPoolSize = {};
	vpPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vpPoolSize.descriptorCount = static_cast<uint32_t>(m_uniformBuffers.size());

	// Dynamic uniform buffer pool size
	//VkDescriptorPoolSize dynamicPoolSize = {};
	//dynamicPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	//dynamicPoolSize.descriptorCount = static_cast<uint32_t>(m_dynamicUniformBuffers.size());

	std::array<VkDescriptorPoolSize, 1> poolSizes = { vpPoolSize };

	// Data to create the descriptor pool
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.maxSets = static_cast<uint32_t>(m_swapChainImages.size());
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();

	// Create the descriptor pool
	if (vkCreateDescriptorPool(m_renderDevice.logicalDevice, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create uniform descriptor pool!");
	}
	else {
		std::cout << "Descriptor pool created successfully!" << std::endl;
	}

	// CREATE SAMPLER DESCRIPTOR POOL
	VkDescriptorPoolSize samplerPoolSize = {};
	samplerPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerPoolSize.descriptorCount = MAX_OBJECTS; // TODO: NEED TO CHANGE WITH SOMETHING REGISTER TEXTURE BEFORE LOADING

	VkDescriptorPoolCreateInfo samplerPoolInfo = {};
	samplerPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	samplerPoolInfo.maxSets = MAX_OBJECTS; // TODO: NEED TO CHANGE WITH SOMETHING REGISTER TEXTURE BEFORE LOADING
	samplerPoolInfo.poolSizeCount = 1;
	samplerPoolInfo.pPoolSizes = &samplerPoolSize;

	if (vkCreateDescriptorPool(m_renderDevice.logicalDevice, &samplerPoolInfo, nullptr, &m_samplerDescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create sampler descriptor pool!");
	}
	else {
		std::cout << "Sampler descriptor pool created successfully!" << std::endl;
	}
}

void Renderer::createDescriptorSets()
{
	m_descriptorSets.resize(m_swapChainImages.size());

	std::vector<VkDescriptorSetLayout> layouts(m_swapChainImages.size(), m_descriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(m_swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(m_renderDevice.logicalDevice, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	// Update the descriptor set bindings for each swap chain image
	for (size_t i = 0; i < m_swapChainImages.size(); i++) {

		// VIEW PROJECTION UNIFORM BUFFER
		VkDescriptorBufferInfo vpBufferInfo = {};
		vpBufferInfo.buffer = m_uniformBuffers[i]->getUniformBuffer();
		vpBufferInfo.offset = 0;
		vpBufferInfo.range = m_uniformBuffers[i]->getBufferSize();

		VkWriteDescriptorSet vpDescriptorWrite = {};
		vpDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		vpDescriptorWrite.dstSet = m_descriptorSets[i];
		vpDescriptorWrite.dstBinding = 0;
		vpDescriptorWrite.dstArrayElement = 0;
		vpDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vpDescriptorWrite.descriptorCount = 1;
		vpDescriptorWrite.pBufferInfo = &vpBufferInfo;


		// MODEL UNIFORM BUFFER (DYNAMIC)
		//VkDescriptorBufferInfo modelBufferInfo = {};
		//modelBufferInfo.buffer = m_dynamicUniformBuffers[i]->getUniformBuffer();
		//modelBufferInfo.offset = 0; // Offset will be dynamic
		//modelBufferInfo.range = m_modelUniformAlignment; // Size of one model not the whole buffer :D

		//VkWriteDescriptorSet modelDescriptorWrite = {};
		//modelDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//modelDescriptorWrite.dstSet = m_descriptorSets[i];
		//modelDescriptorWrite.dstBinding = 1;
		//modelDescriptorWrite.dstArrayElement = 0;
		//modelDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		//modelDescriptorWrite.descriptorCount = 1;
		//modelDescriptorWrite.pBufferInfo = &modelBufferInfo;

		std::array<VkWriteDescriptorSet, 1> descriptorWrites = { vpDescriptorWrite };

		vkUpdateDescriptorSets(m_renderDevice.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void Renderer::createTextureSampler()
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;



	if (vkCreateSampler(m_renderDevice.logicalDevice, &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void Renderer::updateUniformBuffer(uint32_t currentImage)
{
	// Copy the view projection data to the uniform buffer
	void* data;
	vkMapMemory(m_renderDevice.logicalDevice, m_uniformBuffers[currentImage]->getUniformBufferMemory(), 0, m_uniformBuffers[currentImage]->getBufferSize(), 0, &data);
	memcpy(data, &m_uboViewProjection, sizeof(UboViewProjection));
	vkUnmapMemory(m_renderDevice.logicalDevice, m_uniformBuffers[currentImage]->getUniformBufferMemory());

	// Copy the model data to the dynamic uniform buffer
	//for (size_t i = 0; i < m_meshes.size(); i++) {
	//	Model* thisModel = (Model*)((uint64_t)m_modelTransferSpace + (i * m_modelUniformAlignment));
	//	*thisModel = m_meshes[i]->getModel();
	//}
	//vkMapMemory(m_renderDevice.logicalDevice, m_dynamicUniformBuffers[currentImage]->getUniformBufferMemory(), 0, m_modelUniformAlignment * m_meshes.size(), 0, &data);
	//memcpy(data, m_modelTransferSpace, m_modelUniformAlignment * m_meshes.size());
	//vkUnmapMemory(m_renderDevice.logicalDevice, m_dynamicUniformBuffers[currentImage]->getUniformBufferMemory());
}

void Renderer::recordCommands(uint32_t currentImage)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = m_renderPass;
	renderPassBeginInfo.renderArea.offset = { 0, 0 };
	renderPassBeginInfo.renderArea.extent = m_swapChainExtent;

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil.depth = 1.0f;

	renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassBeginInfo.pClearValues = clearValues.data();

	renderPassBeginInfo.framebuffer = m_swapChainFramebuffers[currentImage];
	VkResult result = vkBeginCommandBuffer(m_commandBuffers[currentImage], &beginInfo);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	vkCmdBeginRenderPass(m_commandBuffers[currentImage], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(m_commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

	// Draw the meshes
	for (size_t j = 0; j < m_meshes.size(); j++) {
		auto mesh = m_meshes[j];
		auto vertexBuffer = m_vertexBuffers[mesh->vertexBufferIndex];
		auto indexBuffer = m_indexBuffers[mesh->indexBufferIndex];

		VkBuffer vertexBuffers[] = { vertexBuffer->getVertexBuffer()};
		VkDeviceSize offsets[] = { 0 };
		uint32_t indexCount = static_cast<uint32_t>(indexBuffer->getIndexCount());

		std::array<VkDescriptorSet, 2> descriptorSets = { 
			m_descriptorSets[currentImage], 
			m_samplerDescriptorSets[mesh->textureIndex]
		};

		//uint32_t dynamicOffset = static_cast<uint32_t>(m_modelUniformAlignment) * j;
		vkCmdBindDescriptorSets(m_commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout,
			0, static_cast<uint32_t>(descriptorSets.size()),	descriptorSets.data(), 0, nullptr);

		vkCmdPushConstants(m_commandBuffers[currentImage], m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Model), &m_meshes[j]->getModel());
		vkCmdBindVertexBuffers(m_commandBuffers[currentImage], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(m_commandBuffers[currentImage], indexBuffer->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(m_commandBuffers[currentImage], indexCount, 1, 0, 0, 0);
	}
	//vkCmdDraw(m_commandBuffers[i], static_cast<uint32_t>(m_vertexBuffer->getVertexCount()), 1, 0, 0); // Draw a triangle with 3 vertices, 1 instance, first vertex 0, first instance 0
	vkCmdEndRenderPass(m_commandBuffers[currentImage]);

	result = vkEndCommandBuffer(m_commandBuffers[currentImage]);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}


/// <summary>
/// Check if the requested validation layers are supported
/// </summary>
/// <returns></returns>
bool Renderer::checkValidationLayerSupport()
{
	// Get the number of available layers
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	// Get the available layers
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Check if all requested layers are available
	for (const char* layerName : m_validationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}
	return true;
}

/// <summary>
/// Check if the required instance extensions are supported
/// </summary>
/// <param name="checkExtensions"></param>
/// <returns></returns>
bool Renderer::checkInstanceExtensionSupport(const std::vector<const char*>* checkExtensions)
{
	// Get the number of extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// Get the extensions
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

	for (const auto& checkExtension : *checkExtensions) {
		bool found = false;
		for (const auto& extension : availableExtensions) {
			if (strcmp(checkExtension, extension.extensionName)) {
				found = true;
				break;
			}
		}

		if (!found) {
			return false;
		}
	}

	return true;
}

bool Renderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	// Get the number of device extensions
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	// Check if there are any extensions
	if (extensionCount == 0) {
		return false;
	}

	// Get the device extensions
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	// Check if all required extensions are available
	for (const auto& requiredExtension : deviceExtensions) {
		bool hasExtension = false;
		for (const auto& extension : availableExtensions) {
			if (strcmp(requiredExtension, extension.extensionName) == 0) {
				hasExtension = true;
				break;
			}
		}
		if (!hasExtension) {
			return false;
		}
	}

	return true;
}

/// <summary>
/// Check if a physical device is suitable for the application
/// </summary>
/// <param name="device"></param>
/// <returns></returns>
bool Renderer::checkDeviceSuitable(VkPhysicalDevice device)
{
	//// Device Information
	//VkPhysicalDeviceProperties deviceProperties;
	//vkGetPhysicalDeviceProperties(device, &deviceProperties);

	// Device Features
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	QueueFamilyIndices indices = getQueueFamilies(device);

	// Check for device extension support
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	// Check if swapchain is valid
	SwapChainSupportDetails swapChainDetails = getSwapChainSupport(device);
	bool swapchainValid = swapChainDetails.isValid();

 	return indices.isValid() && extensionsSupported && swapchainValid && deviceFeatures.samplerAnisotropy;
}

/// <summary>
/// Get the queue family indices for a given physical device
/// </summary>
/// <param name="device"></param>
/// <returns></returns>
QueueFamilyIndices Renderer::getQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;
	
	// Get the number of queue families
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	// Get the queue families
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// Find suitable queue families
	for (size_t i = 0; i < queueFamilies.size(); i++) {
		auto& queueFamily = queueFamilies[i];

		// Check if quefamily is an graphics family
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = static_cast<int>(i);
		}

		// Check if quefamily support presentation
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentationSupport);
		if (queueFamily.queueCount > 0 && presentationSupport) {
			indices.presentationFamily = i;
		}

		if (indices.isValid()) {
			break;
		}
	}

	return indices;
}

SwapChainSupportDetails Renderer::getSwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	
	// Get the surface capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

	// Get the supported surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
	}

	// Get the supported presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

/// <summary>
/// Choose the best surface format
/// Preferably VK_FORMAT_R8G8B8A8_UNORM with VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
/// </summary>
/// <param name="availableFormats"></param>
/// <returns></returns>
VkSurfaceFormatKHR Renderer::chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_R8G8B8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
		else if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

/// <summary>
/// Choose the best presentation mode
/// Preferably VK_PRESENT_MODE_MAILBOX_KHR, else VK_PRESENT_MODE_FIFO_KHR
/// </summary>
/// <param name="availablePresentModes"></param>
/// <returns></returns>
VkPresentModeKHR Renderer::chooseBestPresentationMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(m_window, &width, &height);
		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		// Clamp the values to the allowed extents TODO: Look into it
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

VkFormat Renderer::chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : formats) {
		// Get the properties of the format
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(m_renderDevice.physicalDevice, format, &props);

		// Depending on the tiling, need to check for different bit flag
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	std::runtime_error("failed to find supported format!");	
}

/// <summary>
/// Create a shader module from SPIR-V code
/// </summary>
/// <param name="code"></param>
/// <returns></returns>
VkShaderModule Renderer::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(m_renderDevice.logicalDevice, &createInfo, nullptr, &shaderModule) == VK_SUCCESS) {
		return shaderModule;
	}
	else {
		throw std::runtime_error("failed to create shader module!");
	}
}

int Renderer::createTextureDescriptor(VkImageView textureImageView)
{
	// Allocate descriptor set from the sampler descriptor pool
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_samplerDescriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &m_samplerSetLayout;

	VkDescriptorSet descriptorSet;
	if (vkAllocateDescriptorSets(m_renderDevice.logicalDevice, &allocInfo, &descriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate texture descriptor set!");
	}

	// Texture image info
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = textureImageView;
	imageInfo.sampler = m_textureSampler;

	// Descriptor Write info
	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfo;

	// Update the descriptor set with the image info
	vkUpdateDescriptorSets(m_renderDevice.logicalDevice, 1, &descriptorWrite, 0, nullptr);

	// Store the descriptor set
	m_samplerDescriptorSets.push_back(descriptorSet);

	// Return the index of the descriptor set
	return m_samplerDescriptorSets.size() - 1;
}

//void Renderer::allocateDynamicBufferTransferSpace()
//{
//	// Calculate alignment of model data
//	m_modelUniformAlignment = (sizeof(Model) + m_minUniformBufferOffset - 1)
//		& ~(m_minUniformBufferOffset - 1);
//
//	// Create space in memory to hold dynamic buffer that is aligned to our required alignment and holds MAX_OBJECTS
//	m_modelTransferSpace = (Model*)_aligned_malloc(m_modelUniformAlignment * MAX_OBJECTS, m_modelUniformAlignment);
//}


/// <summary>
/// Initialize the renderer
/// </summary>
/// <param name="window"></param>
/// <returns></returns>
int Renderer::init(GLFWwindow* window)
{
	m_window = window;
	try {
		// Create validation layers for the instance
		createValidationLayers();

		// Create the Vulkan instance
		createInstance();
		createSurface();
		getPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createDepthBufferImage();
		createRenderPass();
		createDescriptorSetLayout();
		// TODO: make createGraphicsPipeline return the pipeline and store it then in m_graphicsPipeline
		ShaderSourceCollection shaders = {};
		shaders.vert = "Shaders/vert.spv";
		shaders.frag = "Shaders/frag.spv";
		createGraphicsPipeline(shaders);
		createFramebuffers();
		createCommandPool();
		createCommandBuffers();
		createTextureSampler();
		//allocateDynamicBufferTransferSpace();
		createUniformBuffers();
		createDescriptorPool();
		createDescriptorSets();
		createSyncObjects();

		// Load test textures
		m_uboViewProjection.projection = glm::perspective(glm::radians(45.0f), m_swapChainExtent.width / (float)m_swapChainExtent.height, 0.1f, 10.0f);
		m_uboViewProjection.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_uboViewProjection.projection[1][1] *= -1;

	}
	catch (const std::runtime_error& e) {
		printf("Failed to create instance: %s\n", e.what());
		return EXIT_FAILURE;
	}

    return 0;
}

void Renderer::addImageTexture(ImageTexture* imageTexture)
{
	imageTexture->init(m_renderDevice.physicalDevice, m_renderDevice.logicalDevice, m_graphicsQueue, m_commandPool);
	imageTexture->descriptorIndex = createTextureDescriptor(imageTexture->imageView);
	m_imageTextures.push_back(imageTexture);
}

int Renderer::createVertexBuffer(std::vector<Vertex>* vertices)
{
	VertexBuffer* vertexBuffer = new VertexBuffer(m_renderDevice.physicalDevice, m_renderDevice.logicalDevice, m_graphicsQueue, m_commandPool, vertices);
	m_vertexBuffers.push_back(vertexBuffer);
	return m_vertexBuffers.size() - 1;
}

int Renderer::createIndexBuffer(std::vector<uint32_t>* indices)
{
	IndexBuffer* indexBuffer = new IndexBuffer(m_renderDevice.physicalDevice, m_renderDevice.logicalDevice, m_graphicsQueue, m_commandPool, indices);
	m_indexBuffers.push_back(indexBuffer);
	return m_indexBuffers.size() - 1;
}

void Renderer::addMesh(Mesh* mesh)
{
	m_meshes.push_back(mesh);
}

void Renderer::draw()
{
	// Wait for the previous frame to finish
	vkWaitForFences(m_renderDevice.logicalDevice, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	// Setp 1: Get the next image from the swap chain
	uint32_t imageIndex;
	vkAcquireNextImageKHR(m_renderDevice.logicalDevice, m_swapChain, std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);
	
	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(m_renderDevice.logicalDevice, 1, &m_imagesInFlight[imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());
	}
	m_imagesInFlight[imageIndex] = m_inFlightFences[m_currentFrame];
	vkResetFences(m_renderDevice.logicalDevice, 1, &m_inFlightFences[m_currentFrame]);

	// Record command buffer for this image
	recordCommands(imageIndex);
	updateUniformBuffer(imageIndex);

	// Step 2: Submit the command buffer from the image to the graphics queue
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;											// Number of semaphores to wait on before execution begins
	submitInfo.pWaitSemaphores = &m_imageAvailableSemaphores[m_currentFrame];	// Semaphores to wait on before execution begins
	VkPipelineStageFlags waitStages[] = { 
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT 
	};
	submitInfo.pWaitDstStageMask = waitStages;									// Stages to wait at
	submitInfo.commandBufferCount = 1;											// Number of command buffers to be submitted
	submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];					// Command buffers to submit
	submitInfo.signalSemaphoreCount = 1;										// Number of semaphores to signal once the command buffer has finished execution
	submitInfo.pSignalSemaphores = &m_renderFinishedSemaphores[m_currentFrame];	// Semaphores to signal once the command buffer has finished execution

	if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	// Step 3: Present the image in the swap chain to the presentation queue
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;											// Number of semaphores to wait on before presentation
	presentInfo.pWaitSemaphores = &m_renderFinishedSemaphores[m_currentFrame];	// Semaphores to wait on before presentation
	presentInfo.swapchainCount = 1;												// Number of swap chains to present images to
	presentInfo.pSwapchains = &m_swapChain;										// Swap chains to present images to
	presentInfo.pImageIndices = &imageIndex;									// Indices of the images in the swap chains to present

	if (vkQueuePresentKHR(m_presentQueue, &presentInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	m_currentFrame = (m_currentFrame + 1) % m_numFramesInFlight;
}

/// <summary>
/// Dispose the renderer and free resources
/// </summary>
void Renderer::dispose()
{
	vkDeviceWaitIdle(m_renderDevice.logicalDevice);

	// DESTROY TEXTURE DESCRIPTORS
	vkDestroyDescriptorPool(m_renderDevice.logicalDevice, m_samplerDescriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(m_renderDevice.logicalDevice, m_samplerSetLayout, nullptr);

	// DESTROY SAMPLER
	vkDestroySampler(m_renderDevice.logicalDevice, m_textureSampler, nullptr);

	// FREE TEXTURES
	for (ImageTexture* imageTexture : m_imageTextures) {
		imageTexture->dispose(m_renderDevice.logicalDevice);
		delete imageTexture;
	}
	m_imageTextures.clear();

	//_aligned_free(m_modelTransferSpace);
	vkDestroyImageView(m_renderDevice.logicalDevice, m_depthBufferImageView, nullptr);
	vkDestroyImage(m_renderDevice.logicalDevice, m_depthBufferImage, nullptr);
	vkFreeMemory(m_renderDevice.logicalDevice, m_depthBufferImageMemory, nullptr);

	vkDestroyDescriptorPool(m_renderDevice.logicalDevice, m_descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(m_renderDevice.logicalDevice, m_descriptorSetLayout, nullptr);
	// Free uniform buffers
	for (auto uniformBuffer : m_uniformBuffers) {
		uniformBuffer->dispose();
		delete uniformBuffer;
	}
	m_uniformBuffers.clear();

	for (auto dynamicUniformBuffer : m_dynamicUniformBuffers) {
		dynamicUniformBuffer->dispose();
		delete dynamicUniformBuffer;
	}
	m_dynamicUniformBuffers.clear();

	// Free meshes
	for (auto mesh : m_meshes) {

		mesh->dispose();
		delete mesh;
	}
	m_meshes.clear();

	// Free vertex buffers
	for (auto vertexBuffer : m_vertexBuffers) {
		vertexBuffer->dispose();
		delete vertexBuffer;
	}
	m_vertexBuffers.clear();

	// Free index buffers
	for (auto indexBuffer : m_indexBuffers) {
		indexBuffer->dispose();
		delete indexBuffer;
	}
	m_indexBuffers.clear();

	for (size_t i = 0; i < m_numFramesInFlight; i++) {
		vkDestroySemaphore(m_renderDevice.logicalDevice, m_renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(m_renderDevice.logicalDevice, m_imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(m_renderDevice.logicalDevice, m_inFlightFences[i], nullptr);
	}
	vkDestroyCommandPool(m_renderDevice.logicalDevice, m_commandPool, nullptr);
	for (auto framebuffer : m_swapChainFramebuffers) {
		vkDestroyFramebuffer(m_renderDevice.logicalDevice, framebuffer, nullptr);
	}
	vkDestroyPipeline(m_renderDevice.logicalDevice, m_graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(m_renderDevice.logicalDevice, m_pipelineLayout, nullptr);
	vkDestroyRenderPass(m_renderDevice.logicalDevice, m_renderPass, nullptr);
	for (auto& image : m_swapChainImages) {
		vkDestroyImageView(m_renderDevice.logicalDevice, image.imageView, nullptr);
	}
	vkDestroySwapchainKHR(m_renderDevice.logicalDevice, m_swapChain, nullptr);
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyDevice(m_renderDevice.logicalDevice, nullptr);
	vkDestroyInstance(m_instance, nullptr);
}

Renderer::~Renderer()
{

}
