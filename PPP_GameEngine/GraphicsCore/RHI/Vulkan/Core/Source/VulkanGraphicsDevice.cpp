//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGraphicsDevice.cpp
///             @brief  GraphicsDevice
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanGraphicsDevice.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <set>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma comment (lib, "vulkan-1.lib")
using namespace rhi::vulkan;
#pragma warning(disable: 26812)

namespace 
{
	constexpr const char* s_DeviceName = "Graphics Device Vulkan";
	constexpr const char* s_EngineName = "PPP Engine";
	const std::vector<const char*> s_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> s_DeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}
	void     DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
	/****************************************************************************
	*				  			SwapchainSupportDetails
	*************************************************************************//**
	*  @class     SwapchainSupportDetails
	*  @brief     Swapchain support
	*****************************************************************************/
	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR         Capabilities;
		std::vector<VkSurfaceFormatKHR>  Formats;
		std::vector<VkPresentModeKHR>    PresentModes;

		static SwapchainSupportDetails Query(VkPhysicalDevice device, VkSurfaceKHR surface)
		{
			/*-------------------------------------------------------------------
			-               Acquire Surface Capabilities
			---------------------------------------------------------------------*/
			SwapchainSupportDetails details = {};
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);
			/*-------------------------------------------------------------------
			-               Acquire Surface Formats
			---------------------------------------------------------------------*/
			UINT32 formatCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
			if (formatCount != 0)
			{
				details.Formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());
			}
			/*-------------------------------------------------------------------
			-               Acquire Surface Formats
			---------------------------------------------------------------------*/
			UINT32 presentModeCount = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
			if (presentModeCount != 0)
			{
				details.PresentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
			}
			return details;
		}
	};
}
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GraphicsDeviceVulkan::~GraphicsDeviceVulkan()
{

}
void GraphicsDeviceVulkan::StartUp(HWND hwnd, HINSTANCE hInstance)
{
	_hwnd = hwnd; _hInstance = hInstance;
	CreateInstance();
	SetUpDebugMessenger();
	CreateSurface();
	SelectPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateDepthBuffer();
	CreateCommandPool();
	CreateFrameBuffer();
	CreateCommandBuffers();
	CreateSyncObjects();
}

void GraphicsDeviceVulkan::ShutDown()
{
	CleanUpSwapchain();
	vkDestroyDevice(_logicalDevice, nullptr); // destroy logical device
#ifdef _DEBUG
	DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
#endif
	vkDestroySurfaceKHR(_instance, _surface, nullptr);
	vkDestroyInstance(_instance, nullptr);

}

