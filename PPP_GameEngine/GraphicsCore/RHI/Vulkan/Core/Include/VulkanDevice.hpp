//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanDevice.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_DEVICE_HPP
#define VULKAN_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDevice.hpp"
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                          Device class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	class RHICommandQueue;
	class RHISwapchain;
	/****************************************************************************
	*				  			Device class
	*************************************************************************//**
	*  @class     Device
	*  @brief     Logical device
	*****************************************************************************/
	class RHIDevice : public core::RHIDevice
	{
		friend class rhi::vulkan::RHICommandQueue;
	public:
		struct GraphicsQueueFamilyIndices
		{
			std::optional<uint32_t> GraphicsFamily;
			bool IsComplete() { return GraphicsFamily.has_value(); }
		};
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Create(HWND hwnd, HINSTANCE hInstance, bool useRaytracing = false) override;
		std::shared_ptr<core::RHIFence>            CreateFence()          override  { return nullptr; };
		std::shared_ptr<core::RHICommandList>      CreateCommandList(const std::shared_ptr<rhi::core::RHICommandAllocator>& allocator) override { return nullptr; };
		std::shared_ptr<core::RHICommandQueue>     CreateCommandQueue() override { return nullptr; };
		std::shared_ptr<core::RHICommandAllocator> CreateCommandAllocator() override { return nullptr; };
		std::shared_ptr<core::RHISwapchain>        CreateSwapchain(const std::shared_ptr<rhi::core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0) override;

		size_t GetGraphicsQueueFamilyIndex() { return _queueFamilyIndex.GraphicsFamily.value(); }
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		static VkInstance GetInstance() { return _instance; }
		VkDevice          GetDevice()   { return _logicalDevice; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDevice() = default;
		~RHIDevice();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		size_t AllocateQueue() { return 0; };
		void FreeQueue(const size_t index){};
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		static inline VkInstance _instance = nullptr;
		std::vector<const char*> _deviceExtensions;
		static inline std::vector<const char*> _instanceLayers;
		static inline bool _enableValidationLayer = true;

		VkDebugUtilsMessengerEXT         _debugMessenger = nullptr;

		VkPhysicalDevice                 _physicalDevice = nullptr;
		VkPhysicalDeviceFeatures         _physicalDeviceFeatures = {};
		VkPhysicalDeviceMemoryProperties _memoryProperties       = {};

		GraphicsQueueFamilyIndices _queueFamilyIndex;
		std::vector<size_t> _freeQueues;

		VkSurfaceKHR                     _surface;
		/** @ brief: Logical device representation (application's view of the device)  */
		VkDevice      _logicalDevice    = nullptr;
		/** @ brief: Vulkan API Version.*/
		std::uint32_t _vulkanAPIVersion = VK_API_VERSION_1_3;
		
		bool _useRaytracing = false;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		bool CheckValidationLayerSupport();
		bool CreateInstance();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetUpDebugMessenger();
		void SelectPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		void LogQueueList(VkPhysicalDevice device);
		void CreateSurface();
		void CreateLogicalDevice(bool useRaytracing);
		void FindGraphicsQueueFamilies(VkPhysicalDevice device);
	};
}
#endif