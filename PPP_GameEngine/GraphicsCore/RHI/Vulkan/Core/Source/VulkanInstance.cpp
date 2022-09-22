//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (このエンジンを使用時最初に呼び出す.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanAdapter.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 26812 4100)
using namespace rhi;
using namespace rhi::vulkan;
#pragma comment(lib, "vulkan-1.lib")
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
#pragma region Create and Destroy Debug Messenger
	/****************************************************************************
	*				  			CreateDebugUtilsMessengerEXT
	*************************************************************************//**
	*  @class     DestroyDebugUtilsMessengerEXT
	*  @brief     Call free debug messenger
	*****************************************************************************/
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

	/****************************************************************************
	*				  			DestroyDebugUtilsMessengerEXT
	*************************************************************************//**
	*  @class     DestroyDebugUtilsMessengerEXT
	*  @brief     Call free debug messenger
	*****************************************************************************/
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}
#pragma endregion Create and Destroy Debug Messenger
#pragma region Debug Report Function
	/****************************************************************************
	*                     SkipErrorMessage
	*************************************************************************//**
	*  @fn        std::vector<VkExtensionProperties> RHIInstance::AcquireExtensionProperties()
	*  @brief     Return isSkip (true : skip, false: not skip)
	*  @param[in] void
	*  @return 　　bool
	*****************************************************************************/
	bool SkipWarningMessage(VkDebugUtilsMessageSeverityFlagBitsEXT flags, VkObjectType objectType, const std::string& message)
	{
		// ignore verbose and info
		if (objectType != VK_OBJECT_TYPE_INSTANCE)                   { return false; }
		if (flags < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) { return false; }

		// skip error message list
		static std::vector<std::string> mutedWarnings = {
			"UNASSIGNED-CoreValidation-Shader-InconsistentSpirv",
			"VUID-vkCmdDrawIndexed-None-04007",
			"VUID-vkDestroyDevice-device-00378",
			"VUID-VkSubmitInfo-pWaitSemaphores-03243",
			"VUID-VkSubmitInfo-pSignalSemaphores-03244",
			"VUID-vkCmdPipelineBarrier-pDependencies-02285",
			"VUID-VkImageMemoryBarrier-oldLayout-01213",
			"VUID-vkCmdDrawIndexed-None-02721",
			"VUID-vkCmdDrawIndexed-None-02699",
			"VUID-vkCmdTraceRaysKHR-None-02699",
			"VUID-VkShaderModuleCreateInfo-pCode-04147",
		};

		// skip check
		for (auto& str : mutedWarnings)
		{
			if (message.find(str) != std::string::npos) { return true; }
		}
		
		return false; // not found skip 
	}

	/****************************************************************************
	*                     DebugReportCallback
	*************************************************************************//**
	*  @fn        std::vector<VkExtensionProperties> RHIInstance::AcquireExtensionProperties()
	*  @brief     Report debug error message. (処理は止めない. 報告のみ)
	*****************************************************************************/
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData)
	{
		/*-------------------------------------------------------------------
		-          Skip error message check
		---------------------------------------------------------------------*/
		if (SkipWarningMessage(messageSeverity, pCallbackData->pObjects->objectType, pCallbackData->pMessage)) { return VK_FALSE; }
		/*-------------------------------------------------------------------
		-			Report debug message
		---------------------------------------------------------------------*/
		std::stringstream buffer;
		buffer << "[validation layer] " << pCallbackData->pMessageIdName << " " <<  
			pCallbackData->pMessage << std::endl;
#ifdef _WIN32
		OutputDebugStringA(buffer.str().c_str());
#else
		printf("%s\n", buf.str().c_str());
#endif    
		return VK_FALSE;
	}
#pragma endregion Debug Report Function
}

