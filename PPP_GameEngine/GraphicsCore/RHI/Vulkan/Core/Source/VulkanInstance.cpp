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
	*  @brief     Define a callback to capture the messages
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
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,    // メッセージの重要度
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
RHIInstance::RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger, bool useGPUDebugBreak) 
	:core::RHIInstance(enableCPUDebugger, enableGPUDebugger, useGPUDebugBreak)
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
	-               Acquire current api version 
	---------------------------------------------------------------------*/
	// find the instance version
	std::uint32_t instanceVersion = VK_API_VERSION_1_0;
	auto EnumrateInstanceVersion  = PFN_vkEnumerateInstanceVersion(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));
	if (EnumrateInstanceVersion)
	{
		EnumrateInstanceVersion(&instanceVersion);
	}

	// extract newest version info
	_majorVersion = VK_VERSION_MAJOR(instanceVersion);
	_minorVersion = VK_VERSION_MINOR(instanceVersion);
	_patchVersion = VK_VERSION_PATCH(instanceVersion);

	/*-------------------------------------------------------------------
	-               Set Application Infomation
	---------------------------------------------------------------------*/
	const VkApplicationInfo applicationInfo =
	{
		.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO, // structure type
		.pNext              = nullptr,
		.pApplicationName   = "Graphics Device Vulkan",           // application name
		.applicationVersion = VK_MAKE_VERSION(1,0,0),             // application version (特に使用しない)
		.pEngineName        = EngineName,                         // engine name
		.engineVersion      = VK_MAKE_VERSION(1,0,0),             // engine version      (特に使用しない)
		.apiVersion         = VK_MAKE_VERSION(_majorVersion, _minorVersion, _patchVersion), // newest version
	};

	/*-------------------------------------------------------------------
	-               Set VKInstance Create Infomation
	---------------------------------------------------------------------*/
	VkInstanceCreateInfo createInfo = 
	{
		.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,               // structure type
		.pNext                   = nullptr,
		.flags                   = 0,
		.pApplicationInfo        = &applicationInfo,                                     // application infomation
		.enabledLayerCount       = 0,
		.ppEnabledLayerNames     = nullptr,
		.enabledExtensionCount   = static_cast<UINT32>(convertFoundExtensions.size()),   // extension count
		.ppEnabledExtensionNames = convertFoundExtensions.data(),                        // extension name list
	}; 
	
#ifdef _DEBUG
	/*-------------------------------------------------------------------
	-       Layer : mesurement for the validation and performance
	---------------------------------------------------------------------*/
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
	if (_debugMessenger){DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);}
	if (_instance)     { vkDestroyInstance(_instance, nullptr); }
	_instanceLayers.clear(); _instanceLayers.shrink_to_fit();
}

