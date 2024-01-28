//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIDevice.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_COMMANDLIST_HPP
#define DIRECTX12_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"
#include "DirectX12Core.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	class RHIDevice;
	class RHICommandAllocator;
	class RHIRenderPass;
	class RHIFrameBuffer;

	/****************************************************************************
	*				  			RHIDevice
	*************************************************************************//**
	*  @class     RHIDevice
	*  @brief     Device interface
	*****************************************************************************/
	class RHICommandList : public rhi::core::RHICommandList, public gu::EnableSharedFromThis<RHICommandList>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : コマンドリストを記録状態に変更します. これはDraw関数の最初に使用します
		*           stillMidFrameは, コマンドアロケーターの中身をResetするかを決定します.
		/*----------------------------------------------------------------------*/
		void BeginRecording(const bool stillMidFrame) override;

		/*----------------------------------------------------------------------
		*  @brief : コマンドリストを記録状態から実行可能状態に変更します. これはDraw関数の最後に使用します
		/*----------------------------------------------------------------------*/
		void EndRecording  () override;

		/*----------------------------------------------------------------------
		*  @brief : RenderPassを開始します.基本的には各Draw関数のBeginRecordingの後に呼ばれます
		/*----------------------------------------------------------------------*/
		void BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer) override;
		
		/*----------------------------------------------------------------------
		*  @brief : RenderPassを終了します.基本的には各Draw関数のEndRecording前に呼ばれます
		/*----------------------------------------------------------------------*/
		void EndRenderPass() override;

		/*----------------------------------------------------------------------
		*  @brief : Proceed to the record state. コマンドリストを記録状態に変更します.
		            基本的には, ResetではなくBeginRecordingを使用してください.
		/*----------------------------------------------------------------------*/
		void Reset(const gu::SharedPointer<core::RHICommandAllocator>& changeAllocator = nullptr) override;

		/*-------------------------------------------------------------------
		-               Common command
		---------------------------------------------------------------------*/
		void SetDescriptorHeap(const gu::SharedPointer<core::RHIDescriptorHeap>& heap) override;

#pragma region Query
		/*----------------------------------------------------------------------
		*  @brief : GPU情報を取得するためのクエリを開始します
		/*----------------------------------------------------------------------*/
		void BeginQuery(const core::QueryResultLocation& location) override;

		/*----------------------------------------------------------------------
		*  @brief : GPU情報を取得するためのクエリを終了します
		/*----------------------------------------------------------------------*/
		void EndQuery(const core::QueryResultLocation& location) override;
#pragma endregion Query
#pragma region Graphics Command Function
		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief : 深度が指定の範囲に入っているかをテストし, 範囲内ならばピクセルシェーダーを動作させます.
		/*----------------------------------------------------------------------*/
		void SetDepthBounds(const float minDepth, const float maxDepth) override;

		void SetPrimitiveTopology(const core::PrimitiveTopology topology) override;

		void SetViewport(const core::Viewport* viewport, const std::uint32_t numViewport = 1) override;
		
		void SetScissor (const core::ScissorRect* rect, const std::uint32_t numRect = 1) override;
		
		void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override;
		
		void SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		void SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipelineState) override;
		
		void SetVertexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer) override ;
		
		void SetVertexBuffers(const std::vector<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot = 0) override;
		
		void SetIndexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType = core::IndexType::UInt32) override;
		
		/*----------------------------------------------------------------------
		*  @brief : インデックスがついているモデルでかつ, インスタンシング描画が必要となるプリミティブを描画します.
		*           indexCountPerInstance : インスタンス毎に必要となるインデックスの総数
		*           instance Count        : インスタンスの数
		*           startIndexLocation    : インデックスを読み取り始める, インデックスバッファ中の配列要素数
		* 　　　　　　 baseVertexLocation    : 頂点バッファーから頂点を読み取る前に, 各インデックスに追加する値
		*           startInstanceLocation : 描画を行う最初のインスタンス番号
		/*----------------------------------------------------------------------*/
		void DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t startInstanceLocation = 0) override;
		
		/*----------------------------------------------------------------------
		*  @brief : インデックスがついているモデルでかつ, インスタンシング描画が必要ないプリミティブを描画します.
		/*----------------------------------------------------------------------*/
		void DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0) override;
		
		/*----------------------------------------------------------------------
		*  @brief :インデックスバッファを持つモデルに対して, 引数バッファをGPUで設定, 描画を実行出来る関数です
		/*----------------------------------------------------------------------*/
		void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const std::uint32_t drawCallCount) override;

		/*----------------------------------------------------------------------
		*  @brief :Mesh shaderで使用する描画関数です. 
		/*----------------------------------------------------------------------*/
		void DispatchMesh(const std::uint32_t threadGroupCountX = 1, const std::uint32_t threadGroupCountY = 1, const std::uint32_t threadGroupCountZ = 1) override;
#pragma endregion Graphics Command Function
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		void SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipeline) override;
		
		void Dispatch(std::uint32_t threadGroupCountX = 1, std::uint32_t threadGroupCountY = 1, std::uint32_t threadGroupCountZ = 1) override;
		
		/*-------------------------------------------------------------------
		-                Transition layout
		---------------------------------------------------------------------*/
		void TransitionResourceState (const gu::SharedPointer<core::GPUTexture>& texture, core::ResourceState after) override ;

		void TransitionResourceStates(const std::uint32_t numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters) override ;

		void TransitionResourceStates(const std::vector<gu::SharedPointer<core::GPUResource>>& resources, core::ResourceState* afters);
		
		/*-------------------------------------------------------------------
		-                Copy
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief : バッファの領域をあるリソースから別のリソースにコピーする. GPU memcpy
		/*----------------------------------------------------------------------*/
		void CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) override;

		/*----------------------------------------------------------------------
		*  @brief : テクスチャの領域をまとめて別のリソースにコピーする
		/*----------------------------------------------------------------------*/
		void CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source) override;
		
		/*----------------------------------------------------------------------
		*  @brief : あるリソースの領域をまとめて別のリソースにコピーする. 
		*           組み合わせに応じて自動でバッファかテクスチャかを判定します
		/*----------------------------------------------------------------------*/
		void CopyResource(const gu::SharedPointer<core::GPUResource>& dest, const gu::SharedPointer<core::GPUResource>& source);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		CommandListComPtr GetCommandList() const noexcept { return _commandList; }

		void SetName(const gu::tstring& name) override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandList() = default;

		~RHICommandList();

		explicit RHICommandList(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator, 
			const gu::tstring& name);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		CommandListComPtr _commandList = nullptr;
		
	private:
		void BeginRenderPassImpl(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer);
		
		void OMSetFrameBuffer   (const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer);
	};
}
#endif