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
	class RHIDevice : public core::RHIDevice, public gu::EnableSharedFromThis<RHIDevice>
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
		void Destroy() override;

#pragma region Create Resource
		void                                           SetUpDefaultHeap(const core::DefaultHeapCount& heapCount) override;

		gu::SharedPointer<core::RHIFrameBuffer>          CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const std::vector<gu::SharedPointer<core::GPUTexture>>& renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr) override;

		gu::SharedPointer<core::RHIFrameBuffer>          CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr) override;
		
		gu::SharedPointer<core::RHIFence>                CreateFence(const std::uint64_t fenceValue = 0, const gu::tstring& name = SP(""))          override;
		
		gu::SharedPointer<core::RHICommandList>          CreateCommandList(const gu::SharedPointer<rhi::core::RHICommandAllocator>& allocator, const gu::tstring& name) override;
		
		gu::SharedPointer<core::RHICommandQueue>         CreateCommandQueue(const core::CommandListType type, const gu::tstring& name) override;
		
		gu::SharedPointer<core::RHICommandAllocator>     CreateCommandAllocator(const core::CommandListType type, const gu::tstring& name) override;
		
		gu::SharedPointer<core::RHISwapchain>            CreateSwapchain(const gu::SharedPointer<rhi::core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 3, const std::uint32_t vsync = 0, const bool isValidHDR = true) override;
		
		gu::SharedPointer<core::RHISwapchain>            CreateSwapchain(const core::SwapchainDesc& desc) override;

		gu::SharedPointer<core::RHIDescriptorHeap>       CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount) override;
		
		gu::SharedPointer<core::RHIDescriptorHeap>       CreateDescriptorHeap(const std::map<core::DescriptorHeapType, size_t>& heapInfo) override;
		
		gu::SharedPointer<core::RHIResourceLayout>       CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements = {}, const std::vector<core::SamplerLayoutElement>& samplers = {}, const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt, const gu::tstring& name=SP("ResourceLayout")) override;

		gu::SharedPointer<core::GPUPipelineFactory>      CreatePipelineFactory() override;

		gu::SharedPointer<core::GPUGraphicsPipelineState> CreateGraphicPipelineState(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;  // after action: setting pipeline

		gu::SharedPointer<core::GPUComputePipelineState> CreateComputePipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override; // after action: setting pipeline

		gu::SharedPointer<core::RHIRenderPass>           CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth) override;
		
		gu::SharedPointer<core::RHIRenderPass>           CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth) override;
		
		//gu::SharedPointer<core::GPURayTracingPipelineState>CreateRayTracingPipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override { return nullptr; };
		
		
		gu::SharedPointer<core::GPUResourceView>         CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUTexture>& texture, const gu::uint32 mipSlice = 0, const gu::uint32 planeSlice = 0, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		gu::SharedPointer<core::GPUResourceView>         CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUBuffer>& buffer, const gu::uint32 mipSlice = 0, const gu::uint32 planeSlice = 0, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		gu::SharedPointer<core::GPUSampler>              CreateSampler(const core::SamplerInfo& samplerInfo) override; // both
		
		gu::SharedPointer<core::GPUBuffer>               CreateBuffer(const core::GPUBufferMetaData& metaData, const gu::tstring& name = SP("Buffer")) override;
		
		gu::SharedPointer<core::GPUTexture>              CreateTexture(const core::GPUTextureMetaData& metaData, const gu::tstring& name = SP("Texture")) override;
		
		gu::SharedPointer<core::GPUTexture>              CreateTextureEmpty() override;
		
		gu::SharedPointer<core::RayTracingGeometry>      CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const gu::SharedPointer<core::GPUBuffer>& vertexBuffer, const gu::SharedPointer<core::GPUBuffer>& indexBuffer = nullptr) override;
		
		gu::SharedPointer<core::ASInstance>              CreateASInstance(
			const gu::SharedPointer<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform,
			const std::uint32_t instanceID, const std::uint32_t instanceContributionToHitGroupIndex,
			const std::uint32_t instanceMask = 0xFF, const core::RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None) override;
		
		gu::SharedPointer<core::BLASBuffer>                 CreateRayTracingBLASBuffer(const std::vector<gu::SharedPointer<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags) override;
		
		gu::SharedPointer<core::TLASBuffer>                 CreateRayTracingTLASBuffer(const std::vector<gu::SharedPointer<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags) override;

		gu::SharedPointer<core::RHIQuery> CreateQuery([[maybe_unused]] const core::QueryHeapType heapType) override { return nullptr; };
#pragma endregion Create Resource
		size_t GetQueueFamilyIndex(const core::CommandListType type) { return _commandQueueInfo[type].QueueFamilyIndex; }
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		inline VkDevice GetDevice() { return _logicalDevice; }
		
		//inline VkSurfaceKHR      GetSurface() { return _surface; }
		
		/* @brief : 使いたいメモリの種類を持っているmemoryIndexを返す.*/
		std::uint32_t  GetMemoryTypeIndex(std::uint32_t typeBits, const VkMemoryPropertyFlags& flags);
		
		std::uint32_t GetQueueCount(const rhi::core::CommandListType type) { return _commandQueueInfo[type].QueueCount; }

		std::uint32_t GetShadingRateImageTileSize() const { return 0; };
		
		gu::SharedPointer<core::RHIDescriptorHeap>   GetDefaultHeap(const core::DescriptorHeapType heapType) override;
		
		std::uint64_t GetDeviceAddress(VkBuffer buffer);

		void SetName(const gu::tstring& name) override;

		// @note : !!! prohibit japanese name. To convert utf8f string!!!
		void SetVkResourceName(const gu::tstring& name, const VkObjectType type, const std::uint64_t objectHandle);

		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		bool IsSupportedDxr                () const override { return _isSupportedRayTracing; };
		bool IsSupportedHDR                () const override { return _isSupportedHDR; };
		bool IsSupportedVariableRateShading() const override { return _isSupportedVariableRateShading; };
		bool IsSupportedMeshShading        () const override { return _isSupportedMeshShading; };
		bool IsSupportedDrawIndirected     () const override { return _isSupportedDrawIndirected; };
		bool IsSupportedGeometryShader     () const override { return _isSupportedGeometryShader; };
		bool IsSupportedRenderPass         () const override { return true; }
		bool IsSupportedDepthBoundsTest    () const override { return true; }
		bool IsSupportedSamplerFeedback    ()const override { return false; }
		bool IsSupportedStencilReferenceFromPixelShader() const override { return true; }
		bool IsSupportedWaveLane() const override { return true; }
		bool IsSupportedNative16bitOperation() const override { return true; }
		bool IsSupportedAtomicOperation() const override { return true; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDevice() = default;
		
		~RHIDevice();
		
		RHIDevice(const gu::SharedPointer<core::RHIDisplayAdapter>& adapter);
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
		// base class : std::unordered_map<CommandListType, gu::SharedPointer<core::RHICommandQueue>> _commandQueues;
		std::map<core::CommandListType, QueueInfo> _commandQueueInfo;
		std::map<core::CommandListType, std::uint32_t> _newCreateCommandQueueIndex;

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
		void SetUpCommandQueueInfo();
		void CheckSupports(); // Deviceのすべてのサポート内容を調べ, メンバ変数のboolに代入する.
		void CreateLogicalDevice();

		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		gu::SharedPointer<core::RHIDescriptorHeap> _defaultHeap = nullptr;
	};
}
#endif