#pragma region Constructor and Destructor
RHIInstance::RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger) 
	:core::RHIInstance(enableCPUDebugger, enableGPUDebugger)
{
	/*-------------------------------------------------------------------
	-          Check validation support (in case enable CPU or GPU debugger)
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	if (enableCPUDebugger || enableGPUDebugger)
	{
		if (!CheckValidationLayerSupport())
		{
			throw std::runtime_error("validation layers requested, but not available");
		}
	}
#endif

	/*-------------------------------------------------------------------
	-          Acquire Extension Infomation (for debugging layer)
	---------------------------------------------------------------------*/
	std::vector<std::string> foundExtensions = AcquireExtensionList();       // 単純なconst char* 値渡しでは出来なかった. 
	std::vector<const char*> convertFoundExtensions(foundExtensions.size()); // VKInstance create informationで使用するためにデータ成型
	for (int i = 0; i < foundExtensions.size(); ++i)
	{
		convertFoundExtensions[i] = foundExtensions[i].c_str();
	}
	
	/*-------------------------------------------------------------------
	-               Set Application Infomation
	---------------------------------------------------------------------*/
	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO; // structure type
	applicationInfo.pApplicationName = "Graphics Device Vulkan";           // application name
	applicationInfo.pEngineName      = EngineName;                         // engine name
	applicationInfo.apiVersion       = _vulkanAPIVersion;                  // newest version
	applicationInfo.engineVersion    = VK_MAKE_VERSION(1, 0, 0);           // engine version

	/*-------------------------------------------------------------------
	-               Set VKInstance Create Infomation
	---------------------------------------------------------------------*/
	VkInstanceCreateInfo createInfo = {}; 
	createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;        // structure type
	createInfo.pApplicationInfo        = &applicationInfo;                              // application infomation
	createInfo.enabledExtensionCount   = static_cast<UINT32>(convertFoundExtensions.size());   // extension count
	createInfo.ppEnabledExtensionNames = convertFoundExtensions.data();                        // extension name list
	createInfo.flags                   = 0;
	createInfo.enabledLayerCount       = 0;
	createInfo.pNext                   = nullptr;
#ifdef _DEBUG
	if (enableCPUDebugger || enableGPUDebugger)
	{
		createInfo.enabledLayerCount   = static_cast<std::uint32_t>(_instanceLayers.size());                 // enable layer count
		createInfo.ppEnabledLayerNames = _instanceLayers.data();
	}
#endif

	/*-------------------------------------------------------------------
	-               Create VKInstance
	---------------------------------------------------------------------*/
	VkResult vkResult = vkCreateInstance(&createInfo, nullptr, &_instance);
	if (vkResult != VkResult::VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vulkan instance.");
	}

	/*-------------------------------------------------------------------
	-               Set up debugging layer
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	if (!(enableCPUDebugger || enableGPUDebugger)) { return; }

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
	PopulateDebugMessengerCreateInfo(debugCreateInfo);
	if (::CreateDebugUtilsMessengerEXT(_instance, &debugCreateInfo, nullptr, &_debugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug messenger!");
	}
#endif
}

RHIInstance::~RHIInstance()
{
#ifdef _DEBUG
	printf("Destroy RHI Instance : (Vulkan)\n");
#endif
	if(_debugMessenger){DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);}
	if (_instance)     { vkDestroyInstance(_instance, nullptr); }
	_instanceLayers.clear(); _instanceLayers.shrink_to_fit();
}

#pragma endregion Constructor and Destructor
#pragma region Adapter Function
/****************************************************************************
*                     EnumrateAdapters
*************************************************************************//**
*  @fn        std::vector<std::shared_ptr<core::RHIAdapter>> EnumrateAdapters()
*  @brief     Return all availablle adapter lists
*  @param[in] void
*  @return 　　std::vector<std::shared_ptr<core::RHIAdapter>> 
*****************************************************************************/
std::vector<std::shared_ptr<core::RHIDisplayAdapter>> RHIInstance::EnumrateAdapters()
{
	std::vector<std::shared_ptr<core::RHIDisplayAdapter>> adapterLists = {};

	/*-------------------------------------------------------------------
	-               Acquire physical devices
	---------------------------------------------------------------------*/
	const auto& devices = EnumratePhysicalDevices();

	/*-------------------------------------------------------------------
	-               Acquire Vulkan Adapter List
	---------------------------------------------------------------------*/
	for (const auto& device : devices)
	{
		adapterLists.emplace_back(std::make_shared<vulkan::RHIDisplayAdapter>(shared_from_this(), device));
	}
	return adapterLists;
}
/****************************************************************************
*                     SearchHighPerformanceAdapter
*************************************************************************//**
*  @fn        std::shared_ptr<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
*  @brief     Return discrete GPU adapter 
*  @param[in] void
*  @return 　　std::shared_ptr<core::RHIAdapter>
*****************************************************************************/
std::shared_ptr<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
{
	const auto& devices = EnumratePhysicalDevices();
	if (devices.size() == 0) { return nullptr; }

	std::shared_ptr<core::RHIDisplayAdapter> adapter = std::make_shared<vulkan::RHIDisplayAdapter>(shared_from_this(),devices[0]);
	for (int i = 1; i < devices.size(); ++i)
	{
		/*-------------------------------------------------------------------
		-               Get properties
		---------------------------------------------------------------------*/
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(devices[i], &properties);
		/*-------------------------------------------------------------------
		-               Discrete GPU check
		---------------------------------------------------------------------*/
		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			adapter = std::make_shared<vulkan::RHIDisplayAdapter>(shared_from_this(),devices[i]); break;
		}
	}

	return adapter;
}

