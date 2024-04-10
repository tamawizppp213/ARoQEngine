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
		
		/*!**********************************************************************
		*  @brief     ビューポートによって描画領域を設定します. シザー矩形もViewportに合わせて自動で設定します
		*  @param[in] const core::Viewport& : 描画領域を示す単一のビューポート
		*************************************************************************/
		virtual void SetViewport(const core::Viewport& viewport) override {  };

		/*!**********************************************************************
		*  @brief     ビューポートの配列(アドレス)を入れて描画領域を設定します. シザー矩形もViewportに合わせて自動で設定します
		*  @param[in] const core::Viewport* : 描画領域を記述した配列, もしくは単一のViewportのアドレス
		*  @param[in] const gu::uint32 : ビューポートの配列数 (Defaultは1)
		*************************************************************************/
		void SetViewport (const core::Viewport* viewport, const gu::uint32 numViewport = 1)override;
		
		/*!**********************************************************************
		*  @brief     VRのような立体視を行う時に設定する描画領域です
		*  @param[in] const core::Viewport& 左側の視野を示す描画領域
		*  @param[in] const core::Viewport& 右側の視野を示す描画領域
		*************************************************************************/
		virtual void SetStereoViewport(const core::Viewport& leftView, const core::Viewport& rightView) override {};

		/*!**********************************************************************
		*  @brief     ビューポート内で実際に描画される領域を制限するためのシザー矩形を手動で設定します.
		*  @param[in] const core::ScissorRect* : 描画領域を制限するためのシザー矩形の配列
		*  @param[in] const gu::uint32 : シザー矩形の配列数
		*************************************************************************/
		virtual void SetScissor(const core::ScissorRect* rect, const gu::uint32 numRect = 1) override;

		/*!**********************************************************************
		*  @brief     描画領域を示すビューポートと, その中で実際に描画される範囲を指定するシザー矩形をそれぞれ手動で設定します.
		*  @param[in] const core::Viewport& 描画領域を示すビューポート
		*  @param[in] const core::ScissorRect& 実際に描画される範囲を示すシザー矩形
		*************************************************************************/
		virtual void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override;

		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		void SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipeline) override {};
		
		void SetVertexBuffer (const gu::SharedPointer<core::GPUBuffer>& buffer) override;
		
		void SetVertexBuffers(const gu::DynamicArray<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot = 0) override;
		
		void SetIndexBuffer  (const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType = core::IndexType::UInt32) override;
		
		void DrawIndexed(gu::uint32 indexCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0)override;
		
		void DrawIndexedInstanced(gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0, gu::uint32 startInstanceLocation = 0)override;
		
		/*----------------------------------------------------------------------
		*  @brief :インデックスバッファを持つモデルに対して, 引数バッファをGPUで設定, 描画を実行出来る関数です
		/*----------------------------------------------------------------------*/
		void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount) override {};

		/*----------------------------------------------------------------------
		*  @brief :Mesh shaderで使用する描画関数です.
		/*----------------------------------------------------------------------*/
		void DispatchMesh(const gu::uint32 threadGroupCountX = 1, const gu::uint32 threadGroupCountY = 1, const gu::uint32 threadGroupCountZ = 1) override {};
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override{};
		
		void Dispatch(gu::uint32 threadGroupCountX = 1, gu::uint32 threadGroupCountY = 1, gu::uint32 threadGroupCountZ = 1)override;
		/*-------------------------------------------------------------------
		-                Transition Resource State
		---------------------------------------------------------------------*/
		void TransitionResourceState(const gu::SharedPointer<core::GPUTexture>& texture, core::ResourceState after) override;
		
		void TransitionResourceStates(const gu::uint32 numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters) override;
		
		void CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source) override {};;
		
		/*----------------------------------------------------------------------
		*  @brief : バッファの領域をあるリソースから別のリソースにコピーする. GPU版memcpy
		/*----------------------------------------------------------------------*/
		void CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) override {};

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkCommandBuffer GetCommandList() { return _commandBuffer; }
		
		void SetName(const gu::tstring& name);
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandList() = default;
		
		~RHICommandList();
		
		explicit RHICommandList(
			const gu::SharedPointer<core::RHIDevice>& device,
			const gu::SharedPointer<core::RHICommandAllocator>& allocator,
			const gu::tstring& name);
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
	inline void rhi::vulkan::RHICommandList::DrawIndexed(gu::uint32 indexCount, gu::uint32 startIndexLocation, gu::uint32 baseVertexLocation)
	{
		vkCmdDrawIndexed(_commandBuffer, indexCount, 1, startIndexLocation, baseVertexLocation, 0);
	}
	inline void rhi::vulkan::RHICommandList::DrawIndexedInstanced(gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation, gu::uint32 baseVertexLocation, gu::uint32 startInstanceLocation)
	{
		vkCmdDrawIndexed(_commandBuffer, indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startIndexLocation);
	}
	inline void rhi::vulkan::RHICommandList::Dispatch(gu::uint32 threadGroupCountX, gu::uint32 threadGroupCountY, gu::uint32 threadGroupCountZ)
	{
		vkCmdDispatch(_commandBuffer, threadGroupCountX, threadGroupCountY, threadGroupCountZ);
	}
}
#endif