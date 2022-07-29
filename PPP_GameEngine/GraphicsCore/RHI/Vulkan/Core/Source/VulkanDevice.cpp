//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanDevice.cpp
///             @brief  Device 
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandQueue.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandList.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandAllocator.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDescriptorHeap.hpp"
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUPipelineFactory.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <Windows.h>
#include <set>
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 26812 4100)
using namespace rhi;
using namespace rhi::vulkan;
#pragma comment(lib, "vulkan-1.lib")
namespace
{
	constexpr const char*          s_DeviceName       = "Graphics Device Vulkan";
	constexpr const char*          s_EngineName       = "PPP Engine";
	const std::vector<const char*> s_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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
#pragma region Destructor
RHIDevice::~RHIDevice()
{
	vkDestroyDevice(_logicalDevice, nullptr); // destroy logical device
	vkDestroySurfaceKHR(_instance, _surface, nullptr);
#ifdef _DEBUG
	DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
#endif
	vkDestroyInstance(_instance, nullptr);

	_deviceExtensions.clear(); _deviceExtensions.shrink_to_fit();
	_instanceLayers  .clear(); _instanceLayers  .shrink_to_fit();
}
#pragma endregion Destructor
#pragma region Start Up Function
bool RHIDevice::Create(HWND hwnd, HINSTANCE hInstance, bool useHDR, bool useRaytracing)
{
	_apiVersion = rhi::core::APIVersion::Vulkan;
	_hwnd = hwnd; _hInstance = hInstance;
	_useRaytracing = useRaytracing;
	_isHDRSupport  = useHDR;
	CreateInstance();
	SetUpDebugMessenger();
	CreateSurface();
	SelectPhysicalDevice();
	CreateLogicalDevice(useRaytracing);
	return true;
}
#pragma endregion        Start Up Function
#pragma region Create Resource Function
std::shared_ptr<core::RHISwapchain>  RHIDevice::CreateSwapchain(const std::shared_ptr<rhi::core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount, const std::uint32_t vsync)
{
	return nullptr;
}
std::shared_ptr<core::RHICommandList> RHIDevice::CreateCommandList(const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator)
{
	return std::static_pointer_cast<core::RHICommandList>(std::make_shared<vulkan::RHICommandList>(shared_from_this(), commandAllocator));
}
std::shared_ptr<core::RHICommandQueue> RHIDevice::CreateCommandQueue()
{
	return std::static_pointer_cast<core::RHICommandQueue>(std::make_shared<vulkan::RHICommandQueue>(shared_from_this()));
}
std::shared_ptr<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator()
{
	return std::static_pointer_cast<core::RHICommandAllocator>(std::make_shared<vulkan::RHICommandAllocator>(shared_from_this()));
}
std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount)
{
	return std::static_pointer_cast<core::RHIDescriptorHeap>(std::make_shared<vulkan::RHIDescriptorHeap>(shared_from_this(), heapType, maxDescriptorCount));
}
std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const std::vector<core::DescriptorHeapType>& heapTypes, const std::vector<size_t>& maxDescriptorCounts)
{
	return std::static_pointer_cast<core::RHIDescriptorHeap>(std::make_shared<vulkan::RHIDescriptorHeap>(shared_from_this(), heapTypes, maxDescriptorCounts));
}
std::shared_ptr<core::GPUPipelineFactory> RHIDevice::CreatePipelineFactory()
{
	return std::static_pointer_cast<core::GPUPipelineFactory>(std::make_shared<vulkan::GPUPipelineFactory>());
}
size_t RHIDevice::AllocateQueue()
{
	if (_freeQueues.empty()) { throw std::runtime_error("too many queues were allocated"); }

	size_t index = _freeQueues.back();
	_freeQueues.pop_back();
	return index;
}
void RHIDevice::FreeQueue(const size_t index)
{
	const auto it = std::find(_freeQueues.begin(), _freeQueues.end(), index);

	if (it == _freeQueues.end())
	{
		_freeQueues.push_back(index);
	}
}
#pragma endregion Create Resource Function
#pragma region Debug Function
/****************************************************************************
*                     CheckValidationLayerSupport
*************************************************************************//**
*  @fn        bool RHIDevice::CheckValidationLayerSupport()
*  @brief     Validation layer support check
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool RHIDevice::CheckValidationLayerSupport()
{
	/*-------------------------------------------------------------------
	-                  Get Available Layers
	---------------------------------------------------------------------*/
	std::uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
