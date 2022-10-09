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
		struct QueueInfo
		{
			std::uint32_t QueueFamilyIndex = 0;
			std::uint32_t QueueCount = 0;
		};
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void SetUp() override;
		void Destroy() override;
#pragma region Create Resource
		std::shared_ptr<core::RHIFrameBuffer>          CreateFrameBuffer(const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override;
		std::shared_ptr<core::RHIFrameBuffer>          CreateFrameBuffer(const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override;
		std::shared_ptr<core::RHIFence>                CreateFence(const std::uint64_t fenceValue = 0)          override;
		std::shared_ptr<core::RHICommandList>          CreateCommandList(const std::shared_ptr<rhi::core::RHICommandAllocator>& allocator) override;
		std::shared_ptr<core::RHICommandQueue>         CreateCommandQueue(const core::CommandListType type) override;
		std::shared_ptr<core::RHICommandAllocator>     CreateCommandAllocator() override;
		std::shared_ptr<core::RHISwapchain>            CreateSwapchain(const std::shared_ptr<rhi::core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 3, const std::uint32_t vsync = 0, const bool isValidHDR = true) override;
		std::shared_ptr<core::RHIDescriptorHeap>       CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount) override;
		std::shared_ptr<core::RHIDescriptorHeap>       CreateDescriptorHeap(const std::map<core::DescriptorHeapType, size_t>& heapInfo) override;
		std::shared_ptr<core::RHIRenderPass>           CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth) override;
		std::shared_ptr<core::RHIRenderPass>           CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth) override;
		std::shared_ptr<core::GPUGraphicsPipelineState> CreateGraphicPipelineState(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override;  // after action: setting pipeline
		std::shared_ptr<core::GPUComputePipelineState> CreateComputePipelineState(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override; // after action: setting pipeline
		std::shared_ptr<core::RHIResourceLayout>       CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements = {}, const std::vector<core::SamplerLayoutElement>& samplers = {}, const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt) override;
		std::shared_ptr<core::GPUPipelineFactory>      CreatePipelineFactory() override;
		std::shared_ptr<core::GPUSampler>              CreateSampler(const core::SamplerInfo& samplerInfo); // both
		std::shared_ptr<core::GPUBuffer>               CreateBuffer(const core::GPUBufferMetaData& metaData) override;
		std::shared_ptr<core::GPUTexture>              CreateTexture(const core::GPUTextureMetaData& metaData) override;
#pragma endregion Create Resource
		size_t GetQueueFamilyIndex(const core::CommandListType type) { return _commandQueueInfo[type].QueueFamilyIndex; }
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		inline VkDevice          GetDevice() { return _logicalDevice; }
		//inline VkSurfaceKHR      GetSurface() { return _surface; }
		std::uint32_t            GetMemoryTypeIndex(std::uint32_t typeBits, const VkMemoryPropertyFlags& flags);
		std::uint32_t GetShadingRateImageTileSize() const { return 0; };
		std::shared_ptr<core::RHICommandQueue> GetCommandQueue(const core::CommandListType commandListType) override;
		std::shared_ptr<core::RHICommandAllocator> GetCommandAllocator(const core::CommandListType commandListType, const std::uint32_t frameCount = 0) override;
		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		bool IsSupportedDxr                () const override { return _isSupportedRayTracing; };
		bool IsSupportedHDR                () const override { return _isSupportedHDR; };
		bool IsSupportedVariableRateShading() const override { return _isSupportedVariableRateShading; };
		bool IsSupportedMeshShading        () const override { return _isSupportedMeshShading; };
		bool IsSupportedDrawIndirected     () const override { return _isSupportedDrawIndirected; };
		bool IsSupportedGeometryShader     () const override { return _isSupportedGeometryShader; };
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDevice() = default;
		~RHIDevice();
		RHIDevice(const std::shared_ptr<core::RHIDisplayAdapter>& adapter, const std::uint32_t frameCount);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/** @ brief: Logical device representation (application's view of the device)  */
		VkDevice      _logicalDevice    = nullptr;

		/* @brief : Command queue set up data*/
		// base class : std::unordered_map<CommandListType, std::shared_ptr<core::RHICommandQueue>> _commandQueues;
		std::map<core::CommandListType, QueueInfo>     _commandQueueInfo;

		/* @brief : variable shading rate : 1, 2, 4, 8, 16, 32, 64*/
		std::uint32_t _shadingRateImageTileSize = 0;
#pragma region Support check flags
		bool _isSupportedRayTracing          = false;
		bool _isSupportedTearing             = false;
		bool _isSupportedHDR                 = true;
		bool _isSupportedVariableRateShading = false;
		bool _isSupportedMeshShading         = false;
		bool _isSupportedRayQuery            = false;
		bool _isSupportedGeometryShader      = false;
		bool _isSupportedTessellation        = false;
		bool _isSupportedDrawIndirected      = false;
#pragma endregion Support check flags
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void SetUpCommandQueue();
		void SetUpCommandPool();
		void CheckSupports(); // Deviceのすべてのサポート内容を調べ, メンバ変数のboolに代入する.
		void CreateLogicalDevice();
	};
}
#endif