/****************************************************************************
*                     LogAdapters
*************************************************************************//**
*  @fn        void RHIInstance::LogAdapters()
*  @brief     Show all available adapter information using Output debug string
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIInstance::LogAdapters()
{
	auto adapterList = EnumrateAdapters();
	for (auto& adapter : adapterList)
	{
		adapter->PrintInfo();
	}
}

#pragma endregion Adapter Function
#pragma region Set Up Function
/****************************************************************************
*                     AcquireExtensionProperties
*************************************************************************//**
*  @fn        std::vector<VkExtensionProperties> RHIInstance::AcquireExtensionProperties()
*  @brief     Return all enable vk extension name list. 
*  @param[in] void
*  @return 　　std::vector<const char*>
*****************************************************************************/
std::vector<std::string> RHIInstance::AcquireExtensionList()
{
	std::vector<std::string> foundExtensions = {};
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
			foundExtensions.push_back(std::string(prop.extensionName));
		}
	}
	return foundExtensions;
}
/****************************************************************************
*                     EnumratePhysicalDevices
*************************************************************************//**
*  @fn        std::vector<VkPhysicalDevice> RHIInstance::EnumratePhysicalDevices()
*  @brief     Return physical device vector list
*  @param[in] void
*  @return 　　std::vector<VkPhysicalDevice>
*****************************************************************************/
std::vector<VkPhysicalDevice> RHIInstance::EnumratePhysicalDevices()
{
	UINT32 deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

	/*error check*/
	if (deviceCount == 0) { throw std::runtime_error("failed to find GPUs with Vulkan support."); }

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());
	
	return devices;
}
#pragma endregion  Set Up  Function
#pragma region Debug Function
/****************************************************************************
*                     CheckValidationLayerSupport
*************************************************************************//**
*  @fn        bool RHIDevice::CheckValidationLayerSupport()
*  @brief     Validation layer support check
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool RHIInstance::CheckValidationLayerSupport()
{
	/*-------------------------------------------------------------------
	-                  Get Available Layers
	---------------------------------------------------------------------*/
	std::uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	/*-------------------------------------------------------------------
	-                  Search Available Layers
	---------------------------------------------------------------------*/
	// 1.1.106.0 のバージョンから単一のレイヤだけで旧レイヤーの全ての検証機能を使用可能
	if constexpr (VK_HEADER_VERSION_COMPLETE >= VK_MAKE_VERSION(1, 1, 106)) 
	{
		_instanceLayers.push_back("VK_LAYER_KHRONOS_validation"); // normally use
	}
	else
	{
		_instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
	}

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
*  @fn        void RHIInstance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
*  @brief     Set up callback debug report 
*  @param[out] VkDebugReportCallbackCreateInfoEXT& createInfo
*  @return 　　void
*****************************************************************************/
void RHIInstance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallback;
}
#pragma endregion   Debug   Function