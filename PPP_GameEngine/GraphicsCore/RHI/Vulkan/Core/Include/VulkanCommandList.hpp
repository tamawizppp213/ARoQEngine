//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanCommandList.hpp
///             @brief  CommandList
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_COMMANDLIST_HPP
#define VULKAN_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4100)
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	class RHIResourceLayout;
	class GPUGraphicsPipelineState;
	/****************************************************************************
	*				  			TemplateClass
	*************************************************************************//**
	*  @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	class RHICommandList : public rhi::core::RHICommandList, public gu::EnableSharedFromThis<RHICommandList>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : This function must be called at draw function initially (stillMidFrame = false).
					If still mid frame is set false, this function clears the command allocator.*/
		void BeginRecording(const bool stillMidFrame) override; 
		
		void EndRecording () override;  // end call   at draw frame
		
		void BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer) override;
		
		void EndRenderPass() override;
		
		/* @brief : Proceed to the record state.*/
		void Reset(const gu::SharedPointer<core::RHICommandAllocator>& changeAllocator) override {};

#pragma region Query
		/*----------------------------------------------------------------------
		*  @brief : GPU情報を取得するためのクエリを開始します
		/*----------------------------------------------------------------------*/
		void BeginQuery(const core::QueryResultLocation& location) override {};

		/*----------------------------------------------------------------------
		*  @brief : GPU情報を取得するためのクエリを終了します
		/*----------------------------------------------------------------------*/
		void EndQuery(const core::QueryResultLocation& location) override {};

#pragma endregion Query
		/*-------------------------------------------------------------------
		-               Graphic Pipeline command
		---------------------------------------------------------------------*/
		void SetDepthBounds(const float minDepth, const float maxDepth) override {};

		void SetDescriptorHeap(const gu::SharedPointer<core::RHIDescriptorHeap>& heap) override {};
		
		void SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& layout) override;
		
		void SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipeline);
		
		void SetPrimitiveTopology(const core::PrimitiveTopology topology) override;
		
		void SetViewport          (const core::Viewport* viewport, const std::uint32_t numViewport = 1)override;
		
		void SetScissor           (const core::ScissorRect* rect , const std::uint32_t numRect = 1) override;
		
		void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override;
		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		void SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipeline) override {};
		
		void SetVertexBuffer (const gu::SharedPointer<core::GPUBuffer>& buffer) override;
		
		void SetVertexBuffers(const std::vector<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot = 0) override;
		
		void SetIndexBuffer  (const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType = core::IndexType::UInt32) override;
		
		void DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0)override;
		
		void DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t startInstanceLocation = 0)override;
		
		/*----------------------------------------------------------------------
		*  @brief :インデックスバッファを持つモデルに対して, 引数バッファをGPUで設定, 描画を実行出来る関数です
		/*----------------------------------------------------------------------*/
		void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const std::uint32_t drawCallCount) override {};

		/*----------------------------------------------------------------------
		*  @brief :Mesh shaderで使用する描画関数です.
		/*----------------------------------------------------------------------*/
		void DispatchMesh(const std::uint32_t threadGroupCountX = 1, const std::uint32_t threadGroupCountY = 1, const std::uint32_t threadGroupCountZ = 1) override {};
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override{};
		
		void Dispatch(std::uint32_t threadGroupCountX = 1, std::uint32_t threadGroupCountY = 1, std::uint32_t threadGroupCountZ = 1)override;
		/*-------------------------------------------------------------------
		-                Transition Resource State
		---------------------------------------------------------------------*/
		void TransitionResourceState(const gu::SharedPointer<core::GPUTexture>& texture, core::ResourceState after) override;
		
		void TransitionResourceStates(const std::uint32_t numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters) override;
		
		void CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source) override {};;
		
		/*----------------------------------------------------------------------
		*  @brief : バッファの領域をあるリソースから別のリソースにコピーする. GPU版memcpy
		/*----------------------------------------------------------------------*/
		void CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) override {};

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkCommandBuffer GetCommandList() { return _commandBuffer; }
		
		void SetName(const gu::wstring& name);
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandList() = default;
		
		~RHICommandList();
		
		explicit RHICommandList(
			const gu::SharedPointer<core::RHIDevice>& device,
			const gu::SharedPointer<core::RHICommandAllocator>& allocator,
			const gu::wstring& name);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkCommandBuffer _commandBuffer = nullptr;
		
		gu::SharedPointer<RHIResourceLayout> _resourceLayout = nullptr;
	
		bool _isFirstFrame = true;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		VkAccessFlags SelectVkAccessFlag(const VkImageLayout imageLayout);
	};
	inline void rhi::vulkan::RHICommandList::DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation)
	{
		vkCmdDrawIndexed(_commandBuffer, indexCount, 1, startIndexLocation, baseVertexLocation, 0);
	}
	inline void rhi::vulkan::RHICommandList::DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation, std::uint32_t startInstanceLocation)
	{
		vkCmdDrawIndexed(_commandBuffer, indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startIndexLocation);
	}
	inline void rhi::vulkan::RHICommandList::Dispatch(std::uint32_t threadGroupCountX, std::uint32_t threadGroupCountY, std::uint32_t threadGroupCountZ)
	{
		vkCmdDispatch(_commandBuffer, threadGroupCountX, threadGroupCountY, threadGroupCountZ);
	}
}
#endif