/****************************************************************************
*                     OnResize
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::OnResize()
*  @brief     recreate swapchain buffer
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::OnResize()
{
	CleanUpSwapchain();
	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateDepthBuffer();
	CreateFrameBuffer();
}

/****************************************************************************
*                     BeginDrawFrame
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::BeginDrawFrame()
*  @brief     Begin draw frame. This function is called when draw frame start.
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::BeginDrawFrame()
{
	UINT32 nextImageIndex = 0;
	if (vkAcquireNextImageKHR(_logicalDevice, _swapchain, UINT64_MAX, _presentCompletedSemaphores, VK_NULL_HANDLE, &nextImageIndex) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to acquire next image\n");
	}
	_imageIndex = nextImageIndex;

	/*-------------------------------------------------------------------
	-               WaitFence
	---------------------------------------------------------------------*/
	if (vkWaitForFences(_logicalDevice, 1, &_fences[nextImageIndex], VK_TRUE, UINT64_MAX) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to wait fence\n");
	}
	if (vkResetFences(_logicalDevice, 1, &_fences[nextImageIndex]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to reset fence\n");
	}

	/*-------------------------------------------------------------------
	-               Command render pass started 
	---------------------------------------------------------------------*/
	std::array<VkClearValue, 2> clearValue = {};
	clearValue[0].color        = { {0.5f,0.25f,0.25f,0.0f} };
	clearValue[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo beginInfo = {};
	beginInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.renderPass        = _renderPass;
	beginInfo.framebuffer       = _swapchainFrameBuffers[nextImageIndex];
	beginInfo.renderArea.offset = VkOffset2D{ 0,0 };
	beginInfo.renderArea.extent = _swapchainExtent;
	beginInfo.pClearValues      = clearValue.data();
	beginInfo.clearValueCount   = UINT32(clearValue.size());

	/*-------------------------------------------------------------------
	-               Command record start
	---------------------------------------------------------------------*/
	VkCommandBufferBeginInfo commandBeginInfo = {};
	commandBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkBeginCommandBuffer(_commandBuffers[nextImageIndex], &commandBeginInfo) != VK_SUCCESS) { throw std::runtime_error("failed to begin command buffer\n"); }
	vkCmdBeginRenderPass(_commandBuffers[nextImageIndex], &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

/****************************************************************************
*                     EndDrawFrame
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::EndDrawFrame()
*  @brief     Begin draw frame. This function is called when draw frame end.
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::EndDrawFrame()
{
	auto& command = _commandBuffers[_imageIndex];
	/*-------------------------------------------------------------------
	-               Command render pass finished
	---------------------------------------------------------------------*/
	vkCmdEndRenderPass(command);
	if (vkEndCommandBuffer(command) != VK_SUCCESS) { throw std::runtime_error("failed to record command buffer"); }
	/*-------------------------------------------------------------------
	-               Submit command to command queue
	---------------------------------------------------------------------*/
	VkSubmitInfo         submitInfo    = {};
	VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	submitInfo.sType                   = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount      = 1;
	submitInfo.pCommandBuffers         = &command;
	submitInfo.pWaitDstStageMask       = &waitStageMask;
	submitInfo.waitSemaphoreCount      = 1;
	submitInfo.pWaitSemaphores         = &_presentCompletedSemaphores;
	submitInfo.signalSemaphoreCount    = 1;
	submitInfo.pSignalSemaphores       = &_renderCompletedSemaphores;
	vkResetFences(_logicalDevice, 1, &_fences[_imageIndex]);
	auto result = vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _fences[_imageIndex]);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit queue");
	}
	/*-------------------------------------------------------------------
	-               Present 
	---------------------------------------------------------------------*/
	VkPresentInfoKHR presentInfo   = {};
	presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.swapchainCount     = 1;
	presentInfo.pSwapchains        = &_swapchain;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores    = &_renderCompletedSemaphores;
	presentInfo.pImageIndices      = &_imageIndex;
	
	result = vkQueuePresentKHR(_presentQueue, &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) // frame buffer resized)
	{
		OnResize();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image");
	}

	/*-------------------------------------------------------------------
	-               Go to next frame
	---------------------------------------------------------------------*/
	_currentFrame = (_currentFrame + 1) % FRAME_BUFFER_COUNT;
}
#pragma region Private Function
#pragma region Clean Up 
/****************************************************************************
*                     CleanUpSwapchain()
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CleanUpSwapchain()
*  @brief     Clean up swapchain (Called when OnResize is used.)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CleanUpSwapchain()
{
	/*-------------------------------------------------------------------
	-               Destroy Depth buffer resource
	---------------------------------------------------------------------*/
	vkDestroyImageView(_logicalDevice, _depthBufferView  , nullptr);
	vkDestroyImage    (_logicalDevice, _depthBuffer      , nullptr);
	vkFreeMemory      (_logicalDevice, _depthBufferMemory, nullptr);
	/*-------------------------------------------------------------------
	-               Destroy frame buffers
	---------------------------------------------------------------------*/
	for (auto frameBuffer : _swapchainFrameBuffers)
	{
		vkDestroyFramebuffer(_logicalDevice, frameBuffer, nullptr);
	}
	/*-------------------------------------------------------------------
	-               Destroy Render pass
	---------------------------------------------------------------------*/
	vkDestroyRenderPass(_logicalDevice, _renderPass, nullptr);
	for (auto imageView : _swapchainImageViews)
	{
		vkDestroyImageView(_logicalDevice, imageView, nullptr);
	}
	vkDestroySwapchainKHR(_logicalDevice, _swapchain, nullptr);
}
#pragma endregion Clean Up
#pragma region Set Up
/****************************************************************************
*                     CreateInstance
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::CreateInstance()
*  @brief     Create vkInstance.
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool GraphicsDeviceVulkan::CreateInstance()
{
	const char*              deviceName = s_DeviceName;
	const char*              engineName = s_EngineName;
	std::vector<const char*> extensions; // all extension name saved.

#ifdef _DEBUG
	if (!CheckValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available");
	}
#endif
	/*-------------------------------------------------------------------
	-               Set Application Infomation
	---------------------------------------------------------------------*/
	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO; // structure type
	applicationInfo.pApplicationName = deviceName;                         // application name
	applicationInfo.pEngineName      = engineName;                         // engine name
	applicationInfo.apiVersion       = _apiVersion;                        // newest version
	applicationInfo.engineVersion    = VK_MAKE_VERSION(1, 0, 0);           // engine version
	/*-------------------------------------------------------------------
	-               Acquire Extension Infomation (for debugging layer)
	---------------------------------------------------------------------*/
	std::vector<VkExtensionProperties> properties;
	{
		UINT32 count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);           // acqure property count
		properties.resize(count);                                                   // set buffer region 
		vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data()); // acquire property data

		for (const auto& prop : properties)
		{
#ifdef _DEBUG
			std::wstring name = unicode::ToWString(prop.extensionName) + L"\n";
			OutputDebugStringW(name.c_str());
#endif
			extensions.push_back(prop.extensionName);
		}
	}

	/*-------------------------------------------------------------------
	-               Set VKInstance Create Infomation
	---------------------------------------------------------------------*/
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // structure type
	createInfo.pApplicationInfo        = &applicationInfo;                       // application infomation
	createInfo.enabledExtensionCount   = static_cast<UINT32>(extensions.size()); // extension count
	createInfo.ppEnabledExtensionNames = extensions.data();                      // extension name list

