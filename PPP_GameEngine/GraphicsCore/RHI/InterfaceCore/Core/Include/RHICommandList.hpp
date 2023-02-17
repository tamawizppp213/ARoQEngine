//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIDevice.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMANDLIST_HPP
#define RHI_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHICommandAllocator;
	class RHIRenderPass;
	class RHIFrameBuffer;
	class RHIResourceLayout;
	class GPUGraphicsPipelineState;
	class GPUComputePipelineState;
	class GPUResource;
	class GPUTexture;
	class GPUBuffer;
	class RHIDescriptorHeap;

	/****************************************************************************
	*				  			RHIDevice
	*************************************************************************//**
	*  @class     RHIDevice
	*  @brief     Device interface
	*****************************************************************************/
	class RHICommandList : public NonCopyable, std::enable_shared_from_this<RHICommandList>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*-------------------------------------------------------------------
		-               Draw Frame Function
		---------------------------------------------------------------------*/
		/* @brief : This function must be called at draw function initially (stillMidFrame = false). 
		            If still mid frame is set false, this function clears the command allocator.*/
		virtual void BeginRecording(const bool stillMidFrame = false) = 0;

		/* @brief : This function must be called at draw function at end, 
		            The command list is closed, it transits the executable state.*/
		virtual void EndRecording  () = 0; // Call end function at end

		virtual void BeginRenderPass(const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIFrameBuffer>& frameBuffer) = 0;
		
		virtual void EndRenderPass() = 0;

		virtual void Reset(const std::shared_ptr<RHICommandAllocator>& changeAllocator = nullptr) = 0;

		/*-------------------------------------------------------------------
		-               Common command
		---------------------------------------------------------------------*/
		virtual void SetResourceLayout(const std::shared_ptr<RHIResourceLayout>& resourceLayout) = 0;
		virtual void SetDescriptorHeap(const std::shared_ptr<RHIDescriptorHeap>& heap) = 0;
		//virtual void SetConstant32Bits(std::vector<Value32Bit>& values) = 0;
		//virtual void CopyBuffer(const std::shared_ptr<GPUBuffer>& source, const std::shared_ptr<GPUBuffer>& destination, const size_t size, const size_t sourceOffset = 0, const size_t destinationOffset = 0) = 0;*/
		//virtual void TransitLayout(const std::shared_ptr<GPUTexture>& texture, const ResourceLayout& newLayout) = 0;
		//virtual void TransitLayout(const std::shared_ptr<GPUBuffer>& buffer, const ResourceLayout& newLayout) = 0;

		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		virtual void SetPrimitiveTopology(const PrimitiveTopology topology) = 0;

		virtual void SetViewport          (const Viewport* viewport, const std::uint32_t numViewport = 1) = 0;
		
		virtual void SetScissor           (const ScissorRect* rect , const std::uint32_t numRect = 1)     = 0;
		
		virtual void SetViewportAndScissor(const Viewport& viewport, const ScissorRect& rect)       = 0;
		
		virtual void SetVertexBuffer      (const std::shared_ptr<GPUBuffer>& buffer) = 0;
		
		virtual void SetVertexBuffers     (const std::vector<std::shared_ptr<GPUBuffer>>& buffers, const size_t startSlot = 0) = 0;
		
		virtual void SetIndexBuffer       (const std::shared_ptr<GPUBuffer>& buffer, const IndexType indexType = IndexType::UInt32) = 0;
		
		virtual void SetGraphicsPipeline  (const std::shared_ptr<GPUGraphicsPipelineState>& pipeline) = 0;
		
		virtual void DrawIndexed          (std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0) = 0;
		
		virtual void DrawIndexedInstanced (std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t startInstanceLocation = 0) = 0;
		
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		virtual void SetComputeResourceLayout(const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) = 0;
		
		virtual void SetComputePipeline(const std::shared_ptr<GPUComputePipelineState>& pipeline) = 0;
		
		virtual void Dispatch(std::uint32_t threadGroupCountX  = 1, std::uint32_t threadGroupCountY = 1, std::uint32_t threadGroupCountZ = 1) = 0;
		
		/*-------------------------------------------------------------------
		-                RayTracing Command
		---------------------------------------------------------------------*/
		
		/*-------------------------------------------------------------------
		-                Copy Resource
		---------------------------------------------------------------------*/
		virtual void CopyResource(const std::shared_ptr<GPUTexture>& dest, const std::shared_ptr<GPUTexture>& source) = 0;
		
		/*-------------------------------------------------------------------
		-                Transition layout
		---------------------------------------------------------------------*/
		virtual void TransitionResourceState (const std::shared_ptr<core::GPUTexture>& texture, core::ResourceState after) = 0;
		
		virtual void TransitionResourceStates(const std::uint32_t numStates, const std::shared_ptr<core::GPUTexture>* textures, core::ResourceState* afters) = 0;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		std::shared_ptr<RHICommandAllocator> GetCommandAllocator() const noexcept { return _commandAllocator; }
		
		/* @brief : Command list type (graphics, copy, or compute)*/
		CommandListType GetType() const { return _commandListType; }
		
		bool IsOpen() const { return _isOpen; }

		void SetDevice(std::shared_ptr<RHIDevice> device) { _device = device; }

		virtual void SetName(const std::wstring& name) = 0;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandList()          = default;
		
		virtual ~RHICommandList() = default;
		
		explicit RHICommandList(
			const std::shared_ptr<RHIDevice>& device,
			const std::shared_ptr<RHICommandAllocator>& commandAllocator)
			: _device(device), _commandAllocator(commandAllocator) { };

	protected:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice>           _device           = nullptr;

		std::shared_ptr<RHICommandAllocator> _commandAllocator = nullptr;

		std::shared_ptr<core::RHIRenderPass> _renderPass       = nullptr;

		std::shared_ptr<core::RHIFrameBuffer>_frameBuffer      = nullptr;

		core::CommandListType _commandListType = CommandListType::Unknown;
		bool _isOpen = false;
		bool _beginRenderPass = false;
	};
}
#endif