#ifdef _DEBUG
	/*-------------------------------------------------------------------
	-                  Search Available Layers
	---------------------------------------------------------------------*/
	if constexpr (VK_HEADER_VERSION_COMPLETE >= VK_MAKE_VERSION(1, 1, 106))
	{
		_instanceLayers.push_back("VK_LAYER_KHRONOS_validation"); // normally use
	}
	else
	{
		_instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
	}
#endif
	/* This implementation is used because it is not used in release build.*/
	for (const char* layerName : _instanceLayers)
	{
		bool layerFound = false;
		for (const auto& availableLayer : availableLayers)
		{
			if (strcmp(layerName, availableLayer.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		
		}
		if (!layerFound) { return false; }
	}
	return true;
}
/****************************************************************************
*                     PopulateDebugMessengerCreateInfo
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
*  @brief     Create Debug Messenger infomation.
*  @param[out] VkDebugUtilsMessengerCreateInfoEXT& createInfo
*  @return 　　void
*****************************************************************************/
void RHIDevice::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
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
void RHIDevice::SetUpDebugMessenger()
{
#ifdef _DEBUG
	if (!_enableValidationLayer) { return; }
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
void RHIDevice::LogQueueList(VkPhysicalDevice device)
{
#ifdef _DEBUG
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
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { OutputDebugStringW(L"Graphics supported\n"); }
		if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)  { OutputDebugStringW(L"Compute supported\n"); }
		if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) { OutputDebugStringW(L"Transfer supported\n"); }
		if (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) { OutputDebugStringW(L"Sparce binding supported\n"); }
		if (queueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT) { OutputDebugStringW(L"Protected supported\n"); }
		OutputDebugStringW(L"////////////////////////////////////////////////\n");
	}
#endif
}
#pragma endregion           Debug  Function
#pragma region Set Up Function
/****************************************************************************
*                     CreateInstance
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::CreateInstance()
*  @brief     Create vkInstance.
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool RHIDevice::CreateInstance()
{
	const char* deviceName = s_DeviceName;
	const char* engineName = s_EngineName;
#ifdef _DEBUG
	if (!CheckValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available");
	}
#endif

	/*-------------------------------------------------------------------
	-          Acquire Extension Infomation (for debugging layer)
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
			_deviceExtensions.push_back(prop.extensionName);
		}
	}

	/*-------------------------------------------------------------------
	-               Set Application Infomation
	---------------------------------------------------------------------*/
	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO; // structure type
	applicationInfo.pApplicationName = deviceName;                         // application name
	applicationInfo.pEngineName      = engineName;                         // engine name
	applicationInfo.apiVersion       = _vulkanAPIVersion;                  // newest version
	applicationInfo.engineVersion    = VK_MAKE_VERSION(1, 0, 0);           // engine version

	/*-------------------------------------------------------------------
	-               Set VKInstance Create Infomation
	---------------------------------------------------------------------*/
	VkInstanceCreateInfo createInfo = {}; 
	createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;        // structure type
	createInfo.pApplicationInfo        = &applicationInfo;                              // application infomation
	createInfo.enabledExtensionCount   = static_cast<UINT32>(_deviceExtensions.size()); // extension count
	createInfo.ppEnabledExtensionNames = _deviceExtensions.data();                      // extension name list
	createInfo.flags                   = 0;

#ifdef _DEBUG
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	createInfo.enabledLayerCount       = static_cast<std::uint32_t>(_instanceLayers.size());                 // enable layer count
	createInfo.ppEnabledLayerNames     = _instanceLayers.data();                 // layer name list 
	PopulateDebugMessengerCreateInfo(debugCreateInfo);                           
	createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#else
	createInfo.enabledLayerCount = 0;
	createInfo.pNext            = nullptr;
#endif
	
	/*-------------------------------------------------------------------
	-               Create VKInstance
	---------------------------------------------------------------------*/
	VkResult vkResult = vkCreateInstance(&createInfo, nullptr, &_instance);
	if (vkResult != VkResult::VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vulkan instance.");
	}
	return true;
}

