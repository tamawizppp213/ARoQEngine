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
#include <unordered_map>
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
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
	class GPUSampler;
	class GPUBuffer;
	class GPUTexture;
	class GPUPipelineFactory;
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
		virtual void SetUp() = 0;
		virtual void Destroy() = 0; // shared_ptrでデストラクタが呼ばれないため. 
		/*-------------------------------------------------------------------
		-               Create Objects for Logical Device
		---------------------------------------------------------------------*/
#pragma region Create Resource
		virtual std::shared_ptr<RHIFrameBuffer>           CreateFrameBuffer(const std::vector<std::shared_ptr<GPUTexture>>& renderTargets, const std::shared_ptr<GPUTexture>& depthStencil = nullptr) = 0;
		virtual std::shared_ptr<RHIFrameBuffer>           CreateFrameBuffer(const std::shared_ptr<GPUTexture>& renderTarget, const std::shared_ptr<GPUTexture>& depthStencil = nullptr) = 0;
		virtual std::shared_ptr<RHIFence>                 CreateFence(const std::uint64_t fenceValue = 0) = 0;
		virtual std::shared_ptr<RHICommandList>           CreateCommandList(const std::shared_ptr<RHICommandAllocator>& commandAllocator) = 0;
		virtual std::shared_ptr<RHICommandQueue>          CreateCommandQueue(const core::CommandListType type) = 0;
		virtual std::shared_ptr<RHICommandAllocator>      CreateCommandAllocator() = 0;
		virtual std::shared_ptr<RHISwapchain>             CreateSwapchain(const std::shared_ptr<RHICommandQueue>& commandQueue, const WindowInfo& windowInfo, const PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0, const bool isValidHDR = true) = 0;
		virtual std::shared_ptr<RHIDescriptorHeap>        CreateDescriptorHeap(const DescriptorHeapType heapType, const size_t maxDescriptorCount) = 0;
		virtual std::shared_ptr<RHIDescriptorHeap>        CreateDescriptorHeap(const std::vector<DescriptorHeapType>& heapTypes, const std::vector<size_t>& maxDescriptorCounts) = 0;
		virtual std::shared_ptr<RHIResourceLayout>        CreateResourceLayout(const std::vector<ResourceLayoutElement>& elements = {}, const std::vector<SamplerLayoutElement>& samplers = {}, const std::optional<Constant32Bits>& constant32Bits = std::nullopt) = 0;
		virtual std::shared_ptr<GPUPipelineFactory>       CreatePipelineFactory() = 0;
		virtual std::shared_ptr<GPUGraphicsPipelineState> CreateGraphicPipelineState(const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIResourceLayout>& resourceLayout) = 0; // after action: setting pipeline
		virtual std::shared_ptr<GPUComputePipelineState>  CreateComputePipelineState(const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIResourceLayout>& resourceLayout) = 0; // after action: setting pipeline
		virtual std::shared_ptr<RHIRenderPass>            CreateRenderPass(const std::vector<Attachment>& colors, const std::optional<Attachment>& depth) = 0;
		virtual std::shared_ptr<RHIRenderPass>            CreateRenderPass(const Attachment& color, const std::optional<Attachment>& depth) = 0;
		virtual std::shared_ptr<GPUSampler>               CreateSampler(const core::SamplerInfo& samplerInfo) = 0; // both
		virtual std::shared_ptr<GPUBuffer>                CreateBuffer (const core::GPUBufferMetaData& metaData) = 0;
		virtual std::shared_ptr<GPUTexture>               CreateTexture(const core::GPUTextureMetaData& metaData) = 0;
#pragma endregion Create Resource
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		virtual std::shared_ptr<RHICommandQueue>     GetCommandQueue    (const core::CommandListType commandListType) = 0;
		virtual std::shared_ptr<RHICommandAllocator> GetCommandAllocator(const core::CommandListType commandListType, const std::uint32_t frameCount = 0) = 0;
		virtual std::uint32_t GetShadingRateImageTileSize() const = 0;
		std::shared_ptr<RHIDisplayAdapter> GetDisplayAdapter() { return _adapter; }
		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		virtual bool IsSupportedDxr                () const = 0;
		virtual bool IsSupportedHDR                () const = 0;
		virtual bool IsSupportedVariableRateShading() const = 0;
		virtual bool IsSupportedMeshShading        () const = 0;
		virtual bool IsSupportedDrawIndirected     () const = 0;
		virtual bool IsSupportedGeometryShader     () const = 0;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		RHIDevice()  = default;
		virtual ~RHIDevice()
		{ 
			_adapter.reset(); 
			_commandQueues.clear();
		}
		RHIDevice(const std::shared_ptr<RHIDisplayAdapter>& adapter, const std::uint32_t frameCount) : _adapter(adapter), _frameCount(frameCount) {};
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDisplayAdapter> _adapter = nullptr; //
		std::unordered_map<CommandListType, std::shared_ptr<RHICommandQueue>> _commandQueues;
		std::vector<std::unordered_map<CommandListType, std::shared_ptr<RHICommandAllocator>>> _commandAllocators;
		std::uint32_t _frameCount = 0;
	};
}
#endif