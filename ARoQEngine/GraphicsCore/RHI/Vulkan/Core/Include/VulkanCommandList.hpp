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
	****************************************************************************/
	/* @class     TemplateClass
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
		*----------------------------------------------------------------------*/
		void BeginQuery(const core::QueryResultLocation& location) override {};

		/*----------------------------------------------------------------------
		*  @brief : GPU情報を取得するためのクエリを終了します
		*----------------------------------------------------------------------*/
		void EndQuery(const core::QueryResultLocation& location) override {};

#pragma endregion Query
		/*-------------------------------------------------------------------
		-               Graphic Pipeline command
		---------------------------------------------------------------------*/
		void SetDepthBounds(const float minDepth, const float maxDepth) override {};

		/*!**********************************************************************
		*  @brief     Variable Rate ShadingをGraphics Pipeline上で有効化します. PerTile, PerPrimitiveを使用する場合はSetShadingRateImageも使用してください
		*  @note      https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC168%E5%9B%9E
		*  @oaram[in] 描画ピクセルの単位
		*  @param[in] const gu::DynamicArray<core::ShadingRateCombiner>& Imageの結合方法
		*  @return    void
		*************************************************************************/
		virtual void SetShadingRate(const core::ShadingRate shadingRate, const gu::DynamicArray<core::ShadingRateCombiner>& combiners) override {};

		/*!**********************************************************************
		*  @brief     VariableRateShading : ピクセルシェーダーの起動を1ピクセルごとではなく, 複数ピクセルを合わせて1回のシェーダー起動で処理するためのイメージを設定
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : VariableRateShadingを適用するテクスチャ
		*  @return    void
		*************************************************************************/
		virtual void SetShadingRateImage(const gu::SharedPointer<core::GPUTexture>& texture) override {};

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
		
		void SetIndexBuffer  (const gu::SharedPointer<core::GPUBuffer>& buffer, const core::PixelFormat indexType = core::PixelFormat::R32_UINT) override;
		
		void DrawIndexed(const gu::uint32 indexCount, const gu::uint32 startIndexLocation = 0, const gu::uint32 baseVertexLocation = 0)override;
		
		void DrawIndexedInstanced(const gu::uint32 indexCountPerInstance, const gu::uint32 instanceCount, const gu::uint32 startIndexLocation = 0, const gu::uint32 baseVertexLocation = 0, const gu::uint32 startInstanceLocation = 0)override;
		
		/*----------------------------------------------------------------------
		*  @brief :インデックスバッファを持つモデルに対して, 引数バッファをGPUで設定, 描画を実行出来る関数です
		*----------------------------------------------------------------------*/
		void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount) override {};

		/*----------------------------------------------------------------------
		*  @brief :Mesh shaderで使用する描画関数です.
		*----------------------------------------------------------------------*/
		void DispatchMesh(const gu::uint32 threadGroupCountX = 1, const gu::uint32 threadGroupCountY = 1, const gu::uint32 threadGroupCountZ = 1) override {};
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override{};
		
		void Dispatch(gu::uint32 threadGroupCountX = 1, gu::uint32 threadGroupCountY = 1, gu::uint32 threadGroupCountZ = 1)override;
		/*-------------------------------------------------------------------
		-                Transition Resource State
		---------------------------------------------------------------------*/
		//void TransitionResourceState(const gu::SharedPointer<core::GPUTexture>& texture, core::ResourceState after) override;
		
		//void TransitionResourceStates(const gu::uint32 numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters) override;
		
		void CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source) override {};;
		
		/*----------------------------------------------------------------------
		*  @brief : バッファの領域をあるリソースから別のリソースにコピーする. GPU版memcpy
		*----------------------------------------------------------------------*/
		void CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) override {};

		/*!**********************************************************************
		*  @brief     GPUテクスチャの領域をあるGPUポインタから別のGPUポインタにコピーを行う. GPU版のmemcpy
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : コピー先のテクスチャ
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : コピー元のテクスチャ
		*  @param[in] const core::GPUTextureCopyInfo : コピーする際の情報
		*  @return void
		*************************************************************************/
		virtual void CopyTextureRegion(const gu::SharedPointer<core::GPUTexture>& destination, const gu::SharedPointer<core::GPUTexture>& source, const core::GPUTextureCopyInfo& copyInfo) override {};
		#pragma region Resource Barrier
		
		/*!**********************************************************************
		*  @brief  単独のGPUリソースの状態遷移を示します. @n
		*          リソースの使い方が変わるタイミングで呼び出します.
		*  @param[in] const gu::SharedPointer<core::GPUBuffer> GPUバッファ
		*  @param[in] const core::ResourceState 遷移前のリソース状態
		*  @param[in] const core::ResourceState 遷移後のリソース状態
		*  @param[in] const gu::uint32 サブリソースを示すインデックス (デフォルトは全てのサブリソースを示します) 0xfffffffの場合は全てのインデックスで有効化されます
		*  @return    void
		*************************************************************************/
		virtual void PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& buffer, const core::ResourceState after, const gu::uint32 subresource = 0xffffffff) override {};

		/*!**********************************************************************
		*  @brief  同じメモリ領域にマッピングされた複数のGPUリソースに対し, 使用するリソース自体を切り替える際に使用します. @n
		*          同時に使用しないことが担保されているリソースのメモリを節約することが可能となります. @n
		*          ただし, 本関数を使用する場合は,CreateCommittedResourceでは無く, CreatePlacedResourceを使用した方法でヒープの確保を行ってください.@n
		*          (別々のヒープを作ってしまうことになり, 同じメモリ領域を扱わなくなるため.)
		*  @note   https://logicalbeat.jp/blog/8185/ (AliasingBarrierの活用方法についての記述)
		*  @param[in] const gu::SharedPointer<core::GPUResource> 切り替える前に使用していたGPUリソース
		*  @param[in] const gu::SharedPointer<core::GPUResource> 切り替える前に使用していたGPUリソース
		*  @return    void
		*************************************************************************/
		virtual void PushAliasingBarrier(const gu::SharedPointer<core::GPUResource>& before, const gu::SharedPointer<core::GPUResource>& after) override {};

		/*!**********************************************************************
		*  @brief     Unordered access view専用の状態バリア @n
		*             UAVの読み書き中にほかのUAVが対象リソースを読み書きする描画コマンドの実行を防ぐことを目的とします @n
		*  @param[in] const gu::SharedPointer<core::GPUResource> Unordered access viewを持つGPUリソース
		*  @return    void
		*************************************************************************/
		virtual void PushUAVBarrier(const gu::SharedPointer<core::GPUResource>& buffer) override {};

		/*!**********************************************************************
		*  @brief  コマンドリストを使ってResourceBarrierをまとめて呼び出します.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void FlushResourceBarriers() override {};
		
		#pragma endregion Resource Barrier
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		VkCommandBuffer GetCommandList() const { return _commandBuffer; }
		
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
		**                Protected Property
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