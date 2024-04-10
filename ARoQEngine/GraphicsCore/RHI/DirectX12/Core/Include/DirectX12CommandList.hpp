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
	*				  			RHICommandList
	*************************************************************************//**
	*  @class     RHICommandList
	*  @brief     GPUの描画命令をまとめたクラス. BeginRecordingとEndRecordingは1フレームの開始と終了時に呼んでください.
	*****************************************************************************/
	class RHICommandList : public rhi::core::RHICommandList, public gu::EnableSharedFromThis<RHICommandList>
	{
	public:
		#pragma region Public Function
		#pragma region Main Draw Frame 
		/*!**********************************************************************
		*  @brief     コマンドリストを記録状態に変更します. これはDraw関数の最初に使用します @n
		*  @param[in] 描画フレーム中に呼ばれる場合にコマンドアロケータの中身をResetするかを決定するbool値.@n
		*             描画フレーム中に呼ぶのは, コマンドリストを切り替える際に使用される可能性があるためです.
		*************************************************************************/
		virtual void BeginRecording(const bool stillMidFrame) override;

		/*!**********************************************************************
		*  @brief  コマンドリストを記録状態から実行可能状態に変更します. これはDraw関数の最後に使用します
		*************************************************************************/
		virtual void EndRecording () override;

		/*----------------------------------------------------------------------
		*  @brief : RenderPassを開始します.基本的には各Draw関数のBeginRecordingの後に呼ばれます
		/*----------------------------------------------------------------------*/
		virtual void BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer) override;
		
		/*----------------------------------------------------------------------
		*  @brief : RenderPassを終了します.基本的には各Draw関数のEndRecording前に呼ばれます
		/*----------------------------------------------------------------------*/
		virtual void EndRenderPass() override;

		/*!**********************************************************************
		*  @brief     コマンドリストを記録状態に変更します.またコマンドアロケータ中のコマンドバッファの内容を先頭に戻します.
		*  @param[in] コマンドアロケータを変更するかどうか
		*  @attention 基本的には, ResetではなくBeginRecordingを使用してください.
		*************************************************************************/
		virtual void Reset(const gu::SharedPointer<core::RHICommandAllocator>& changeAllocator = nullptr) override;

		/*-------------------------------------------------------------------
		-               Common command
		---------------------------------------------------------------------*/
		void SetDescriptorHeap(const gu::SharedPointer<core::RHIDescriptorHeap>& heap) override;

		#pragma endregion Main Draw Frame
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
		/*!**********************************************************************
		*  @brief     深度が指定の範囲に入っているかをテストし, 範囲内ならばピクセルシェーダーを動作させます.
		*  @param[in] const float : 最小の深度情報
		*  @param[in] const float : 最大の深度情報
		*  @return    void
		*************************************************************************/
		virtual void SetDepthBounds(const float minDepth, const float maxDepth) override;

		/*!**********************************************************************
		*  @brief     頂点情報のつなぎ方を設定します. 
		*  @param[in] const core::PrimitiveTopology : プリミティブのトポロジー種類
		*************************************************************************/
		virtual void SetPrimitiveTopology(const core::PrimitiveTopology topology) override;

		/*!**********************************************************************
		*  @brief     ビューポートによって描画領域を設定します. シザー矩形もViewportに合わせて自動で設定します
		*  @param[in] const core::Viewport& : 描画領域を示す単一のビューポート
		*************************************************************************/
		virtual void SetViewport(const core::Viewport& viewport) override;

		/*!**********************************************************************
		*  @brief     ビューポートの配列(アドレス)を入れて描画領域を設定します. シザー矩形もViewportに合わせて自動で設定します
		*  @param[in] const core::Viewport* : 描画領域を記述した配列, もしくは単一のViewportのアドレス
		*  @param[in] const gu::uint32 : ビューポートの配列数 (Defaultは1)
		*************************************************************************/
		virtual void SetViewport(const core::Viewport* viewport, const gu::uint32 numViewport) override;
		
		/*!**********************************************************************
		*  @brief     VRのような立体視を行う時に設定する描画領域です.シザー矩形もViewportに合わせて自動で設定します
		*  @param[in] const core::Viewport& 左側の視野を示す描画領域
		*  @param[in] const core::Viewport& 右側の視野を示す描画領域
		*************************************************************************/
		virtual void SetStereoViewport(const core::Viewport& leftView, const core::Viewport& rightView) override ;

		/*!**********************************************************************
		*  @brief     ビューポート内で実際に描画される領域を制限するためのシザー矩形を手動で設定します.
		*  @param[in] const core::ScissorRect* : 描画領域を制限するためのシザー矩形の配列
		*  @param[in] const gu::uint32 : シザー矩形の配列数
		*************************************************************************/
		virtual void SetScissor (const core::ScissorRect* rect, const gu::uint32 numRect = 1) override;
		
		/*!**********************************************************************
		*  @brief     描画領域を示すビューポートと, その中で実際に描画される範囲を指定するシザー矩形をそれぞれ手動で設定します.
		*  @param[in] const core::Viewport& 描画領域を示すビューポート
		*  @param[in] const core::ScissorRect& 実際に描画される範囲を示すシザー矩形
		*************************************************************************/
		virtual void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override;
		
		void SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		void SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipelineState) override;
		
		void SetVertexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer) override ;
		
		void SetVertexBuffers(const gu::DynamicArray<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot = 0) override;
		
		void SetIndexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType = core::IndexType::UInt32) override;
		
		/*----------------------------------------------------------------------
		*  @brief : インデックスがついているモデルでかつ, インスタンシング描画が必要となるプリミティブを描画します.
		*           indexCountPerInstance : インスタンス毎に必要となるインデックスの総数
		*           instance Count        : インスタンスの数
		*           startIndexLocation    : インデックスを読み取り始める, インデックスバッファ中の配列要素数
		* 　　　　　　 baseVertexLocation    : 頂点バッファーから頂点を読み取る前に, 各インデックスに追加する値
		*           startInstanceLocation : 描画を行う最初のインスタンス番号
		/*----------------------------------------------------------------------*/
		void DrawIndexedInstanced(gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0, gu::uint32 startInstanceLocation = 0) override;
		
		/*----------------------------------------------------------------------
		*  @brief : インデックスがついているモデルでかつ, インスタンシング描画が必要ないプリミティブを描画します.
		/*----------------------------------------------------------------------*/
		void DrawIndexed(gu::uint32 indexCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0) override;
		
		/*----------------------------------------------------------------------
		*  @brief :インデックスバッファを持つモデルに対して, 引数バッファをGPUで設定, 描画を実行出来る関数です
		/*----------------------------------------------------------------------*/
		void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount) override;

		/*----------------------------------------------------------------------
		*  @brief :Mesh shaderで使用する描画関数です. 
		/*----------------------------------------------------------------------*/
		void DispatchMesh(const gu::uint32 threadGroupCountX = 1, const gu::uint32 threadGroupCountY = 1, const gu::uint32 threadGroupCountZ = 1) override;
        #pragma endregion Graphics Command Function
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		void SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipeline) override;
		
		void Dispatch(gu::uint32 threadGroupCountX = 1, gu::uint32 threadGroupCountY = 1, gu::uint32 threadGroupCountZ = 1) override;
		
		/*-------------------------------------------------------------------
		-                Transition layout
		---------------------------------------------------------------------*/
		void TransitionBarrierState (const gu::SharedPointer<core::GPUTexture>& texture, core::BarrierState after) override ;

		void TransitionBarrierStates(const gu::uint32 numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::BarrierState* afters) override ;

		void TransitionBarrierStates(const gu::DynamicArray<gu::SharedPointer<core::GPUResource>>& resources, core::BarrierState* afters);
		
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
		
		#pragma endregion 
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