#ifdef _DEBUG // enable debug layer 
	const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
	if constexpr(VK_HEADER_VERSION_COMPLETE < VK_MAKE_VERSION(1, 1, 106))
	{
		layers[0] = "VK_LAYER_LUNARG_standard_validation";
	}
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	createInfo.enabledLayerCount       = 1;                                      // enable layer count
	createInfo.ppEnabledLayerNames     = layers;                                 // layer name list 
	PopulateDebugMessengerCreateInfo(debugCreateInfo);                           
	createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;    
#else
	createInfo.enabledLayerCount = 0;
	createInfo.pNext = nullptr;
#endif
    /*-------------------------------------------------------------------
	-               Create VKInstance
	---------------------------------------------------------------------*/
	VkResult vkResult = vkCreateInstance(&createInfo, nullptr, &_instance);
	if (vkResult != VkResult::VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vulkan instance.");
	}
	
}

/****************************************************************************
*                     SelectPhysicalDevice
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::SelectPhysicalDevice()
*  @brief     Select Physical Device
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool GraphicsDeviceVulkan::SelectPhysicalDevice()
{
	/*-------------------------------------------------------------------
	-               Acquire physical devices
	---------------------------------------------------------------------*/
	UINT32 deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
	
	/*error check*/
	if (deviceCount == 0) { throw std::runtime_error("failed to find GPUs with Vulkan support."); }

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());
	/*-------------------------------------------------------------------
	-               Acquire Suitable Physical Device
	---------------------------------------------------------------------*/
	for (const auto& device : devices)
	{
#ifdef _DEBUG
		LogQueueList(device);
#endif
		if (IsDeviceSuitable(device)) { _physicalDevice = device; break; }
	}
	if (_physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find suitable GPU.");
	}
	/*-------------------------------------------------------------------
	-               Acquire memory properties
	---------------------------------------------------------------------*/
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_physicalMemoryProperties);
	return true;
}
/****************************************************************************
*                     CheckDeviceExtensionSupport
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::CheckDeviceExtensionSupport(VkPhysicalDevice device)
*  @brief     Is empty required extensions
*  @param[in] VkPhysicalDevice device
*  @return 　　bool
*****************************************************************************/
bool GraphicsDeviceVulkan::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	/*-------------------------------------------------------------------
	-               Acquire Extensions
	---------------------------------------------------------------------*/
	UINT32 extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	/*-------------------------------------------------------------------
	-               Acquire Extensions
	---------------------------------------------------------------------*/
	std::vector<const char*> extensions; // all extension name saved.
	std::vector<VkExtensionProperties> properties;
	{
		UINT32 count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);           // acqure property count
		properties.resize(count);                                                   // set buffer region 
		vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data()); // acquire property data

		for (const auto& prop : properties)
		{
			extensions.push_back(prop.extensionName);
		}
	}

	std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}
	return requiredExtensions.empty();
	
}
/****************************************************************************
*                     GetLogicalDevice
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::CreateLogicalDevice()
*  @brief     Get Logical Device
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool GraphicsDeviceVulkan::CreateLogicalDevice()
{
	/*-------------------------------------------------------------------
	-               Set DeviceQueueCreateInfo
	---------------------------------------------------------------------*/
	GraphicsQueueFamilyIndices indices = FindGraphicsQueueFamilies(_physicalDevice);
	const float defaultQueuePriority = 1.0f;
	
	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
	std::set<UINT32> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };
	for (UINT32 queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; // structure type
		deviceQueueCreateInfo.queueFamilyIndex = queueFamily;             // queue index
		deviceQueueCreateInfo.queueCount       = 1;                                          // queue count : 1
		deviceQueueCreateInfo.pQueuePriorities = &defaultQueuePriority;                      // queue property : 1.0
		deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
	}

	/*-------------------------------------------------------------------
	-               Acquire extension infomation
	---------------------------------------------------------------------*/
	std::vector<VkExtensionProperties> extensionProperties;
	{
		UINT32 count = 0;
		vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &count, nullptr);
		extensionProperties.resize(count);
		vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &count, extensionProperties.data());
	}

	std::vector<const char*> extensions;
	for (const auto& prop : extensionProperties)
	{
		extensions.push_back(prop.extensionName);
	}

	/*-------------------------------------------------------------------
	-               Set Device Create Info
	---------------------------------------------------------------------*/
	VkPhysicalDeviceFeatures deviceFeatures{};
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;   // structure type
	deviceCreateInfo.pQueueCreateInfos       = deviceQueueCreateInfos.data();                 // vk device queue create info
	deviceCreateInfo.queueCreateInfoCount    = static_cast<UINT32>(deviceQueueCreateInfos.size());   // count 2
	deviceCreateInfo.pEnabledFeatures        = &deviceFeatures;
	deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
	deviceCreateInfo.enabledExtensionCount   = static_cast<UINT32>(extensions.size());

	/*-------------------------------------------------------------------
	-               Set Device Create Info
	---------------------------------------------------------------------*/
	VkResult result = vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &_logicalDevice);
	if (result != VK_SUCCESS) { throw std::runtime_error("failed to create logical device"); return false; }
	/*-------------------------------------------------------------------
	-               Get Device Queue
	---------------------------------------------------------------------*/
	vkGetDeviceQueue(_logicalDevice, indices.GraphicsFamily.value(), 0, &_graphicsQueue);
	vkGetDeviceQueue(_logicalDevice, indices.PresentFamily .value(), 0, &_presentQueue);
	return true;
}
/****************************************************************************
*                     IsDeviceSuitable
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::IsDeviceSuitable(VkPhysicalDevice device)
*  @brief     Return Find QueueFamily Indices IsComplete
*  @param[in] VkPhysicalDevice device
*  @return 　　bool
*****************************************************************************/
bool GraphicsDeviceVulkan::IsDeviceSuitable(VkPhysicalDevice device)
{
	GraphicsQueueFamilyIndices indices = FindGraphicsQueueFamilies(device);
	
	bool extensionsSupported = true;//CheckDeviceExtensionSupport(device);
	bool swapchainAdequated  = false;
	if (extensionsSupported)
	{
		SwapchainSupportDetails details = SwapchainSupportDetails::Query(device, _surface);
		swapchainAdequated = !details.Formats.empty() && !details.PresentModes.empty();
	}
	return indices.IsComplete() && extensionsSupported && swapchainAdequated;
}
/****************************************************************************
*                     FindGraphicsQueue
*************************************************************************//**
*  @fn        GraphicsQueueFamilyIndices GraphicsDeviceVulkan::FindGraphicsQueueFamilies(VkPhysicalDevice device);
*  @brief     Return Find Graphics Queues
*  @param[in] VkPhysicalDevice device
*  @return 　　GraphicsQueueFamilyIndices
*****************************************************************************/
GraphicsDeviceVulkan::GraphicsQueueFamilyIndices GraphicsDeviceVulkan::FindGraphicsQueueFamilies(VkPhysicalDevice device)
{
	GraphicsQueueFamilyIndices indices;
	/*-------------------------------------------------------------------
	-               Acquire Queue family
	---------------------------------------------------------------------*/
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	/*-------------------------------------------------------------------
	-               Find Queue famiky
	---------------------------------------------------------------------*/
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { indices.GraphicsFamily = i; }
		
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);
		if (presentSupport) { indices.PresentFamily = i; }

		if (indices.IsComplete()) { break; }
		i++;
	}

	return indices;
}


