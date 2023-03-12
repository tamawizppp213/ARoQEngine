//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIDevice.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_DEVICE_HPP
#define RHI_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayoutElement.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>
#include <optional>
#include <map> // vulkanだめだったらunordered_mapも追加.
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct Float3x4;
}
namespace rhi::core
{
	class RHIDisplayAdapter;
	class RHIFrameBuffer;
	class RHIFence;
	class RHICommandList;
	class RHICommandQueue;
	class RHICommandAllocator;
	class RHISwapchain;
	class RHIDescriptorHeap;
	class RHIResourceLayout;
	class RHIRenderPass;
	class GPUGraphicsPipelineState;
	class GPUComputePipelineState;
	class GPURayTracingPipelineState;
	class GPUResourceView;
	class GPUSampler;
	class GPUBuffer;
	class GPUTexture;
	class GPUPipelineFactory;
	class RayTracingGeometry;
	class BLASBuffer;
	class TLASBuffer;
	class ASInstance;
	struct SwapchainDesc;

	/****************************************************************************
	*				  			RHIDevice
	*************************************************************************//**
	*  @class     RHIDevice 
	*  @brief     Logical Device interface. Device : GPU resources generator.
	*****************************************************************************/
	class RHIDevice : public NonCopyable, public std::enable_shared_from_this<RHIDevice> 
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Destroy() = 0; // shared_ptrでデストラクタが呼ばれないため. 

		/*-------------------------------------------------------------------
		-               Create Objects for Logical Device
		---------------------------------------------------------------------*/
		bool IsDiscreteGPU() const;
		
#pragma region Create Resource
		virtual void                                        SetUpDefaultHeap(const core::DefaultHeapCount& heapCount) = 0;
		
		virtual std::shared_ptr<RHIFrameBuffer>             CreateFrameBuffer(const std::shared_ptr<RHIRenderPass>& renderPass, const std::vector<std::shared_ptr<GPUTexture>>& renderTargets, const std::shared_ptr<GPUTexture>& depthStencil = nullptr) = 0;
		
		virtual std::shared_ptr<RHIFrameBuffer>             CreateFrameBuffer(const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<GPUTexture>& renderTarget, const std::shared_ptr<GPUTexture>& depthStencil = nullptr) = 0;
		
		virtual std::shared_ptr<RHIFence>                   CreateFence(const std::uint64_t fenceValue = 0, const std::wstring& name = L"Fence") = 0;
		
		virtual std::shared_ptr<RHICommandList>             CreateCommandList(const std::shared_ptr<RHICommandAllocator>& commandAllocator, const std::wstring& name = L"CommandList") = 0;
		
		virtual std::shared_ptr<RHICommandQueue>            CreateCommandQueue(const core::CommandListType type, const std::wstring& name = L"CommandQueue") = 0;
		
		virtual std::shared_ptr<RHICommandAllocator>        CreateCommandAllocator(const core::CommandListType type, const std::wstring& name = L"CommandAllocator") = 0;
		
		virtual std::shared_ptr<RHISwapchain>               CreateSwapchain(const std::shared_ptr<RHICommandQueue>& commandQueue, const WindowInfo& windowInfo, const PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0, const bool isValidHDR = true) = 0;
		
		virtual std::shared_ptr<RHISwapchain>               CreateSwapchain(const SwapchainDesc& desc) = 0;

		virtual std::shared_ptr<RHIDescriptorHeap>          CreateDescriptorHeap(const DescriptorHeapType heapType, const size_t maxDescriptorCount) = 0;
		
		virtual std::shared_ptr<RHIDescriptorHeap>          CreateDescriptorHeap(const std::map<DescriptorHeapType, size_t>& heapInfo) = 0;
		
		virtual std::shared_ptr<RHIResourceLayout>          CreateResourceLayout(const std::vector<ResourceLayoutElement>& elements = {}, const std::vector<SamplerLayoutElement>& samplers = {}, const std::optional<Constant32Bits>& constant32Bits = std::nullopt, const std::wstring& name = L"ResourceLayout") = 0;
		
		virtual std::shared_ptr<GPUPipelineFactory>         CreatePipelineFactory() = 0;
		