/****************************************************************************
*                     SelectPhysicalDevice
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::SelectPhysicalDevice()
*  @brief     Select Physical Device
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
void RHIDevice::SelectPhysicalDevice()
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
	vkGetPhysicalDeviceFeatures(_physicalDevice, &_physicalDeviceFeatures);    // all features enabled (指定機能だけ有効にしても良いかもしれません.)
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_memoryProperties);
}
/****************************************************************************
*                     IsDeviceSuitable
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::IsDeviceSuitable(VkPhysicalDevice device)
*  @brief     Return Find QueueFamily Indices IsComplete
*  @param[in] VkPhysicalDevice device
*  @return 　　bool
*****************************************************************************/
bool RHIDevice::IsDeviceSuitable(VkPhysicalDevice device)
{
	FindGraphicsQueueFamilies(device);

	bool extensionsSupported = true;//CheckDeviceExtensionSupport(device);
	bool swapchainAdequated = false;
	if (extensionsSupported)
	{
		SwapchainSupportDetails details = SwapchainSupportDetails::Query(device, _surface);
		swapchainAdequated = !details.Formats.empty() && !details.PresentModes.empty();
	}
	return _freeQueues.size()!=0 && extensionsSupported && swapchainAdequated;
}

/****************************************************************************
*                     CreateSurface
*************************************************************************//**
*  @fn        void GraphicsDeviceVulkan::CreateSurface()
*  @brief     Create Window Surface
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CreateSurface()
{
	VkWin32SurfaceCreateInfoKHR createInfo = {};                            // Window32 Surface. (! not cross platform)
	createInfo.hwnd      = _hwnd;                                           // hwnd 
	createInfo.hinstance = _hInstance;                                      // hInstance
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR; // Structure type
	VkResult result = vkCreateWin32SurfaceKHR(_instance, &createInfo, nullptr, &_surface);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to prepare surface.");
	}

}
/****************************************************************************
*                     GetLogicalDevice
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::CreateLogicalDevice()
*  @brief     Get Logical Device
*  @param[in] bool useRaytracing
*  @return 　　bool
*****************************************************************************/
void RHIDevice::CreateLogicalDevice(bool useRaytracing)
{
	assert(_physicalDevice);
	std::vector<float> queuePriorities(_freeQueues.size(), 1.0f);
	for (size_t index = 0; index < _freeQueues.size(); ++index)
	{
		_freeQueues[index] = _freeQueues.size() - index - 1;
	}
	/*-------------------------------------------------------------------
	-               Set device queue create info
	---------------------------------------------------------------------*/
	VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
	deviceQueueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; // structure type
	deviceQueueCreateInfo.queueFamilyIndex = _queueFamilyIndex.GraphicsFamily.value();   // queue index
	deviceQueueCreateInfo.queueCount       = static_cast<std::uint32_t>(_freeQueues.size());                                          // queue count : 1
	deviceQueueCreateInfo.pQueuePriorities = queuePriorities.data();                      // queue property : 1.0

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
	/*-------------------------------------------------------------------
	-               All Device Extensions : Enabled
	---------------------------------------------------------------------*/
	std::vector<const char*> extensions;
	for (const auto& prop : extensionProperties)
	{
		extensions.push_back(prop.extensionName);
	}

	/*-------------------------------------------------------------------
	-               Set Device Create Info
	---------------------------------------------------------------------*/
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;   // structure type
	deviceCreateInfo.pQueueCreateInfos       = &deviceQueueCreateInfo;                 // vk device queue create info
	deviceCreateInfo.queueCreateInfoCount    = static_cast<UINT32>(1);
	deviceCreateInfo.pEnabledFeatures        = &_physicalDeviceFeatures;
	deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
	deviceCreateInfo.enabledExtensionCount   = static_cast<UINT32>(extensions.size());
	deviceCreateInfo.pNext                   = nullptr;

	/*-------------------------------------------------------------------
	-               Set Raytracing features
	---------------------------------------------------------------------*/
	if (useRaytracing) 
	{
		VkPhysicalDeviceBufferDeviceAddressFeaturesKHR enabledBufferDeviceAddressFeatures = {};
		enabledBufferDeviceAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
		enabledBufferDeviceAddressFeatures.pNext = nullptr;

		VkPhysicalDeviceRayTracingPipelineFeaturesKHR enabledDeviceRayTracingPipelineFeatures = {};
		enabledDeviceRayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
		enabledDeviceRayTracingPipelineFeatures.pNext = nullptr;

		VkPhysicalDeviceAccelerationStructureFeaturesKHR enabledAccelerationStructureFeatures = {};
		enabledAccelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
		enabledAccelerationStructureFeatures.pNext = nullptr;

		VkPhysicalDeviceDescriptorIndexingFeatures enabledDescriptorIndexingFeatures = {};
		enabledDescriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;

		enabledBufferDeviceAddressFeatures.bufferDeviceAddress                       = VK_TRUE;
		enabledDeviceRayTracingPipelineFeatures.rayTracingPipeline                   = VK_TRUE;
		enabledAccelerationStructureFeatures.accelerationStructure                   = VK_TRUE;
		enabledDescriptorIndexingFeatures.shaderUniformBufferArrayNonUniformIndexing = VK_TRUE;
		enabledDescriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing  = VK_TRUE;
		enabledDescriptorIndexingFeatures.runtimeDescriptorArray                     = VK_TRUE;
		enabledDescriptorIndexingFeatures.descriptorBindingVariableDescriptorCount   = VK_TRUE;
		enabledDescriptorIndexingFeatures.descriptorBindingPartiallyBound            = VK_TRUE;// 部分的なバインディング.
		enabledDeviceRayTracingPipelineFeatures.pNext                                = &enabledBufferDeviceAddressFeatures;
		enabledAccelerationStructureFeatures.pNext                                   = &enabledDeviceRayTracingPipelineFeatures;
		enabledDescriptorIndexingFeatures.pNext                                      = &enabledAccelerationStructureFeatures;

		VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{
			VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, nullptr,
		};
		physicalDeviceFeatures2.pNext    = &enabledDescriptorIndexingFeatures;
		physicalDeviceFeatures2.features = _physicalDeviceFeatures;
		

		deviceCreateInfo.pNext            = &physicalDeviceFeatures2;
		deviceCreateInfo.pEnabledFeatures = nullptr;
	}

	/*-------------------------------------------------------------------
	-               Set Device Create Info
	---------------------------------------------------------------------*/
	VkResult result = vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &_logicalDevice);
	if (result != VK_SUCCESS) { throw std::runtime_error("failed to create logical device"); return; }
}