#pragma endregion Constructor and Destructor
#pragma region Adapter Function
/****************************************************************************
*                     EnumrateAdapters
*************************************************************************//**
*  @fn        std::vector<gu::SharedPointer<core::RHIAdapter>> EnumrateAdapters()
*  @brief     Return all availablle adapter lists
*  @param[in] void
*  @return 　　std::vector<gu::SharedPointer<core::RHIAdapter>> 
*****************************************************************************/
std::vector<gu::SharedPointer<core::RHIDisplayAdapter>> RHIInstance::EnumrateAdapters()
{
	std::vector<gu::SharedPointer<core::RHIDisplayAdapter>> adapterLists = {};

	/*-------------------------------------------------------------------
	-               Acquire physical devices
	---------------------------------------------------------------------*/
	const auto& devices = EnumratePhysicalDevices();

	/*-------------------------------------------------------------------
	-               Acquire Vulkan Adapter List
	---------------------------------------------------------------------*/
	for (const auto& device : devices)
	{
		adapterLists.emplace_back(gu::MakeShared<vulkan::RHIDisplayAdapter>(SharedFromThis(), device));
	}
	return adapterLists;
}
/****************************************************************************
*                     SearchHighPerformanceAdapter
*************************************************************************//**
*  @fn        gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
*  @brief     Return discrete GPU adapter. not found : first adapter
*  @param[in] void
*  @return 　　gu::SharedPointer<core::RHIAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
{
	return SearchAdapter(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
}

/****************************************************************************
*                     SearchMinimumPowerAdapter
*************************************************************************//**
*  @fn        gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchMinimumPowerAdapter()
*  @brief     Return integrated GPU adapter. not found : first adapter
*  @param[in] void
*  @return 　　gu::SharedPointer<core::RHIAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchMinimumPowerAdapter()
{
	return SearchAdapter(VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
}

/****************************************************************************
*                     SearchAdapter
*************************************************************************//**
*  @fn        gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchAdapter(const VkPhysicalDeviceType deviceType)
*  @brief     Return proper GPU adapte. not found : first found adapter 
*  @param[in] void
*  @return 　　gu::SharedPointer<core::RHIAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchAdapter(const VkPhysicalDeviceType deviceType)
{
	const auto& devices = EnumratePhysicalDevices();
	if (devices.size() == 0) { return nullptr; }

	// 必要となるものがなければ最初に見つけたAdapterを渡す. 
	gu::SharedPointer<core::RHIDisplayAdapter> adapter = gu::MakeShared<vulkan::RHIDisplayAdapter>(SharedFromThis(), devices[0]);
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
		if (properties.deviceType == deviceType)
		{
			adapter = gu::MakeShared<vulkan::RHIDisplayAdapter>(SharedFromThis(), devices[i]); break;
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
	const auto availableLayers = GetInstanceLayers();

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

	_instanceLayers.push_back("VK_LAYER_KHRONOS_shader_object");
	//_instanceLayers.push_back( "VK_LAYER_LUNARG_api_dump");

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

std::vector<VkLayerProperties> RHIInstance::GetInstanceLayers() const
{
	std::uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	return availableLayers;
}

/****************************************************************************
*                     FillFilteredNameArray
*************************************************************************//**
*  @fn        VkResult RHIInstance::FillFilteredNameArray(std::vector<std::string>& used,
			  const std::vector<VkLayerProperties>& properties,
		      const std::vector<Entry>& requestedLayers)

*  @brief     used: layer property name vector list. 
* 
*  @param[out] std::vector<std::string>& used : layer property name vector.
*  @param[in]  std::vector<VkLayerProperties>properties : VkLayerProperty
*  @param[in]  std::vector<Entry>& requestedLayer
* 
*  @return 　　VkResult
*****************************************************************************/
VkResult RHIInstance::FillFilteredNameArray(std::vector<std::string>& used,
	const std::vector<VkLayerProperties>& properties,
	const std::vector<Entry>& requestedLayers)
{
	/* This implementation is used because it is not used in release build.*/
	// requested layerの中から, propertiesの名前を見つける.
	for (const auto& layer : requestedLayers)
	{
		bool layerFound = false;
		for (const auto& property : properties)
		{
			if (strcmp(layer.Name.c_str(), property.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (layerFound)
		{
			used.push_back(layer.Name);
		}
		else if (layer.Optional == false)
		{
			return VK_ERROR_LAYER_NOT_PRESENT;
		}
	}

	return VK_SUCCESS;
}

/****************************************************************************
*                     FillFilteredNameArray
*************************************************************************//**
*  @fn        VkResult RHIInstance::FillFilteredNameArray(std::vector<std::string>& used,
			  const std::vector<VkLayerProperties>& properties,
			  const std::vector<Entry>& requestedLayers)

*  @brief     used: layer property name vector list.
*
*  @param[out] std::vector<std::string>& used : layer property name vector.
*  @param[in]  std::vector<VkLayerProperties>properties : VkLayerProperty
*  @param[in]  std::vector<Entry>& requestedLayer
*  @param[out] std::vector<void*>& featureStructs
*
*  @return 　　VkResult
*****************************************************************************/
VkResult RHIInstance::FillFilteredNameArray(std::vector<std::string>& used,
	const std::vector<VkExtensionProperties>& properties,
	const std::vector<Entry>& requested,
	std::vector<void*>& featureStructs)
{
	for (const auto& itr : requested)
	{
		bool found = false;
		for (const auto& property : properties)
		{
			// 対象の名前とスペックが一致するかを調べる.
			if (strcmp(itr.Name.c_str(), property.extensionName) == 0 &&
				itr.Version == 0 || itr.Version == property.specVersion)
			{
				found = true;
				break;
			}
		}

		// feature struct push back
		if (found)
		{
			used.push_back(itr.Name);
			if (itr.FeatureStruct)
			{
				featureStructs.push_back(itr.FeatureStruct);
			}
		}
		else if (!itr.Optional)
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	return VK_SUCCESS;
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

	// メッセージの重要性: 診断メッセージ, アプリケーションのバグである可能性が高い動作に対するメッセージ, 無効でクラッシュにつながる動作についてのメッセージを出力する
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	
	// 仕様にもパフォーマンスにも関係のないイベントが発生, 仕様に違反した何かが行った場合, VUlkanの最適でない使い方をしている可能性のあるものを報告
	createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	
	// コールバックを行う関数.
	createInfo.pfnUserCallback = DebugCallback;
}
#pragma endregion   Debug   Function