//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIAdapter.hpp
///             @brief  Physical Device (adapter), Describe gpu information 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanAdapter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <Windows.h>
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIDisplayAdapter::RHIDisplayAdapter(const gu::SharedPointer<core::RHIInstance>& instance, const VkPhysicalDevice physicalDevice)
	: core::RHIDisplayAdapter(instance), _physicalDevice(physicalDevice)
{
	Checkf(_instance,"instance is nullptr");

	const auto adapterProperties = GetProperties();
	const auto memoryProperties  = GetMemoryProperties();
	_name     = adapterProperties.deviceName;
	_venderID = adapterProperties.vendorID;
	_deviceID = adapterProperties.deviceID;

	/*-------------------------------------------------------------------
	-                  Get property and memory infomation
	---------------------------------------------------------------------*/
	_isDiscreteGPU = memoryProperties.memoryHeapCount > 1;

	/*-------------------------------------------------------------------
	-                Set up physical device info
	---------------------------------------------------------------------*/
	// for quaries and device creation
	VkPhysicalDeviceFeatures2   adapterFeatures2   = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
	VkPhysicalDeviceProperties2 adapterProperties2 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };

	// memory property
	_physicalDeviceInfo.MemoryProperties      = GetMemoryProperties();
	_physicalDeviceInfo.QueueFamilyProperties = GetQueueFamilyProperties();
	
	const auto vkInstance = gu::StaticPointerCast<vulkan::RHIInstance>(instance);
	if (vkInstance->MeetRequiredVersion(1, 2))
	{
		// set up linked list (Features)
		adapterFeatures2.pNext               = &_physicalDeviceInfo.Features11;
		_physicalDeviceInfo.Features11.pNext = &_physicalDeviceInfo.Features12;
		_physicalDeviceInfo.Features12.pNext = nullptr;
		
		// set up linked list (Properties)
		_physicalDeviceInfo.Properties12.driverID                     = VK_DRIVER_ID_NVIDIA_PROPRIETARY;
		_physicalDeviceInfo.Properties12.supportedDepthResolveModes   = VK_RESOLVE_MODE_MAX_BIT;
		_physicalDeviceInfo.Properties12.supportedStencilResolveModes = VK_RESOLVE_MODE_MAX_BIT;
		adapterProperties2.pNext               = &_physicalDeviceInfo.Properties11;
		_physicalDeviceInfo.Properties11.pNext = &_physicalDeviceInfo.Properties12;
		_physicalDeviceInfo.Properties12.pNext = nullptr;
	}
	if (vkInstance->MeetRequiredVersion(1, 3))
	{
		_physicalDeviceInfo.Features12.pNext   = &_physicalDeviceInfo.Features13;
		_physicalDeviceInfo.Features13.pNext   = nullptr;
		_physicalDeviceInfo.Properties12.pNext = &_physicalDeviceInfo.Properties13;
		_physicalDeviceInfo.Properties13.pNext = nullptr;
	}

	vkGetPhysicalDeviceFeatures2  (_physicalDevice, &adapterFeatures2);
	vkGetPhysicalDeviceProperties2(_physicalDevice, &adapterProperties2);
	_physicalDeviceInfo.Properties10 = adapterProperties2.properties;
	_physicalDeviceInfo.Features10   = adapterFeatures2.features;
}

RHIDisplayAdapter::~RHIDisplayAdapter()
{
	
}

#pragma endregion Constructor and Destructor 

#pragma region Public Function
gu::SharedPointer<core::RHIDevice> RHIDisplayAdapter::CreateDevice()
{
	return gu::MakeShared<RHIDevice>(SharedFromThis());
}

/****************************************************************************
*                     PrintInfo
*************************************************************************//**
*  @fn        void RHIAdapter::PrintInfo()
* 
*  @brief     Print physical device information and spec
* 
*  @param[in] void
* 
*  @return 　  void
*****************************************************************************/
void RHIDisplayAdapter::PrintInfo()
{
	/*-------------------------------------------------------------------
	-                  Get memory infomation
	---------------------------------------------------------------------*/
	const auto memoryProperties = GetMemoryProperties();

	/*-------------------------------------------------------------------
	-                  Print Adapter Name
	---------------------------------------------------------------------*/
	gu::string adapterName
		= "\n//////////////////////////\n Adapter : ";
	adapterName += gu::string(_name.CString());
	adapterName += "\n//////////////////////////\n";
	OutputDebugStringA(adapterName.CString());

	for (std::uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i)
	{
		// どのmemoryかはDirectX12から推定することになりそう.(現状)
		const std::string str = "Memory : " + std::to_string(memoryProperties.memoryHeaps[i].size) + "\n";
		OutputDebugStringA(str.c_str());
	}
}

/****************************************************************************
*                     GetProperties
*************************************************************************//**
*  @fn        VkPhysicalDeviceProperties RHIDisplayAdapter::GetProperties() const 
* 
*  @brief     Return device properties struct
* 
*  @param[in] void
* 
*  @return 　 VkPhysicalDeviceProperties
*****************************************************************************/
VkPhysicalDeviceProperties RHIDisplayAdapter::GetProperties() const noexcept
{
	VkPhysicalDeviceProperties deviceProperty = {};
	vkGetPhysicalDeviceProperties(_physicalDevice, &deviceProperty);
	return deviceProperty;
}


