//////////////////////////////////////////////////////////////////////////////////
///  @file   VulkanAdapter.hpp
///  @brief  論理デバイスに渡す物理デバイス(Apdapter)の設定, GPU情報を取得
///  @author Toide Yutaro
///  @date   2024_03_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_ADAPTER_HPP
#define VULKAN_ADAPTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIAdapter.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include <vulkan/vulkan.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHIDisplayAdapter
	*************************************************************************//**
	/* @class     RHIDisplayAdapter
	*  @brief     論理デバイスに渡す物理デバイス(Apdapter)の設定, GPU情報を取得
	*****************************************************************************/
	class RHIDisplayAdapter : public rhi::core::RHIDisplayAdapter, public gu::EnableSharedFromThis<RHIDisplayAdapter>
	{
	public:
		struct PhysicalDeviceInfo
		{
			// memory 
			VkPhysicalDeviceMemoryProperties MemoryProperties = {};

			gu::DynamicArray<VkQueueFamilyProperties> QueueFamilyProperties = {};

			// vulkanのversionに合わせたFeature list
			VkPhysicalDeviceFeatures         Features10 = {};
			VkPhysicalDeviceVulkan11Features Features11 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES};
			VkPhysicalDeviceVulkan12Features Features12 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
			VkPhysicalDeviceVulkan13Features Features13 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };

			// vulkanのversionに合わせたproperty list
			VkPhysicalDeviceProperties         Properties10 = {};
			VkPhysicalDeviceVulkan11Properties Properties11 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES };
			VkPhysicalDeviceVulkan12Properties Properties12 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES };
			VkPhysicalDeviceVulkan13Properties Properties13 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES };
		};

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief 自身の物理デバイスに基づいて論理デバイスを生成し, そのSharedPointerを渡します.
		*************************************************************************/
		gu::SharedPointer<core::RHIDevice> CreateDevice() override;
		
		/*!**********************************************************************
		*  @brief 物理デバイスの名前とスペックを出力に表示します@n
		*         基本的に実行時のログとして使用するものになります. @n
		*         ファイルや文字列に出力は行わないです.
		*************************************************************************/
		virtual void PrintInfo() const override; 

		#pragma endregion Public Function

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief  アダプタに直接接続されている出力の数 (モニターなど)を返します.
		*************************************************************************/
		gu::uint64 GetOutputCount() const override { return 0; }

		const VkPhysicalDevice GetPhysicalDevice() const noexcept { return _physicalDevice; }

		/* return all physical device information(memory, feature, property)*/
		PhysicalDeviceInfo& GetPhysicalDeviceInfo() { return _physicalDeviceInfo; }
		const PhysicalDeviceInfo& GetPhysicalDeviceInfo() const noexcept { return _physicalDeviceInfo; }
		
		/* return physical device characteristics (device id, vender id...)*/
		VkPhysicalDeviceProperties GetProperties() const noexcept;

		// return physical device limits
		VkPhysicalDeviceLimits GetLimits() const noexcept;
		
		/* return physical device support list (ex. can use geometry shader...?)*/
		VkPhysicalDeviceFeatures GetSupports() const noexcept;
		
		/* return format available properties*/
		VkFormatProperties GetFormatProperties(const VkFormat format) const noexcept;

		// return memory type, memory heap.
		VkPhysicalDeviceMemoryProperties     GetMemoryProperties() const noexcept;
		
		/* return all available extension name list*/
		gu::DynamicArray<VkExtensionProperties>   GetExtensionProperties  () const noexcept;
		gu::DynamicArray<gu::string>              GetExtensionNameList    () const noexcept;
		
		/* return queue family characteristics*/
		gu::DynamicArray<VkQueueFamilyProperties> GetQueueFamilyProperties() const noexcept;
		
		bool IsPresentSupported(VkSurfaceKHR surface, std::uint32_t queueFamilyIndex) const;

		#pragma endregion Public Property
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDisplayAdapter() = default;
		
		~RHIDisplayAdapter();
		
		RHIDisplayAdapter(const gu::SharedPointer<core::RHIInstance>& instance, const VkPhysicalDevice physicalDevice);
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		VkPhysicalDevice _physicalDevice = nullptr;

		PhysicalDeviceInfo _physicalDeviceInfo = {};
	};
}
#endif