//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (このエンジンを使用時最初に呼び出す.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_RHI_INSTANCE_HPP
#define VULKAN_RHI_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
#include "VulkanHelper.hpp"
#include <vulkan/vulkan.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHIInstance
	*************************************************************************//**
	*  @class     RHIInstance
	*  @brief     Select device api and display adapter
	*****************************************************************************/
	class RHIInstance : public core::RHIInstance, public gu::EnableSharedFromThis<RHIInstance>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* return all available display adapter*/
		std::vector<gu::SharedPointer<core::RHIDisplayAdapter>> EnumrateAdapters() override;
		
		/*vulkan : dGPU (not : first select gpu) */
		gu::SharedPointer<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		
		/*vulkan : iGPU (not : first select gpu) */
		gu::SharedPointer<core::RHIDisplayAdapter> SearchMinimumPowerAdapter() override;
		
		/* OutputDebugString : adapter list*/
		void LogAdapters() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkInstance       GetVkInstance()       { return _instance; }
		
		const VkInstance GetVkInstance() const { return _instance; }

		std::vector<VkLayerProperties> GetInstanceLayers() const;

		// Vulkan version check. 
		bool MeetRequiredVersion(const std::uint32_t major, const std::uint32_t minor, const std::uint32_t patch = 0)
		{
			return VK_MAKE_VERSION(_majorVersion, _minorVersion, _patchVersion) >= VK_MAKE_VERSION(major, minor, patch);
		}
		std::uint32_t GetVkAPIVersion() const { return VK_MAKE_VERSION(_majorVersion, _minorVersion, _patchVersion); }
		std::uint32_t GetVkMajorVersion() const { return _majorVersion; }
		std::uint32_t GetVkMinorVersion() const { return _minorVersion; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIInstance() = default;

		~RHIInstance();

		RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger, bool useGPUDebugBreak);

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		bool CheckValidationLayerSupport(); // check enable cpu and gpu debugger
		
		gu::SharedPointer<core::RHIDisplayAdapter> SearchAdapter(const VkPhysicalDeviceType deviceType);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkInstance               _instance         = nullptr;
		VkDebugUtilsMessengerEXT _debugMessenger   = nullptr;
		std::vector<const char*> _instanceLayers   = {};
		
		// current version
		std::uint32_t _majorVersion = 0;
		std::uint32_t _minorVersion = 0;
		std::uint32_t _patchVersion = 0;
	
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		// Set up
		std::vector<std::string>      AcquireExtensionList();

		std::vector<VkPhysicalDevice> EnumratePhysicalDevices();

		// push back name array list
		VkResult FillFilteredNameArray(std::vector<std::string>& used, 
			const std::vector<VkLayerProperties>& properties, 
			const std::vector<Entry>& requestedLayers);

		VkResult FillFilteredNameArray(std::vector<std::string>& used,
			const std::vector<VkExtensionProperties>& properties,
			const std::vector<Entry>& requested,
			std::vector<void*>& featureStructs);

		// debugging
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	};
}

#endif