/****************************************************************************
*                     FindGraphicsQueue
*************************************************************************//**
*  @fn        GraphicsQueueFamilyIndices GraphicsDeviceVulkan::FindGraphicsQueueFamilies(VkPhysicalDevice device);
*  @brief     Return Find Graphics Queues
*  @param[in] VkPhysicalDevice device
*  @return 　　GraphicsQueueFamilyIndices
*****************************************************************************/
void RHIDevice::FindGraphicsQueueFamilies(VkPhysicalDevice device)
{
	/*-------------------------------------------------------------------
	-               Acquire Queue family
	---------------------------------------------------------------------*/
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	/*-------------------------------------------------------------------
	-               Find Queue family
	---------------------------------------------------------------------*/
	// [0] : Most ideal condition,  [1] : graphics support 
	VkQueueFlags flags[] = { VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT , VK_QUEUE_GRAPHICS_BIT };
	for (std::uint32_t flag = 0; flag < _countof(flags); ++flag)
	{
		for (std::uint32_t i = 0; i < queueFamilyCount; ++i)
		{
			/*-------------------------------------------------------------------
			-               Was queue family found ? 
			---------------------------------------------------------------------*/
			if (_queueFamilyIndex.IsComplete()) { break; }
			/*-------------------------------------------------------------------
			-               Graphics support check
			---------------------------------------------------------------------*/
			if (!(queueFamilies[i].queueFlags & flags[flag])) { continue; }

#ifdef _WIN32
			if (!vkGetPhysicalDeviceWin32PresentationSupportKHR(device, static_cast<std::uint32_t>(i))) { continue; }
#endif
			/*-------------------------------------------------------------------
			-             Acquire Graphics queue family index and queue count
			---------------------------------------------------------------------*/
			_queueFamilyIndex.GraphicsFamily = i;
			_freeQueues.resize(queueFamilies[i].queueCount);
		}
	}
	
	if (_freeQueues.size() == 0) { throw std::runtime_error("Free queue's size is 0."); }
}
#pragma endregion          Set Up Function
