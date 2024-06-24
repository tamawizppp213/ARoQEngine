//////////////////////////////////////////////////////////////////////////////////
///  @file   RHICommandList.hpp
///  @brief  GPUの描画命令をまとめたクラス. BeginRecordingとEndRecordingは1フレームの開始と終了時に呼んでください. 
///  @author Toide Yutaro
///  @date   2024_04_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMANDLIST_HPP
#define RHI_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
	class RHIQuery;
	struct QueryResultLocation;
	class GPUGraphicsPipelineState;
	class GPUComputePipelineState;
	class GPUResource;
	class GPUTexture;
	class GPUBuffer;
	class RHIDescriptorHeap;

	/****************************************************************************
	*				  			RHICommandList
	****************************************************************************/
	/* @class     RHICommandList
	*  @brief     GPUの描画命令をまとめたクラス. BeginRecordingとEndRecordingは1フレームの開始と終了時に呼んでください. 
	*****************************************************************************/
	class RHICommandList : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		#pragma region Main Draw Frame 
		/*!**********************************************************************
		*  @brief     コマンドリストを記録状態に変更します. これはDraw関数の最初に使用します @n
		*  @param[in] const bool stillMidFrame : 
		*             描画フレーム中に呼ばれる場合にコマンドアロケータの中身をResetするかを決定するbool値.@n
		*             描画フレーム中に呼ぶのは, コマンドリストを切り替える際に使用される可能性があるためです. 
		*************************************************************************/
		virtual void BeginRecording(const bool stillMidFrame = false) = 0;

		/*!**********************************************************************
		*  @brief     コマンドリストを記録状態から実行可能状態に変更します. これはDraw関数の最後に使用します
		*************************************************************************/
		virtual void EndRecording  () = 0; // Call end function at end

		/*----------------------------------------------------------------------
		*  @brief : Start the render pass. This function basically called at draw function at start
		*----------------------------------------------------------------------*/
		virtual void BeginRenderPass(const gu::SharedPointer<RHIRenderPass>& renderPass, const gu::SharedPointer<RHIFrameBuffer>& frameBuffer) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief : Finish the render pass. This function basically called at draw function at end
		*----------------------------------------------------------------------*/
		virtual void EndRenderPass() = 0;

		/*!**********************************************************************
		*  @brief     コマンドリストを記録状態に変更します.またコマンドアロケータ中のコマンドバッファの内容を先頭に戻します.
		*  @param[in] コマンドアロケータを変更するかどうか
		*  @attention 基本的には, ResetではなくBeginRecordingを使用してください.
		*************************************************************************/
		virtual void Reset(const gu::SharedPointer<RHICommandAllocator>& changeAllocator = nullptr) = 0;

		/*!**********************************************************************
		*  @brief     コマンドリストをアロケータに新規に詰め込める状態にあるかを確認します
		*  @param[in] void
		*  @return    bool : 新規に詰め込める場合にtrueになる
		*************************************************************************/
		bool IsOpen() const { return _isOpen; }

		/*!**********************************************************************
		*  @brief     コマンドリストが閉じて新規にアロケータに詰め込めない状態であるかを確認します. @n
		*             閉じた状態になったら, コマンドキューのExecute関数を呼び出してGPU命令を実行してください. 
		*  @param[in] void
		*  @return    bool : コマンドリストが閉じた状態でtrueを返す
		*************************************************************************/
		bool IsClosed() const { return !_isOpen; }

		#pragma endregion Main Draw Frame
		/*-------------------------------------------------------------------
		-               Common command
		---------------------------------------------------------------------*/
		virtual void SetResourceLayout(const gu::SharedPointer<RHIResourceLayout>& resourceLayout) = 0;
		virtual void SetDescriptorHeap(const gu::SharedPointer<RHIDescriptorHeap>& heap) = 0;
		//virtual void SetConstant32Bits(gu::DynamicArray<Value32Bit>& values) = 0;
		//virtual void CopyBuffer(const gu::SharedPointer<GPUBuffer>& source, const gu::SharedPointer<GPUBuffer>& destination, const size_t size, const size_t sourceOffset = 0, const size_t destinationOffset = 0) = 0;*/
		//virtual void TransitLayout(const gu::SharedPointer<GPUTexture>& texture, const ResourceLayout& newLayout) = 0;
		//virtual void TransitLayout(const gu::SharedPointer<GPUBuffer>& buffer, const ResourceLayout& newLayout) = 0;

		#pragma region Query
		/*----------------------------------------------------------------------
		*  @brief :  Starts the query to get GPU information.
		*----------------------------------------------------------------------*/
		virtual void BeginQuery(const QueryResultLocation& location) = 0;

		/*----------------------------------------------------------------------
		*  @brief :  End the query to get GPU information.
		*----------------------------------------------------------------------*/
		virtual void EndQuery(const QueryResultLocation& location) = 0;
		#pragma endregion Query

		#pragma region Graphics Command Function
		/*!**********************************************************************
		*  @brief     深度が指定の範囲に入っているかをテストし, 範囲内ならばピクセルシェーダーを動作させます.
		*  @param[in] const float : 最小の深度情報
		*  @param[in] const float : 最大の深度情報
		*  @return    void
		*************************************************************************/
		virtual void SetDepthBounds(const float minDepth, const float maxDepth) = 0;

		/*!**********************************************************************
		*  @brief     頂点情報のつなぎ方を設定します.
		*  @param[in] プリミティブのトポロジー種類
		*************************************************************************/
		virtual void SetPrimitiveTopology(const PrimitiveTopology topology) = 0;

		/*!**********************************************************************
		*  @brief     ビューポートによって描画領域を設定します. シザー矩形もViewportに合わせて自動で設定します
		*  @param[in] const core::Viewport& : 描画領域を示す単一のビューポート
		*************************************************************************/
		virtual void SetViewport(const core::Viewport& viewport) = 0;

		/*!**********************************************************************
		*  @brief     ビューポートの配列(アドレス)を入れて描画領域を設定します. シザー矩形もViewportに合わせて自動で設定します
		*  @param[in] const core::Viewport* : 描画領域を記述した配列, もしくは単一のViewportのアドレス
		*  @param[in] const gu::uint32 : ビューポートの配列数
		*************************************************************************/
		virtual void SetViewport (const Viewport* viewport, const gu::uint32 numViewport = 1) = 0;
		
		/*!**********************************************************************
		*  @brief     VRのような立体視を行う時に設定する描画領域です. シザー矩形もViewportに合わせて自動で設定します
		*  @param[in] const core::Viewport& 左側の視野を示す描画領域
		*  @param[in] const core::Viewport& 右側の視野を示す描画領域
		*************************************************************************/
		virtual void SetStereoViewport(const Viewport& leftView, const Viewport& rightView) = 0;

		/*!**********************************************************************
		*  @brief     ビューポート内で実際に描画される領域を制限するためのシザー矩形を手動で設定します. 
		*  @param[in] const core::ScissorRect* : 描画領域を制限するためのシザー矩形の配列
		*  @param[in] const gu::uint32 : シザー矩形の配列数
		*************************************************************************/
		virtual void SetScissor (const ScissorRect* rect , const gu::uint32 numRect = 1) = 0;
		
		/*!**********************************************************************
		*  @brief     描画領域を示すビューポートと, その中で実際に描画される範囲を指定するシザー矩形をそれぞれ手動で設定します.
		*  @param[in] const core::Viewport& 描画領域を示すビューポート
		*  @param[in] const core::ScissorRect& 実際に描画される範囲を示すシザー矩形
		*************************************************************************/
		virtual void SetViewportAndScissor(const Viewport& viewport, const ScissorRect& rect)       = 0;
		
		virtual void SetVertexBuffer(const gu::SharedPointer<GPUBuffer>& buffer) = 0;
		
		virtual void SetVertexBuffers     (const gu::DynamicArray<gu::SharedPointer<GPUBuffer>>& buffers, const size_t startSlot = 0) = 0;
		
		virtual void SetIndexBuffer       (const gu::SharedPointer<GPUBuffer>& buffer, const PixelFormat indexType = PixelFormat::R32_UINT) = 0;
		
		virtual void SetGraphicsPipeline  (const gu::SharedPointer<GPUGraphicsPipelineState>& pipeline) = 0;
		
		virtual void DrawIndexed          (gu::uint32 indexCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief : インデックスがついているモデルでかつ, インスタンシング描画が必要となるプリミティブを描画します.
		*           indexCountPerInstance : インスタンス毎に必要となるインデックスの総数
		*           instance Count        : インスタンスの数
		*           startIndexLocation    : インデックスを読み取り始める, インデックスバッファ中の配列要素数
		* 　　　　　　 baseVertexLocation    : 頂点バッファーから頂点を読み取る前に, 各インデックスに追加する値
		*           startInstanceLocation : 描画を行う最初のインスタンス番号
		*----------------------------------------------------------------------*/
		virtual void DrawIndexedInstanced (gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0, gu::uint32 startInstanceLocation = 0) = 0;
		
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		virtual void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) = 0;
		
		virtual void SetComputePipeline(const gu::SharedPointer<GPUComputePipelineState>& pipeline) = 0;
		
		virtual void Dispatch(gu::uint32 threadGroupCountX  = 1, gu::uint32 threadGroupCountY = 1, gu::uint32 threadGroupCountZ = 1) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :インデックスバッファを持つモデルに対して, 引数バッファをGPUで設定, 描画を実行出来る関数です
		*----------------------------------------------------------------------*/
		virtual void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount) = 0;

		/*----------------------------------------------------------------------
		*  @brief :Mesh shaderで使用する描画関数です.
		*----------------------------------------------------------------------*/
		virtual void DispatchMesh(const gu::uint32 threadGroupCountX = 1, const gu::uint32 threadGroupCountY = 1, const gu::uint32 threadGroupCountZ = 1) = 0;
		
		/*-------------------------------------------------------------------
		-                RayTracing Command
		---------------------------------------------------------------------*/
		
		#pragma region Copy
		/*!**********************************************************************
		*  @brief     テクスチャの領域を全てのSubresource込みで別のテクスチャにコピーする
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : コピー先のテクスチャ
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : コピー元のテクスチャ
		*  @return void
		*************************************************************************/
		virtual void CopyResource(const gu::SharedPointer<GPUTexture>& dest, const gu::SharedPointer<GPUTexture>& source) = 0;
		
		/*!**********************************************************************
		*  @brief     GPUバッファの領域をあるGPUポインタから別のGPUポインタにコピーを行う. GPU版のmemcpy
		*  @param[in] const gu::SharedPointer<core::GPUBuffer> : コピー先のバッファ
		*  @param[in] const gu::uint64 : コピー先の初期書き取りポインタをずらすoffset byte
		*  @param[in] const gu::SharedPointer<core::GPUBuffer> : コピー元のバッファ
		*  @param[in] const gu::uint64 : コピー元の初期読み取りポインタをずらすoffset byte
		*  @param[in] const gu::uint64 : コピーを行う範囲 (byteSizeで指定)
		*  @return void
		*************************************************************************/
		virtual void CopyBufferRegion(const gu::SharedPointer<GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) = 0;
		
		/*!**********************************************************************
		*  @brief     GPUテクスチャの領域をあるGPUポインタから別のGPUポインタにコピーを行う. GPU版のmemcpy
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : コピー先のテクスチャ
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : コピー元のテクスチャ
		*  @param[in] const core::GPUTextureCopyInfo : コピーする際の情報
		*  @return void
		*************************************************************************/
		virtual void CopyTextureRegion(const gu::SharedPointer<core::GPUTexture>& destination, const gu::SharedPointer<core::GPUTexture>& source, const core::GPUTextureCopyInfo& copyInfo) = 0;
		#pragma endregion Copys

		#pragma region Resource Barrier
		/*!**********************************************************************
		*  @brief  単独のGPUリソースの状態遷移を示します. @n
		*          リソースの使い方が変わるタイミングで呼び出します.
		*  @param[in] const gu::SharedPointer<core::GPUResource> GPUリソース
		*  @param[in] const core::ResourceState 遷移前のリソース状態
		*  @param[in] const core::ResourceState 遷移後のリソース状態
		*  @param[in] const gu::uint32 サブリソースを示すインデックス (デフォルトは全てのサブリソースを示します) 0xfffffffの場合は全てのインデックスで有効化されます
		*  @return    void
		*************************************************************************/
		virtual void PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& buffer  , const core::ResourceState after, const gu::uint32 subresource = 0xffffffff) = 0;
		
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
		virtual void PushAliasingBarrier(const gu::SharedPointer<core::GPUResource>& before  , const gu::SharedPointer<core::GPUResource>& after) = 0;
		
		/*!**********************************************************************
		*  @brief     Unordered access view専用の状態バリア @n
		*             UAVの読み書き中にほかのUAVが対象リソースを読み書きする描画コマンドの実行を防ぐことを目的とします @n
		*  @param[in] const gu::SharedPointer<core::GPUTexture> Unordered access viewを持つGPUバッファ
		*  @return    void
		*************************************************************************/
		virtual void PushUAVBarrier(const gu::SharedPointer<core::GPUResource>& texture) = 0;

		/*!**********************************************************************
		*  @brief  コマンドリストを使ってResourceBarrierをまとめて呼び出します.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void FlushResourceBarriers() = 0;
		
		#pragma endregion Resource Barrier

		#pragma endregion 
		
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     コマンドリストのメモリを格納するコマンドアロケータのポインタを返します
		*  @return    gu::SharedPointer<RHICommandAllocator> コマンドアロケータのポインタ
		*************************************************************************/
		gu::SharedPointer<RHICommandAllocator> GetCommandAllocator() const noexcept { return _commandAllocator; }
		
		/*!**********************************************************************
		*  @brief     現在のコマンドリストの種類を返します. 
		*  @return    CommandListType コマンドリストの種類 [graphics, copy, compute]
		*************************************************************************/
		CommandListType GetType() const { return _commandListType; }
		
		/*!**********************************************************************
		*  @brief     このコマンドリストを特定するための固有IDです. 毎フレームコマンドリストがResetされたとしても残ります.　現在は特に使用していない
		*  @return    コマンドリストを特定する固有ID
		*************************************************************************/
		gu::uint64 GetID() const { return _commandListID; }

		/*!**********************************************************************
		*  @brief     論理デバイスを設定します. 
		*************************************************************************/
		void SetDevice(gu::SharedPointer<RHIDevice> device) { _device = device; }

		/*!**********************************************************************
		*  @brief     デバッグ名を設定します
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		#pragma endregion

		#pragma region Public Constructor and Destructor 

		#pragma endregion
	protected:
		#pragma region Protected Constructor and Destructor 

		/*! @brief デフォルトコンストラクタ*/
		RHICommandList() = default;

		/*! @brief デストラクタ*/
		virtual ~RHICommandList() = default;

		/*! @brief 論理デバイスとコマンドを貯めるアロケータを使って生成するコンストラクタ*/
		explicit RHICommandList(
			const gu::SharedPointer<RHIDevice>& device,
			const gu::SharedPointer<RHICommandAllocator>& commandAllocator)
			: _device(device), _commandAllocator(commandAllocator) { };

		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*! @brief コマンドリストを貯めておくアロケータ*/
		gu::SharedPointer<RHICommandAllocator> _commandAllocator = nullptr;

		/*! @brief 描画用のレンダーパス*/
		gu::SharedPointer<core::RHIRenderPass> _renderPass = nullptr;

		/*! @brief フレームバッファ*/
		gu::SharedPointer<core::RHIFrameBuffer>_frameBuffer = nullptr;

		/*! @brief コマンドリストを特定する固有ID*/
		gu::uint64 _commandListID = 0;

		/*! @brief コマンドリストの種類*/
		core::CommandListType _commandListType = CommandListType::Unknown;

		/*! @brief : コマンドリストが詰め込める状態にあるかを確認します*/
		bool _isOpen = false;

		/*! @brief : レンダーパスを実行しているか*/
		bool _beginRenderPass = false;

		#pragma endregion
		
	};
}
#endif