/****************************************************************************
*                     CreateSurface
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateSurface()
*  @brief     Create Window Surface
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CreateSurface()
{
	VkWin32SurfaceCreateInfoKHR createInfo = {};                            // Window32 Surface. (! not cross platform)
	createInfo.hwnd      = _hwnd;                                           // hwnd 
	createInfo.hinstance = _hInstance;                                      // hInstance
	createInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR; // Structure type
	VkResult result      = vkCreateWin32SurfaceKHR(_instance, &createInfo, nullptr, &_surface);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to prepare surface."); 
	}
	
}
/****************************************************************************
*                     CreateImageViews
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateImageViews()
*  @brief     Create ImageViews
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CreateImageViews()
{
	_swapchainImageViews.resize(_swapchainImages.size());
	for (size_t i = 0; i < _swapchainImages.size(); ++i)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image      = _swapchainImages[i];
		createInfo.viewType   = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format     = _swapchainImageFormat;
		createInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel   = 0;
		createInfo.subresourceRange.levelCount     = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount     = 1;
		if (vkCreateImageView(_logicalDevice, &createInfo, nullptr, &_swapchainImageViews[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image views");
		}
	}
}
/****************************************************************************
*                     CreateDepthBuffer
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateDepthBuffer()
*  @brief     Create DepthBuffer for forward rendering
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CreateDepthBuffer()
{
	/*-------------------------------------------------------------------
	-               Set Image infomation
	---------------------------------------------------------------------*/
	VkImageCreateInfo createInfo = {};
	createInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.imageType     = VK_IMAGE_TYPE_2D;
	createInfo.format        = _depthStencilFormat;
	createInfo.extent.width  = _swapchainExtent.width;
	createInfo.extent.height = _swapchainExtent.height;
	createInfo.extent.depth  = 1;
	createInfo.mipLevels     = 1;
	createInfo.usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	createInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
	createInfo.arrayLayers   = 1;

	if (vkCreateImage(_logicalDevice, &createInfo, nullptr, &_depthBuffer))
	{
		throw std::runtime_error("failed to create depth image");
	}

	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image    = _depthBuffer;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format   = _depthStencilFormat;
	viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	viewInfo.subresourceRange.baseMipLevel   = 0;
	viewInfo.subresourceRange.levelCount     = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount     = 1;
	if (vkCreateImageView(_logicalDevice, &viewInfo, nullptr, &_depthBufferView))
	{
		throw std::runtime_error("failed to create depth image view");
	}

	/*-------------------------------------------------------------------
	-               Get memory allocator
	---------------------------------------------------------------------*/
	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(_logicalDevice, _depthBuffer, &requirements);  // get depth buffer size
	VkMemoryAllocateInfo allocatorInfo = {};
	allocatorInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocatorInfo.allocationSize  = requirements.size;
	allocatorInfo.memoryTypeIndex = GetMemoryTypeIndex(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vkAllocateMemory (_logicalDevice, &allocatorInfo, nullptr, &_depthBufferMemory);
	vkBindImageMemory(_logicalDevice, _depthBuffer, _depthBufferMemory, 0);

}
#pragma region Swapchain Function
/****************************************************************************
*                     CreateSwapchain
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateSwapchain()
*  @brief     Create swapchain
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CreateSwapchain()
{
	/*-------------------------------------------------------------------
	-               Acquire Surface infomation
	---------------------------------------------------------------------*/
	SwapchainSupportDetails details       = SwapchainSupportDetails::Query(_physicalDevice, _surface);
	VkSurfaceFormatKHR      surfaceFormat = SelectSwapchainFormat(details.Formats);
	VkPresentModeKHR        presentMode   = SelectSwapchainPresentMode(details.PresentModes);
	VkExtent2D              extent        = SelectSwapExtent(details.Capabilities);

	/*-------------------------------------------------------------------
	-               Acquire Swapchain frame buffer count
	---------------------------------------------------------------------*/
	UINT32 imageCount = FRAME_BUFFER_COUNT;
	if (details.Capabilities.minImageCount > 0 && imageCount < details.Capabilities.minImageCount)
	{
		imageCount = details.Capabilities.minImageCount;
	}
	if (details.Capabilities.maxImageCount > 0 && imageCount > details.Capabilities.maxImageCount)
	{
		imageCount = details.Capabilities.maxImageCount;
	}
	
	/*-------------------------------------------------------------------
	-               Acquire Swapchain create infomation
	---------------------------------------------------------------------*/
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;   // structure type
	createInfo.surface          = _surface;                                      // display surface 
	createInfo.minImageCount    = imageCount;                                    // frame buffer image count
	createInfo.imageFormat      = surfaceFormat.format;                          // pixel format : SDR B8G8R8A8 HDR : Float
	createInfo.imageColorSpace  = surfaceFormat.colorSpace;                      // color space 
	createInfo.imageExtent      = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	GraphicsQueueFamilyIndices indices = FindGraphicsQueueFamilies(_physicalDevice);
	UINT32 queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };
	if (indices.GraphicsFamily != indices.PresentFamily)
	{
		createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices   = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	createInfo.preTransform   = details.Capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode    = presentMode;
	createInfo.clipped        = TRUE;
	createInfo.oldSwapchain   = VK_NULL_HANDLE;

	/*-------------------------------------------------------------------
	-                 Create swap chain
	---------------------------------------------------------------------*/
	if (vkCreateSwapchainKHR(_logicalDevice, &createInfo, nullptr, &_swapchain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain");
	}
	/*-------------------------------------------------------------------
	-                 Determine swapchain infomation
	---------------------------------------------------------------------*/
	vkGetSwapchainImagesKHR(_logicalDevice, _swapchain, &imageCount, nullptr);
	_swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(_logicalDevice, _swapchain, &imageCount, _swapchainImages.data());
	_swapchainImageFormat = surfaceFormat.format;
	_swapchainExtent      = extent;
}
/****************************************************************************
*                     CreateRenderPass
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateRenderPass()
*  @brief     Create renderpass
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CreateRenderPass()
{
	/*-------------------------------------------------------------------
	-                   Attachment  Description
	---------------------------------------------------------------------*/
	VkAttachmentDescription attachments[2] = {};
	auto& colorAttachment = attachments[0];
	auto& depthAttachment = attachments[1];
	colorAttachment = VkAttachmentDescription{};
	colorAttachment.format         = _swapchainImageFormat;           // pixel format 
	colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;           // resource sample count
	colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;     // at the beginning of a render path, erase already existing data with a specific value
	colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;    // at the end of a render pass, save data on memory
	colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;       // resource layout : undefined
	colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // resource layout : present

	depthAttachment = VkAttachmentDescription{};
	depthAttachment.format         = _depthStencilFormat;           // pixel format 
	depthAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;           // resource sample count
	depthAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;     // at the beginning of a render path, erase already existing data with a specific value
	depthAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;    // at the end of a render pass, save data on memory
	depthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;       // resource layout : undefined
	depthAttachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;; // resource layout : DEPTH_STENCIL_ATTACHMENT_OPTIMAL

	/*-------------------------------------------------------------------
	-                   Attachment  Reference
	---------------------------------------------------------------------*/
	VkAttachmentReference colorReference{}, depthReference{};
	colorReference.attachment = 0;
	colorReference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	depthReference.attachment = 1;
	depthReference.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	/*-------------------------------------------------------------------
	-                   Subpass  Description
	---------------------------------------------------------------------*/
	VkSubpassDependency dependency = {};
	dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass    = 0;
	dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	/*-------------------------------------------------------------------
	-                   Subpass  Description
	---------------------------------------------------------------------*/
	VkSubpassDescription subpass    = {};
	subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS; // graphics pso
	subpass.colorAttachmentCount    = 1;                               // single pass  (GBufferとかは変わるのか?)
	subpass.pColorAttachments       = &colorReference;
	subpass.pDepthStencilAttachment = &depthReference;
	subpass.flags                   = 0;
	subpass.inputAttachmentCount    = 0;
	subpass.pInputAttachments       = nullptr;
	/*-------------------------------------------------------------------
	-                   Render pass create info
	---------------------------------------------------------------------*/
	VkRenderPassCreateInfo createInfo = {};
	createInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = _countof(attachments);
	createInfo.pAttachments    = &attachments[0];
	createInfo.subpassCount    = 1;
	createInfo.pSubpasses      = &subpass;
	createInfo.pDependencies   = &dependency;
	createInfo.dependencyCount = 1;
	if (vkCreateRenderPass(_logicalDevice, &createInfo, nullptr, &_renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass");
	}
}
/****************************************************************************
*                     CreateFrameBuffer
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateFrameBuffer()
*  @brief     Create Swapchain frame buffer
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CreateFrameBuffer()
{
	_swapchainFrameBuffers.resize(_swapchainImageViews.size());

	for (size_t i = 0; i < _swapchainImageViews.size(); ++i)
	{
		VkImageView attachment[] = { _swapchainImageViews[i], _depthBufferView };

		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass      = _renderPass;
		createInfo.attachmentCount = _countof(attachment);
		createInfo.pAttachments    = attachment;
		createInfo.width           = _swapchainExtent.width;
		createInfo.height          = _swapchainExtent.height;
		createInfo.layers          = 1;

		if (vkCreateFramebuffer(_logicalDevice, &createInfo, nullptr, &_swapchainFrameBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create frame buffer");
		}
	}

	_viewport.x        = 0.0f;
	_viewport.y        = 0.0f;
	_viewport.width    = _swapchainExtent.width;
	_viewport.height   = _swapchainExtent.height;
	_viewport.minDepth = 0.0f;
	_viewport.maxDepth = 1.0f;

	_rect2D.offset = { 0,0 };
	_rect2D.extent = _swapchainExtent;
}
/****************************************************************************
*                     SelectSwapchainFormat
*************************************************************************//**
*  @fn        VkSurfaceFormatKHR GraphicsDeviceVulkan::SelectSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& format)
*  @brief     Select swapchain format
*  @param[in] const std::vector<VkSurfaceFormatKHR>& format
*  @return 　　VkSurfaceFormatKHR
*****************************************************************************/
VkSurfaceFormatKHR GraphicsDeviceVulkan::SelectSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
	for (const auto& format : formats)
	{
		/*-------------------------------------------------------------------
		-               Find HDR Format
		---------------------------------------------------------------------*/
		if (_useHDR)
		{
			if ((format.format == VK_FORMAT_R16G16B16A16_SFLOAT) ||
				(format.format == VK_FORMAT_A2B10G10R10_UNORM_PACK32)){}
			else { continue; }

			if ((format.colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT) ||
				(format.colorSpace == VK_COLOR_SPACE_HDR10_ST2084_EXT) ||
				(format.colorSpace == VK_COLOR_SPACE_HDR10_HLG_EXT) ||
				(format.colorSpace == VK_COLOR_SPACE_DOLBYVISION_EXT)) {}
			else { continue; }

			return format;
		}
		/*-------------------------------------------------------------------
		-               Find SDR Format
		---------------------------------------------------------------------*/
		else
		{
			if ((format.format == VK_FORMAT_B8G8R8A8_UNORM) ||
				(format.format == VK_FORMAT_B8G8R8A8_SRGB)){}
			else { continue; }

			if ((format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)){}
			else { continue; }

			return format;
		}
	}
	return formats[0];
}
/****************************************************************************
*                     SelectSwapchainPresentMode
*************************************************************************//**
*  @fn        VkPresentModeKHR GraphicsDeviceVulkan::SelectSwapchainPresentMode(const std::vector<VkPresentModeKHR>& presentMode)
*  @brief     Select vsync mode
*  @param[in] const std::vector<VkPresentModeKHR>& presentMode
*  @return 　　VkPresentModeKHR
*****************************************************************************/
VkPresentModeKHR GraphicsDeviceVulkan::SelectSwapchainPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
{
	for (const auto& presentMode : presentModes)
	{
		if (VSYNC == 0 && presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) { return presentMode; } // No Vsync  
		if (VSYNC  > 0 && presentMode == VK_PRESENT_MODE_MAILBOX_KHR)   { return presentMode; }   // Vsync
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}
/****************************************************************************
*                     SelectSwapExtent
*************************************************************************//**
*  @fn        VkExtent2D  GraphicsDeviceVulkan::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
*  @brief     Select swapchain screen size
*  @param[in] const std::vector<VkPresentModeKHR>& presentMode
*  @return 　　VkExtent2D 
*****************************************************************************/
VkExtent2D  GraphicsDeviceVulkan::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	// https://wasuregasa.3rin.net/%E3%82%A2%E3%83%9E%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0/c--%20%E3%81%AE%20std--numeric_limits
	if (capabilities.currentExtent.width != (std::numeric_limits<unsigned int>::max)())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width  = static_cast<int>(Screen::GetScreenWidth());
		int height = static_cast<int>(Screen::GetScreenHeight());
		VkExtent2D actualExtent = { static_cast<UINT32>(width), static_cast<UINT32>(height)};
		actualExtent.width  = std::clamp(actualExtent.width , capabilities.minImageExtent.width , capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actualExtent;
	}
}
/****************************************************************************
*                     CreateCommandPool
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateCommandPool()
*  @brief     Create command pool
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CreateCommandPool()
{
	GraphicsQueueFamilyIndices indices = FindGraphicsQueueFamilies(_physicalDevice);
	VkCommandPoolCreateInfo createInfo = {};
	createInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex        = indices.GraphicsFamily.value();
	createInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	if (vkCreateCommandPool(_logicalDevice, &createInfo, nullptr, &_commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool");
	}
}
/****************************************************************************
*                     CreateCommandBuffers
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateCommandBuffers()
*  @brief     Create command buffers (frame buffer count)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CreateCommandBuffers()
{
	VkCommandBufferAllocateInfo allocatorInfo{};
	allocatorInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocatorInfo.commandPool        = _commandPool;
	allocatorInfo.commandBufferCount = static_cast<UINT32>(_swapchainImageViews.size());
	allocatorInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	
	_commandBuffers.resize(allocatorInfo.commandBufferCount);
	if (vkAllocateCommandBuffers(_logicalDevice, &allocatorInfo, _commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers");
	}
}
/****************************************************************************
*                     CreateSyncObjects
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateSyncObjects()
*  @brief     Create sync objects
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::CreateSyncObjects()
{
	/*-------------------------------------------------------------------
	-                  Resize sync objects
	---------------------------------------------------------------------*/
	_fences.resize(_swapchainImageViews.size());
	//_renderCompletedSemaphores .resize(_swapchainImageViews.size());
	//_presentCompletedSemaphores.resize(_swapchainImageViews.size());

	/*-------------------------------------------------------------------
	-                  Set up each create info
	---------------------------------------------------------------------*/
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	/*-------------------------------------------------------------------
	-                  Create objects
	---------------------------------------------------------------------*/
	for (size_t i = 0; i < _swapchainImageViews.size(); ++i)
	{
		if (vkCreateFence(_logicalDevice, &fenceCreateInfo, nullptr, &_fences[i]))
		{
			throw std::runtime_error("failed to create fence");
		}
	}
	if (vkCreateSemaphore(_logicalDevice, &semaphoreCreateInfo, nullptr, &_renderCompletedSemaphores))
	{
		throw std::runtime_error("failed to create render completed semaphore");
	}

	if (vkCreateSemaphore(_logicalDevice, &semaphoreCreateInfo, nullptr, &_presentCompletedSemaphores))
	{
		throw std::runtime_error("failed to create present completed semaphore");
	}

}
/****************************************************************************
*                     GetMemoryTypeIndex
*************************************************************************//**
*  @fn        UINT32 GraphicsDeviceVulkan::GetMemoryTypeIndex(UINT32 requestBits, VkMemoryPropertyFlags propertyFlags)  const
*  @brief     Select swapchain screen size
*  @param[in] UINT32 requestBits
*  @param[in] vkMemoryPropertyFlags propertyFlags
*  @return 　　UINT32 Index
*****************************************************************************/
UINT32 GraphicsDeviceVulkan::GetMemoryTypeIndex(UINT32 requestBits, VkMemoryPropertyFlags propertyFlags)  const
{
	UINT32 memoryTypeIndex = ~0u;
	for (UINT32 i = 0; i < _physicalMemoryProperties.memoryTypeCount; ++i)
	{
		if (requestBits & 1)
		{
			const auto& types = _physicalMemoryProperties.memoryTypes[i];
			if ((types.propertyFlags & propertyFlags) == propertyFlags)
			{
				memoryTypeIndex = i;
				break;
			}
		}
		requestBits >>= 1;
	}
	return memoryTypeIndex;
}
#pragma endregion Swapchain Function
#pragma endregion   Set Up
#pragma region Debug Function
/****************************************************************************
*                     PopulateDebugMessengerCreateInfo
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
*  @brief     Create Debug Messenger infomation.
*  @param[out] VkDebugUtilsMessengerCreateInfoEXT& createInfo
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallback;
}
/****************************************************************************
*                     SetUpDebugMessenger
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::SetUpDebugMessenger()
*  @brief     Set up debug messenger
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::SetUpDebugMessenger()
{
#ifdef _DEBUG
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	PopulateDebugMessengerCreateInfo(createInfo);
	if (::CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug messenger!");
	}
#endif
}
/****************************************************************************
*                     LogQueueList
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::LogQueueList(VkPhysicalDevice device)
*  @brief     Log queue list
*  @param[in] VkPhysicalDevice device
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceVulkan::LogQueueList(VkPhysicalDevice device)
{
	/*-------------------------------------------------------------------
	-               Acquire Queue family
	---------------------------------------------------------------------*/
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	/*-------------------------------------------------------------------
	-               Acquire Physical device properties
	---------------------------------------------------------------------*/
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(device, &prop);
	OutputDebugStringA("Device Name: "); OutputDebugStringA(prop.deviceName); OutputDebugStringA("\n");
	/*-------------------------------------------------------------------
	-               Find Queue famiky
	---------------------------------------------------------------------*/
	for (const auto& queueFamily : queueFamilies)
	{
		OutputDebugStringW(L"////////////////////////////////////////////////\n");
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)       { OutputDebugStringW(L"Graphics supported\n"); }
		if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)        { OutputDebugStringW(L"Compute supported\n"); }
		if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)       { OutputDebugStringW(L"Transfer supported\n"); }
		if (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) { OutputDebugStringW(L"Sparce binding supported\n"); }
		if (queueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT)      { OutputDebugStringW(L"Protected supported\n");}
		OutputDebugStringW(L"////////////////////////////////////////////////\n");
	}

}
bool GraphicsDeviceVulkan::CheckValidationLayerSupport()
{
	/*-------------------------------------------------------------------
	-                  Get Available Layers
	---------------------------------------------------------------------*/
	UINT32 layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : s_ValidationLayers)
	{
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true; break;
			}
		}

		if (!layerFound) { return false; }
	}
	return true;
}
#pragma endregion Debug Function
#pragma endregion Private Function

