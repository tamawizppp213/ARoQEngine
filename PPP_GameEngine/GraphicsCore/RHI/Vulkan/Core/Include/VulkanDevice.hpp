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
		bool Create(HWND hwnd, HINSTANCE hInstance, bool useHDR = false, bool useRaytracing = false) override;
		
		std::shared_ptr<core::RHIFrameBuffer>          CreateFrameBuffer(const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override;
		std::shared_ptr<core::RHIFrameBuffer>          CreateFrameBuffer(const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override ;
		std::shared_ptr<core::RHIFence>                CreateFence()          override  { return nullptr; };
		std::shared_ptr<core::RHICommandList>          CreateCommandList(const std::shared_ptr<rhi::core::RHICommandAllocator>& allocator) override;
		std::shared_ptr<core::RHICommandQueue>         CreateCommandQueue() override;
		std::shared_ptr<core::RHICommandAllocator>     CreateCommandAllocator() override;
		std::shared_ptr<core::RHISwapchain>            CreateSwapchain(const std::shared_ptr<rhi::core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0) override;
		std::shared_ptr<core::RHIDescriptorHeap>       CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount) override;
		std::shared_ptr<core::RHIDescriptorHeap>       CreateDescriptorHeap(const std::vector<core::DescriptorHeapType>& heapTypes, const std::vector<size_t>& maxDescriptorCounts) override;
		std::shared_ptr<core::RHIRenderPass>           CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth) override;
		std::shared_ptr<core::RHIRenderPass>           CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth) override;
		std::shared_ptr<core::GPUGraphicsPipelineState> CreateGraphicPipelineState(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override;  // after action: setting pipeline
		std::shared_ptr<core::GPUComputePipelineState> CreateComputePipelineState(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override; // after action: setting pipeline
		std::shared_ptr<core::RHIResourceLayout>       CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements = {}, const std::vector<core::SamplerLayoutElement>& samplers = {}, const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt) override;
		std::shared_ptr<core::GPUPipelineFactory>      CreatePipelineFactory() override;
		std::shared_ptr<core::GPUSampler>              CreateSampler(const core::SamplerInfo& samplerInfo); // both
		std::shared_ptr<core::GPUBuffer>               CreateBuffer(const core::GPUBufferMetaData& metaData) override;
		std::shared_ptr<core::GPUTexture>              CreateTexture(const core::GPUTextureMetaData& metaData) override;
		size_t AllocateQueue();
		void FreeQueue(const size_t index);
		size_t GetGraphicsQueueFamilyIndex() { return _queueFamilyIndex.GraphicsFamily.value(); }
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		static VkInstance        GetInstance()       { return _instance; }
		inline VkDevice          GetDevice()         { return _logicalDevice; }
		inline VkPhysicalDevice  GetPhysicalDevice() { return _physicalDevice; }
		inline VkSurfaceKHR      GetSurface()        { return _surface; }
		std::uint32_t            GetMemoryTypeIndex(std::uint32_t typeBits, const VkMemoryPropertyFlags& flags);
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDevice() = default;
		~RHIDevice();
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
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