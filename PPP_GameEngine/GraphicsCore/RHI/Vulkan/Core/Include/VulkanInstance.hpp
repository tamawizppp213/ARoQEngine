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
	class RHIInstance : public core::RHIInstance
	{
	public:
		
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* return all available display adapter*/
		std::vector<std::shared_ptr<core::RHIDisplayAdapter>> EnumrateAdapters() override;
		/*vulkan : dGPU (not : first select gpu) */
		std::shared_ptr<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		/* OutputDebugString : adapter list*/
		void LogAdapters() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkInstance       GetVkInstance()       { return _instance; }
		const VkInstance GetVkInstance() const { return _instance; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIInstance() = default;
		~RHIInstance();
		RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		bool CheckValidationLayerSupport(); // check enable cpu and gpu debugger
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkInstance               _instance         = nullptr;
		VkDebugUtilsMessengerEXT _debugMessenger   = nullptr;
		std::vector<const char*> _instanceLayers   = {};
		std::uint32_t            _vulkanAPIVersion = VK_API_VERSION_1_3; // newest version
	
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		// Set up
		std::vector<std::string>      AcquireExtensionList();
		std::vector<VkPhysicalDevice> EnumratePhysicalDevices();
		
		// debugging
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	};
}

#endif