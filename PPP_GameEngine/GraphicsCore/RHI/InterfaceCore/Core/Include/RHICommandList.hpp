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

		/* @brief : コマンドリストを詰め込み可能な状態に変更します. またコマンドアロケータ中のコマンドバッファの内容を先頭に戻します.
		            基本的には, BeginRecordingを使用してください.*/
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

#pragma region Graphics Command Function
		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief : 深度が指定の範囲に入っているかをテストし, 範囲内ならばピクセルシェーダーを動作させます.
		/*----------------------------------------------------------------------*/
		virtual void SetDepthBounds(const float minDepth, const float maxDepth) = 0;

		virtual void SetPrimitiveTopology(const PrimitiveTopology topology) = 0;

		virtual void SetViewport          (const Viewport* viewport, const std::uint32_t numViewport = 1) = 0;
		
		virtual void SetScissor           (const ScissorRect* rect , const std::uint32_t numRect = 1)     = 0;
		
		virtual void SetViewportAndScissor(const Viewport& viewport, const ScissorRect& rect)       = 0;
		
		virtual void SetVertexBuffer      (const std::shared_ptr<GPUBuffer>& buffer) = 0;
		
		virtual void SetVertexBuffers     (const std::vector<std::shared_ptr<GPUBuffer>>& buffers, const size_t startSlot = 0) = 0;
		
		virtual void SetIndexBuffer       (const std::shared_ptr<GPUBuffer>& buffer, const IndexType indexType = IndexType::UInt32) = 0;
		
		virtual void SetGraphicsPipeline  (const std::shared_ptr<GPUGraphicsPipelineState>& pipeline) = 0;
		
		virtual void DrawIndexed          (std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief : インデックスがついているモデルでかつ, インスタンシング描画が必要となるプリミティブを描画します.
		*           indexCountPerInstance : インスタンス毎に必要となるインデックスの総数
		*           instance Count        : インスタンスの数
		*           startIndexLocation    : インデックスを読み取り始める, インデックスバッファ中の配列要素数
		* 　　　　　　 baseVertexLocation    : 頂点バッファーから頂点を読み取る前に, 各インデックスに追加する値
		*           startInstanceLocation : 描画を行う最初のインスタンス番号
		/*----------------------------------------------------------------------*/
		virtual void DrawIndexedInstanced (std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t startInstanceLocation = 0) = 0;
		
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		virtual void SetComputeResourceLayout(const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) = 0;
		
		virtual void SetComputePipeline(const std::shared_ptr<GPUComputePipelineState>& pipeline) = 0;
		
		virtual void Dispatch(std::uint32_t threadGroupCountX  = 1, std::uint32_t threadGroupCountY = 1, std::uint32_t threadGroupCountZ = 1) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :インデックスバッファを持つモデルに対して, 引数バッファをGPUで設定, 描画を実行出来る関数です
		/*----------------------------------------------------------------------*/
		virtual void DrawIndexedIndirect(const std::shared_ptr<core::GPUBuffer>& argumentBuffer, const std::uint32_t drawCallCount) = 0;

		/*----------------------------------------------------------------------
		*  @brief :Mesh shaderで使用する描画関数です.
		/*----------------------------------------------------------------------*/
		virtual void DispatchMesh(const std::uint32_t threadGroupCountX = 1, const std::uint32_t threadGroupCountY = 1, const std::uint32_t threadGroupCountZ = 1) = 0;
		
#pragma region Graphics Command Function
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
		
		/* @brief : このコマンドリストを特定するための固有IDです. 毎フレームコマンドリストがResetされたとしても残ります.*/
		std::uint64_t GetID() const { return _commandListID; }
		/*-------------------------------------------------------------------
		-                Command list open or close
		---------------------------------------------------------------------*/
		/* @brief :  コマンドリストを詰め込める状態にあるかを確認します. */
		bool IsOpen  () const { return _isOpen; }
		
		/* @brief : コマンドリストが閉じた状態(詰め込めない状態)にあるかを確認します. */
		bool IsClosed() const { return !_isOpen; }

		/* @brief : デバイスをセットします. */
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
		
		/* @brief : コマンドリストが詰め込める状態にあるかを確認します*/
		bool _isOpen = false;

		bool _beginRenderPass = false;

		std::uint64_t _commandListID = 0;
	};
}
#endif