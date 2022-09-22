//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIAdapter.hpp
///             @brief  Physical Device (adapter), Describe gpu information 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_ADAPTER_HPP
#define VULKAN_ADAPTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIAdapter.hpp"
#include <vulkan/vulkan.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHIAdapter
	*************************************************************************//**
	*  @class     RHIAdapter
	*  @brief     Physical Device (Adapter)  
	*****************************************************************************/
	class RHIDisplayAdapter : public rhi::core::RHIDisplayAdapter
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		std::shared_ptr<core::RHIDevice> CreateDevice(const std::uint32_t frameCount) override;

		void PrintInfo() override; 
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const VkPhysicalDevice               GetPhysicalDevice       () const noexcept { return _physicalDevice; }
		VkPhysicalDeviceProperties           GetProperties           () const noexcept;
		VkPhysicalDeviceFeatures             GetSupports             () const noexcept;
		std::vector<VkExtensionProperties>   GetExtensionProperties  () const noexcept;
		std::vector<std::string>             GetExtensionNameList    () const noexcept;
		std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties() const noexcept;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDisplayAdapter() = default;
		~RHIDisplayAdapter();
		RHIDisplayAdapter(const std::shared_ptr<core::RHIInstance>& instance, const VkPhysicalDevice physicalDevice);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkPhysicalDevice _physicalDevice = nullptr;
	};
}
#endif