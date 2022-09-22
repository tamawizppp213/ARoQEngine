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
RHIDisplayAdapter::RHIDisplayAdapter(const std::shared_ptr<core::RHIInstance>& instance, const VkPhysicalDevice physicalDevice)
	: core::RHIDisplayAdapter(instance), _physicalDevice(physicalDevice)
{
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(_physicalDevice, &prop);
	_name     = prop.deviceName;
	_venderID = prop.vendorID;
	_deviceID = prop.deviceID;

}
RHIDisplayAdapter::~RHIDisplayAdapter()
{
	
}
#pragma endregion Constructor and Destructor 

#pragma region Public Function
std::shared_ptr<core::RHIDevice> RHIDisplayAdapter::CreateDevice(const std::uint32_t frameCount)
{
	std::shared_ptr<core::RHIDevice> device = std::make_shared<RHIDevice>(shared_from_this(), frameCount);
	device->SetUp();
	return device;
}

/****************************************************************************
*                     PrintInfo
*************************************************************************//**
*  @fn        void RHIAdapter::PrintInfo()
*  @brief     Print physical device information
*  @param[in] void
*  @return 　  void
*****************************************************************************/
void RHIDisplayAdapter::PrintInfo()
{
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(_physicalDevice, &prop);

	std::wstring adapterName = L"\n\n***Adapter: ";
	adapterName += unicode::ToWString(_name);
	adapterName += L"\n";

	OutputDebugStringW(adapterName.c_str());
}

/****************************************************************************
*                     GetProperties
*************************************************************************//**
*  @fn        VkPhysicalDeviceProperties RHIDisplayAdapter::GetProperties() const 
*  @brief     Return device properties struct
*  @param[in] void
*  @return 　 VkPhysicalDeviceProperties
*****************************************************************************/
VkPhysicalDeviceProperties RHIDisplayAdapter::GetProperties() const noexcept
{
	VkPhysicalDeviceProperties deviceProperty = {};
	vkGetPhysicalDeviceProperties(_physicalDevice, &deviceProperty);
	return deviceProperty;
}

/****************************************************************************
*                     GetSupport
*************************************************************************//**
*  @fn        VkPhysicalDeviceFeatures RHIDisplayAdapter::GetSupports() const noexcept
*  @brief     Return physical device support contents
*  @param[in] void
*  @return 　 VkPhysicalDeviceFeatures
*****************************************************************************/
VkPhysicalDeviceFeatures RHIDisplayAdapter::GetSupports() const noexcept
{
	VkPhysicalDeviceFeatures deviceFeatures = {};
	vkGetPhysicalDeviceFeatures(_physicalDevice, &deviceFeatures);
	return deviceFeatures;
}
/****************************************************************************
*                     GetExtensionProperties
*************************************************************************//**
*  @fn        VkPhysicalDeviceProperties RHIDisplayAdapter::GetProperties() const
*  @brief     Return extension name and spec version
*  @param[in] void
*  @return 　 std::vector<VkExtensionProperties>
*****************************************************************************/
std::vector<VkExtensionProperties> RHIDisplayAdapter::GetExtensionProperties() const noexcept
{
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
	return extensionProperties;
}
/****************************************************************************
*                     GetExtensionNameList
*************************************************************************//**
*  @fn        std::vector<const char*> RHIDisplayAdapter::GetExtensionNameList() const noexcept
*  @brief     Return all extension name list. 
*  @param[in] void
*  @return 　  std::vector<const char*>
*****************************************************************************/
std::vector<std::string> RHIDisplayAdapter::GetExtensionNameList() const noexcept
{
	/*-------------------------------------------------------------------
	-               Acquire extension infomation
	---------------------------------------------------------------------*/
	std::vector<VkExtensionProperties> extensionProperties = GetExtensionProperties();
	/*-------------------------------------------------------------------
	-               All Device Extensions : Enabled
	---------------------------------------------------------------------*/
	std::vector<std::string> extensions;
	for (const auto& prop : extensionProperties)
	{
		extensions.push_back(prop.extensionName);
	}
	return extensions;
}
/****************************************************************************
*                     GetExtensionProperties
*************************************************************************//**
*  @fn        VkPhysicalDeviceProperties RHIDisplayAdapter::GetProperties() const
*  @brief     Return extension name and spec version
*  @param[in] void
*  @return 　 std::vector<VkExtensionProperties>
*****************************************************************************/
std::vector<VkQueueFamilyProperties> RHIDisplayAdapter::GetQueueFamilyProperties() const noexcept
{
	/*-------------------------------------------------------------------
	-               Acquire extension infomation
	---------------------------------------------------------------------*/
	std::vector<VkQueueFamilyProperties> queueFamilyProperties;
	{
		UINT32 count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &count, nullptr);
		queueFamilyProperties.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &count, queueFamilyProperties.data());
	}
	return queueFamilyProperties;
}
#pragma endregion Public Function