/****************************************************************************
*                     GetLimits
*************************************************************************//**
*  @fn        VkPhysicalDeviceLimits RHIDisplayAdapter::GetLimits() const noexcept
*
*  @brief     Return device limit
*
*  @param[in] void
*
*  @return 　 VkPhysicalDeviceLimits
*****************************************************************************/
VkPhysicalDeviceLimits RHIDisplayAdapter::GetLimits() const noexcept
{
	return GetProperties().limits;
}

/****************************************************************************
*                     GetFormatProperties
*************************************************************************//**
*  @fn        VkFormatProperties RHIDisplayAdapter::GetFormatProperties(const VkFormat format) const noexcept
*
*  @brief     Return device format properties struct
*
*  @param[in] void
*
*  @return 　 VkFormatProperties
*****************************************************************************/
VkFormatProperties RHIDisplayAdapter::GetFormatProperties(const VkFormat format) const noexcept
{
	VkFormatProperties formatProperty = {};
	vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &formatProperty);
	return formatProperty;
}

/****************************************************************************
*                     GetSupport
*************************************************************************//**
*  @fn        VkPhysicalDeviceFeatures RHIDisplayAdapter::GetSupports() const noexcept
* 
*  @brief     Return physical device support contents
* 
*  @param[in] void
* 
*  @return 　 VkPhysicalDeviceFeatures
*****************************************************************************/
VkPhysicalDeviceFeatures RHIDisplayAdapter::GetSupports() const noexcept
{
	VkPhysicalDeviceFeatures deviceFeatures = {};
	vkGetPhysicalDeviceFeatures(_physicalDevice, &deviceFeatures);
	return deviceFeatures;
}

/****************************************************************************
*                     GetMemoryProperties
*************************************************************************//**
*  @fn        VkPhysicalDeviceMemoryProperties RHIDisplayAdapter::GetMemoryProperties() const noexcept
*
*  @brief     Return physical device memory property
*
*  @param[in] void
*
*  @return 　 VkPhysicalDeviceMemoryProperties
*****************************************************************************/
VkPhysicalDeviceMemoryProperties RHIDisplayAdapter::GetMemoryProperties() const noexcept
{
	VkPhysicalDeviceMemoryProperties memoryProperties = {};
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memoryProperties);
	return memoryProperties;
}

/****************************************************************************
*                     GetExtensionProperties
*************************************************************************//**
*  @fn        VkPhysicalDeviceProperties RHIDisplayAdapter::GetProperties() const
* 
*  @brief     Return extension name and spec version
* 
*  @param[in] void
* 
*  @return 　 gu::DynamicArray<VkExtensionProperties>
*****************************************************************************/
gu::DynamicArray<VkExtensionProperties> RHIDisplayAdapter::GetExtensionProperties() const noexcept
{
	/*-------------------------------------------------------------------
	-               Acquire extension infomation
	---------------------------------------------------------------------*/
	gu::DynamicArray<VkExtensionProperties> extensionProperties;
	{
		UINT32 count = 0;
		vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &count, nullptr);
		extensionProperties.Resize(count);
		vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &count, extensionProperties.Data());
	}
	return extensionProperties;
}

/****************************************************************************
*                     GetExtensionNameList
*************************************************************************//**
*  @fn        gu::DynamicArray<const char*> RHIDisplayAdapter::GetExtensionNameList() const noexcept
* 
*  @brief     Return all extension name list. 
* 
*  @param[in] void
* 
*  @return 　  gu::DynamicArray<const char*>
*****************************************************************************/
gu::DynamicArray<gu::string> RHIDisplayAdapter::GetExtensionNameList() const noexcept
{
	/*-------------------------------------------------------------------
	-               Acquire extension infomation
	---------------------------------------------------------------------*/
	gu::DynamicArray<VkExtensionProperties> extensionProperties = GetExtensionProperties();
	/*-------------------------------------------------------------------
	-               All Device Extensions : Enabled
	---------------------------------------------------------------------*/
	gu::DynamicArray<gu::string> extensions;
	for (const auto& prop : extensionProperties)
	{
		extensions.Push(prop.extensionName);
	}
	return extensions;
}

/****************************************************************************
*                     GetExtensionProperties
*************************************************************************//**
*  @fn        VkPhysicalDeviceProperties RHIDisplayAdapter::GetProperties() const
* 
*  @brief     Return extension name and spec version
* 
*  @param[in] void
* 
*  @return 　 gu::DynamicArray<VkExtensionProperties>
*****************************************************************************/
gu::DynamicArray<VkQueueFamilyProperties> RHIDisplayAdapter::GetQueueFamilyProperties() const noexcept
{
	/*-------------------------------------------------------------------
	-               Acquire extension infomation
	---------------------------------------------------------------------*/
	gu::DynamicArray<VkQueueFamilyProperties> queueFamilyProperties;
	{
		UINT32 count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &count, nullptr);
		queueFamilyProperties.Resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &count, queueFamilyProperties.Data());
	}
	return queueFamilyProperties;
}

bool RHIDisplayAdapter::IsPresentSupported(VkSurfaceKHR surface, std::uint32_t queueFamilyIndex) const
{
	VkBool32 presentSupported = false;

	if (surface != VK_NULL_HANDLE)
	{
		vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, queueFamilyIndex, surface, &presentSupported);
	}

	return (bool)presentSupported;
}

#pragma endregion Public Function