		virtual std::shared_ptr<GPUGraphicsPipelineState>   CreateGraphicPipelineState(const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIResourceLayout>& resourceLayout) = 0; // after action: setting pipeline
		
		virtual std::shared_ptr<GPUComputePipelineState>    CreateComputePipelineState(const std::shared_ptr<RHIResourceLayout>& resourceLayout) = 0; // after action: setting pipeline
		
		virtual std::shared_ptr<RHIRenderPass>              CreateRenderPass(const std::vector<Attachment>& colors, const std::optional<Attachment>& depth) = 0;
		
		virtual std::shared_ptr<RHIRenderPass>              CreateRenderPass(const Attachment& color, const std::optional<Attachment>& depth) = 0;
		
		virtual std::shared_ptr<GPUResourceView>            CreateResourceView(const ResourceViewType viewType, const std::shared_ptr<GPUTexture>& texture, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap = nullptr) = 0;
		
		virtual std::shared_ptr<GPUResourceView>            CreateResourceView(const ResourceViewType viewType, const std::shared_ptr<GPUBuffer>& buffer, const std::shared_ptr<RHIDescriptorHeap>& customHeap = nullptr) = 0;
		
		virtual std::shared_ptr<GPUSampler>                 CreateSampler(const core::SamplerInfo& samplerInfo) = 0; // both
		
		virtual std::shared_ptr<GPUBuffer>                  CreateBuffer (const core::GPUBufferMetaData& metaData, const std::wstring& name = L"") = 0;
		
		virtual std::shared_ptr<GPUTexture>                 CreateTexture(const core::GPUTextureMetaData& metaData, const std::wstring& name = L"") = 0;
		
		virtual std::shared_ptr<GPUTexture>                 CreateTextureEmpty() = 0;
		
		//virtual std::shared_ptr<GPURayTracingPipelineState> CreateRayTracingPipelineState(const std::shared_ptr<RHIResourceLayout>& resourceLayout) = 0;
		
		virtual std::shared_ptr<RayTracingGeometry>         CreateRayTracingGeometry(const RayTracingGeometryFlags flags, const std::shared_ptr<GPUBuffer>& vertexBuffer, const std::shared_ptr<GPUBuffer>& indexBuffer = nullptr) = 0;
		
		virtual std::shared_ptr<ASInstance>                 CreateASInstance(
			const std::shared_ptr<BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform, 
			const std::uint32_t instanceID, const std::uint32_t instanceContributionToHitGroupIndex, 
			const std::uint32_t instanceMask = 0xFF, 
			const RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None) = 0;
		
		virtual std::shared_ptr<BLASBuffer>                 CreateRayTracingBLASBuffer(const std::vector<std::shared_ptr<RayTracingGeometry>>& geometryDesc, const BuildAccelerationStructureFlags flags) = 0;
		
		virtual std::shared_ptr<TLASBuffer>                 CreateRayTracingTLASBuffer(const std::vector<std::shared_ptr<ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags) = 0;
		
#pragma endregion Create Resource
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		virtual std::shared_ptr<RHIDescriptorHeap> GetDefaultHeap(const core::DescriptorHeapType heapType) = 0;
		
		virtual std::uint32_t GetShadingRateImageTileSize() const = 0;
		
		std::shared_ptr<RHIDisplayAdapter> GetDisplayAdapter() const noexcept { return _adapter; }
		
		virtual void SetName(const std::wstring& name) = 0;

		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		virtual bool IsSupportedDxr                () const = 0; // Ray Tracing 

		virtual bool IsSupportedHDR                () const = 0; // HDR check ()

		virtual bool IsSupportedVariableRateShading() const = 0;

		virtual bool IsSupportedMeshShading        () const = 0;

		virtual bool IsSupportedDrawIndirected     () const = 0;

		virtual bool IsSupportedGeometryShader     () const = 0;

		virtual bool IsSupportedRenderPass         () const = 0;

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDevice()  = default;

		virtual ~RHIDevice()
		{ 
			_adapter.reset(); 
		}

		RHIDevice(const std::shared_ptr<RHIDisplayAdapter>& adapter) 
			: _adapter(adapter) {};

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : Use Display Apapter (GPU)*/
		std::shared_ptr<RHIDisplayAdapter> _adapter = nullptr;

	};
}
#endif