//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12CommandList.cpp
///             @brief  CommandList 
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12FrameBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12RenderPass.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12DescriptorHeap.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12ResourceLayout.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Query.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUPipelineState.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUResourceView.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBarrierBatcher.hpp"
#include <stdexcept>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Memory/Include/GUAlignment.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHICommandList::~RHICommandList()
{
	if (_commandList) { _commandList.Reset(); _commandList = nullptr; }
}

RHICommandList::RHICommandList(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator, const gu::tstring& name) : 
	rhi::core::RHICommandList(device, commandAllocator)
{
	/*-------------------------------------------------------------------
	-               Get device and command allocator
	---------------------------------------------------------------------*/
	const auto dxDevice     = static_cast<RHIDevice*>(_device.Get())->GetDevice();
	const auto rhiAllocator = gu::StaticPointerCast<directX12::RHICommandAllocator>(_commandAllocator);
	const auto dxAllocator  = rhiAllocator->GetAllocator();

	/*-------------------------------------------------------------------
	-               Get command list type
	---------------------------------------------------------------------*/
	_commandListType = rhiAllocator->GetCommandListType();

	/*-------------------------------------------------------------------
	-               Prepare Closed Command List
	---------------------------------------------------------------------*/
	ThrowIfFailed(dxDevice->CreateCommandList(
		_device->GetGPUMask().Value(),            // GPU index
		EnumConverter::Convert(_commandListType),
		dxAllocator.Get(),                        // Associated command allocator
		nullptr,                                  // Initial PipeLine State Object
		IID_PPV_ARGS(_commandList.GetAddressOf())));

	ThrowIfFailed(_commandList->SetName(name.CString()));
	ThrowIfFailed(_commandList->Close());
	_isOpen = false;

	_barrierBatcher = gu::MakeShared<GPUBarrierBatcher>();
}

#pragma endregion Constructor and Destructor

#pragma region Call Draw Frame
/****************************************************************************
*                     BeginRecording
*************************************************************************//**
/* @brief     コマンドリストを記録状態に変更します. これはDraw関数の最初に使用します @n
*  @param[in] 描画フレーム中に呼ばれる場合にコマンドアロケータの中身をResetするかを決定するbool値.@n
*             描画フレーム中に呼ぶのは, コマンドリストを切り替える際に使用される可能性があるためです.
*
*  @return    void
*****************************************************************************/
void RHICommandList::BeginRecording(const bool stillMidFrame)
{
	/*-------------------------------------------------------------------
	-        コマンドリストが記録可能状態だった場合, Resetすることが出来ない.
	---------------------------------------------------------------------*/
	if (IsOpen()) { return; }

	/*-------------------------------------------------------------------
	-        描画フレームの途中でなければCommandAllocatorのバッファを先頭に戻す.
	---------------------------------------------------------------------*/
	if (_commandAllocator)
	{
		if (!stillMidFrame)
		{
			_commandAllocator->CleanUp();
		} // command buffer clear
		ThrowIfFailed(_commandList->Reset(static_cast<RHICommandAllocator*>(_commandAllocator.Get())->GetAllocator().Get(), nullptr));
	}
	
	/*-------------------------------------------------------------------
	-        コマンドリストを記録可能状態に変更します
	---------------------------------------------------------------------*/
	_isOpen          = true;
	_beginRenderPass = false;
}

/****************************************************************************
*                     EndRecording
*************************************************************************//**
/*  @brief     コマンドリストを記録状態から実行可能状態に変更します. これはDraw関数の最後に使用します
* 
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandList::EndRecording()
{
	/*-------------------------------------------------------------------
	-      　既に閉じられているならこの後の処理は行わない
	---------------------------------------------------------------------*/
	if (IsClosed()) { _RPT0(_CRT_WARN, "Already closed. \n");  return; }
	
	/*-------------------------------------------------------------------
	-      　記録状態からクローズ状態に以降する
	---------------------------------------------------------------------*/
	ThrowIfFailed(_commandList->Close());

	/*-------------------------------------------------------------------
	-      　終了処理
	---------------------------------------------------------------------*/
	_isOpen = false;
	_beginRenderPass = false;
}

/****************************************************************************
*                     Reset
*************************************************************************//**
*  @fn        void RHICommandList::Reset(const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator)
*
*  @brief     Proceed to the record state. コマンドリストを記録状態に変更します.
		      基本的には, ResetではなくBeginRecordingを使用してください.
*
*  @param[in] const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator
*             コマンドをためておくバッファです
*
*  @return    void
*****************************************************************************/
void RHICommandList::Reset(const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator)
{
	/*-------------------------------------------------------------------
	-        コマンドリストが記録可能状態だった場合, Resetすることが出来ない.
	---------------------------------------------------------------------*/
	if (IsOpen()) { return; }

	/*-------------------------------------------------------------------
	-         新しいコマンドバッファが同じキューの種類であるかを確認する
	---------------------------------------------------------------------*/
	if (commandAllocator) 
	{
		// コマンドリストの種類チェック
		if (_commandAllocator->GetCommandListType() != GetType()) 
		{
			_RPTW0(_CRT_WARN, L"Different commandList type");
			return;
		}

		// コマンドリストを登録
		_commandAllocator = commandAllocator; 

		// Reset
		ThrowIfFailed(_commandList->Reset(static_cast<RHICommandAllocator*>(_commandAllocator.Get())->GetAllocator().Get(), nullptr));
	}
	else
	{
		return;
	}

	/*-------------------------------------------------------------------
	-        コマンドリストを開いている状態にする
	---------------------------------------------------------------------*/
	_isOpen = true;
	_beginRenderPass = false;
}

/****************************************************************************
*                     BeginRenderPass
*************************************************************************//**
*  @fn        void RHICommandList::BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer)
*
*  @brief     Begin render pass and frame buffer.
*
*  @param[in] const gu::SharedPointer<core::RHIRenderPass>& renderPass
*  @param[in] const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer
*
*  @return    void
*****************************************************************************/
void RHICommandList::BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Layout Transition (Present -> RenderTarget)
	---------------------------------------------------------------------*/
	gu::DynamicArray<core::ResourceState> states(frameBuffer->GetRenderTargetSize(), core::ResourceState::RenderTarget);
	for (uint64 i = 0; i < frameBuffer->GetRenderTargetSize(); ++i)
	{
		PushTransitionBarrier(frameBuffer->GetRenderTargets()[i], states[i]);
	}
	FlushResourceBarriers();

	/*-------------------------------------------------------------------
	-          Select renderpass and frame buffer action
	---------------------------------------------------------------------*/
	if (_device->IsSupportedRenderPass()) { BeginRenderPassImpl(gu::StaticPointerCast<directX12::RHIRenderPass>(renderPass), gu::StaticPointerCast<directX12::RHIFrameBuffer>(frameBuffer)); }
	else                                  { OMSetFrameBuffer   (gu::StaticPointerCast<directX12::RHIRenderPass>(renderPass), gu::StaticPointerCast<directX12::RHIFrameBuffer>(frameBuffer)); }

	// 参照カウントが無限に増えつつけるバグ対応に使用しました.
	if (_renderPass ) { _renderPass.Reset(); }
	if (_frameBuffer) { _frameBuffer.Reset(); }
	_renderPass  = renderPass;
	_frameBuffer = frameBuffer;
	_beginRenderPass = true;
}

/****************************************************************************
*                     EndRenderPass
*************************************************************************//**
*  @fn        void RHICommandList::EndRenderPass()
*
*  @brief     End render pass
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandList::EndRenderPass()
{
	if (!_beginRenderPass) { return; }
	if (_device->IsSupportedRenderPass()) { _commandList->EndRenderPass(); }

	/*-------------------------------------------------------------------
	-          Layout Transition (RenderTarget -> Present)
	---------------------------------------------------------------------*/
	gu::DynamicArray<core::ResourceState> states(_frameBuffer->GetRenderTargetSize(), core::ResourceState::Present);
	for (uint64 i = 0; i < _frameBuffer->GetRenderTargetSize(); ++i)
	{
		PushTransitionBarrier(_frameBuffer->GetRenderTargets()[i], states[i]);
	}
	FlushResourceBarriers();

	_beginRenderPass = false;
}

#pragma endregion Call Draw Frame

#pragma region GPU Command
/****************************************************************************
*                       SetDepthBounds
*************************************************************************//**
/* @brief     深度が指定の範囲に入っているかをテストし, 範囲内ならばピクセルシェーダーを動作させます.
*  
*  @param[in] const float 最小の深度情報
*  @param[in] const float 最大の深度情報
* 
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetDepthBounds(const float minDepth, const float maxDepth)
{
#if PLATFORM_OS_WINDOWS
	if (_device->IsSupportedDepthBoundsTest())
	{
		_commandList->OMSetDepthBounds(minDepth, maxDepth);
	}
#endif
}

void RHICommandList::SetDescriptorHeap(const gu::SharedPointer<core::RHIDescriptorHeap>& heap)
{
	const auto dxHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(heap);
	_commandList->SetDescriptorHeaps(1, dxHeap->GetHeap().GetAddressOf());
}

#pragma region Query
/****************************************************************************
*                       BeginQuery
*************************************************************************//**
*  @fn        void RHICommandList::BeginQuery(const core::QueryResultLocation& location)
*
*  @brief     GPU情報を取得するためのクエリを開始します
*
*  @param[in] const core::QueryResultLocation& location
*
*  @return 　　void
*****************************************************************************/
void RHICommandList::BeginQuery(const core::QueryResultLocation& location)
{
	/*-------------------------------------------------------------------
	-              Queryの取得
	---------------------------------------------------------------------*/
	const auto query = static_cast<directX12::RHIQuery*>(location.Heap.Get());
	
	Checkf(query, "query is nullptr");
	Check(query->GetDxQueryType() == D3D12_QUERY_TYPE_OCCLUSION ||
		  query->GetDxQueryType() == D3D12_QUERY_TYPE_PIPELINE_STATISTICS);
	
	/*-------------------------------------------------------------------
	-              QueryHeapの取得
	---------------------------------------------------------------------*/
	const auto heap = query->GetHeap();

	Checkf(heap, "heap is nullptr");

	/*-------------------------------------------------------------------
	-               Common command
	---------------------------------------------------------------------*/
	_commandList->BeginQuery(heap.Get(), query->GetDxQueryType(), location.QueryID);
}

/****************************************************************************
*                       EndQuery
*************************************************************************//**
*  @fn        void RHICommandList::EndQuery(const core::QueryResultLocation& location)
*
*  @brief     GPU情報を取得するためのクエリを終了します
*
*  @param[in] const core::QueryResultLocation& location
*
*  @return 　　void
*****************************************************************************/
void RHICommandList::EndQuery(const core::QueryResultLocation& location)
{
	const auto query = static_cast<directX12::RHIQuery*>(location.Heap.Get());
	Checkf(query, "query is nullptr");
	
	// クエリの終了
	_commandList->EndQuery(query->GetHeap().Get(), query->GetDxQueryType(), location.QueryID);
}
#pragma endregion Query
/****************************************************************************
*                       SetPrimitiveTopology
*************************************************************************//**
/* @brief    頂点情報のつなぎ方を設定します. 
* 
*  @param[in] プリミティブのトポロジー種類
* 
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetPrimitiveTopology(const core::PrimitiveTopology topology)
{
	_commandList->IASetPrimitiveTopology(EnumConverter::Convert(topology));
}

/****************************************************************************
*                       SetViewport
*************************************************************************//**
/*  @brief     ビューポートによって描画領域を設定します. シザー矩形もViewportに合わせて自動で設定します
*  @param[in] const core::Viewport* : 描画領域を示す単一のビューポート
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetViewport(const core::Viewport& viewport)
{
	const D3D12_VIEWPORT v = 
	{
		.TopLeftX = viewport.TopLeftX,
		.TopLeftY = viewport.TopLeftY,
		.Width    = viewport.Width,
		.Height   = viewport.Height,
		.MinDepth = viewport.MinDepth,
		.MaxDepth = viewport.MaxDepth
	};


	const D3D12_RECT r =
	{
		.left   = static_cast<LONG>(viewport.TopLeftX),
		.top    = static_cast<LONG>(viewport.TopLeftY),
		.right  = static_cast<LONG>(viewport.TopLeftX + viewport.Width),
		.bottom = static_cast<LONG>(viewport.TopLeftY + viewport.Height)
	};

	_commandList->RSSetViewports(1, &v);
	_commandList->RSSetScissorRects(1, &r);
}


/****************************************************************************
*                       SetViewport
*************************************************************************//**
/*  @brief     ビューポートの配列(アドレス)を入れて描画領域を設定します
*  @param[in] const core::Viewport* : 描画領域を記述した配列, もしくは単一のViewportのアドレス
*  @param[in] const gu::uint32 : ビューポートの配列数
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetViewport(const core::Viewport* viewport, const gu::uint32 numViewport)
{
	gu::DynamicArray<D3D12_VIEWPORT> v(numViewport);
	for (uint32 i = 0; i < numViewport; ++i)
	{
		v[i].TopLeftX = viewport[i].TopLeftX;
		v[i].TopLeftY = viewport[i].TopLeftY;
		v[i].Width    = viewport[i].Width;
		v[i].Height   = viewport[i].Height;
		v[i].MaxDepth = viewport[i].MaxDepth;
		v[i].MinDepth = viewport[i].MinDepth;
	}

	gu::DynamicArray<D3D12_RECT> r(numViewport);
	for (uint32 i = 0; i < numViewport; ++i)
	{
		r[i].left   = static_cast<LONG>(viewport[i].TopLeftX);
		r[i].top    = static_cast<LONG>(viewport[i].TopLeftY);
		r[i].right  = static_cast<LONG>(viewport[i].TopLeftX + viewport[i].Width);
		r[i].bottom = static_cast<LONG>(viewport[i].TopLeftY + viewport[i].Height);
	}
	_commandList->RSSetViewports   (numViewport, v.Data());
	_commandList->RSSetScissorRects(numViewport, r.Data());
}

/****************************************************************************
*                       SetViewport
*************************************************************************//**
/* @brief     VRのような立体視を行う時に設定する描画領域です
* 
*  @param[in] const core::Viewport& 左側の視野を示す描画領域
*  @param[in] const core::Viewport& 右側の視野を示す描画領域
* 
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetStereoViewport(const core::Viewport& leftView, const core::Viewport& rightView)
{
	D3D12_VIEWPORT viewports[2] = {};

	viewports[0].TopLeftX = leftView.TopLeftX;
	viewports[0].TopLeftY = leftView.TopLeftY;
	viewports[0].Width    = leftView.Width;
	viewports[0].Height   = leftView.Height;
	viewports[0].MinDepth = leftView.MinDepth;
	viewports[0].MaxDepth = leftView.MaxDepth;

	viewports[1].TopLeftX = leftView.TopLeftX;
	viewports[1].TopLeftY = leftView.TopLeftY;
	viewports[1].Width    = leftView.Width;
	viewports[1].Height   = leftView.Height;
	viewports[1].MinDepth = leftView.MinDepth;
	viewports[1].MaxDepth = leftView.MaxDepth;

	D3D12_RECT scissorRects[2] =
	{
		{(LONG)viewports[0].TopLeftX, (LONG)viewports[0].TopLeftY, (LONG)(viewports[0].TopLeftX + viewports[0].Width), (LONG)(viewports[0].TopLeftY + viewports[0].Height)},
		{(LONG)viewports[1].TopLeftX, (LONG)viewports[1].TopLeftY, (LONG)(viewports[1].TopLeftX + viewports[1].Width), (LONG)(viewports[1].TopLeftY + viewports[1].Height)}
	};

	_commandList->RSSetViewports   (2, viewports);
	_commandList->RSSetScissorRects(2, scissorRects);
}

/****************************************************************************
*                       SetScissorRect
*************************************************************************//**
/*  @brief     ビューポート内で実際に描画される領域を制限するためのシザー矩形を手動で設定します.
* 
*  @param[in] const core::ScissorRect* : 描画領域を制限するためのシザー矩形の配列
*  @param[in] const gu::uint32 : シザー矩形の配列数
* 
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetScissor(const core::ScissorRect* rect, const gu::uint32 numRect)
{
	gu::DynamicArray<D3D12_RECT> r(numRect);
	for (UINT i = 0; i < numRect; ++i)
	{
		r[i].left   = rect->Left;
		r[i].right  = rect->Right;
		r[i].bottom = rect->Bottom;
		r[i].top    = rect->Top;
	}
	_commandList->RSSetScissorRects(numRect, r.Data());
}

/****************************************************************************
*                       SetViewportAndScissor
*************************************************************************//**
/* @brief     描画領域を示すビューポートと, その中で実際に描画される範囲を指定するシザー矩形をそれぞれ手動で設定します.
* 
*  @param[in] const core::Viewport& 描画領域を示すビューポート
*  @param[in] const core::ScissorRect& 実際に描画される範囲を示すシザー矩形
* 
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect)
{
	D3D12_VIEWPORT v = {};
	v.TopLeftX = viewport.TopLeftX;
	v.TopLeftY = viewport.TopLeftY;
	v.Width    = viewport.Width;
	v.Height   = viewport.Height;
	v.MaxDepth = viewport.MaxDepth;
	v.MinDepth = viewport.MinDepth;

	D3D12_RECT r = {};
	r.left   = rect.Left;
	r.right  = rect.Right;
	r.bottom = rect.Bottom;
	r.top    = rect.Top;

	_commandList->RSSetViewports(1, &v);
	_commandList->RSSetScissorRects(1, &r);
}

void RHICommandList::SetVertexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer)
{
#if __DEBUG
	// Is vertex buffer
	assert(buffer->GetUsage() == core::BufferCreateFlags::VertexBuffer);
#endif

	// Set up vertex buffer view
	D3D12_VERTEX_BUFFER_VIEW view = {
		gu::StaticPointerCast<directX12::GPUBuffer>(buffer)->GetResourcePtr()->GetGPUVirtualAddress(),
		static_cast<UINT>(buffer->GetTotalByteSize()),
		static_cast<UINT>(buffer->GetElementByteSize())
	};
	// Regist command list
	_commandList->IASetVertexBuffers(0, 1, &view);
}

void RHICommandList::SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	_commandList->SetGraphicsRootSignature(gu::StaticPointerCast<directX12::RHIResourceLayout>(resourceLayout)->GetRootSignature().Get());
}

void RHICommandList::SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	_commandList->SetComputeRootSignature(gu::StaticPointerCast<directX12::RHIResourceLayout>(resourceLayout)->GetRootSignature().Get());
}

void RHICommandList::SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipelineState)
{
	SetPrimitiveTopology(pipelineState->GetInputAssemblyState()->GetPrimitiveTopology());
	_commandList->SetPipelineState(gu::StaticPointerCast<directX12::GPUGraphicsPipelineState>(pipelineState)->GetPipeline().Get());
}

void RHICommandList::SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipelineState)
{
	_commandList->SetPipelineState(gu::StaticPointerCast<directX12::GPUComputePipelineState>(pipelineState)->GetPipeline().Get());
}
void RHICommandList::SetVertexBuffers(const gu::DynamicArray<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot)
{
	auto views = gu::DynamicArray<D3D12_VERTEX_BUFFER_VIEW>(buffers.Size());
	for (size_t i = 0; i < views.Size(); ++i)
	{
#if __DEBUG
		assert(buffers[i]->GetUsage() == core::BufferCreateFlags::VertexBuffer);
#endif
		views[i].BufferLocation = gu::StaticPointerCast<directX12::GPUBuffer>(buffers[i])->GetResourcePtr()->GetGPUVirtualAddress();
		views[i].SizeInBytes    = static_cast<UINT>(buffers[i]->GetTotalByteSize());
		views[i].StrideInBytes  = static_cast<UINT>(buffers[i]->GetElementByteSize());
	}

	_commandList->IASetVertexBuffers(static_cast<UINT>(startSlot), static_cast<UINT>(views.Size()), views.Data());
}

void RHICommandList::SetIndexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType)
{
#if __DEBUG
	assert(buffer->GetUsage() == core::BufferCreateFlags::IndexBuffer);
#endif

	D3D12_INDEX_BUFFER_VIEW view = {};
	view.BufferLocation = gu::StaticPointerCast<directX12::GPUBuffer>(buffer)->GetResourcePtr()->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<UINT>(buffer->GetTotalByteSize());
	view.Format         = EnumConverter::Convert(indexType);

	_commandList->IASetIndexBuffer(&view);
}

void RHICommandList::DrawIndexed(gu::uint32 indexCount, gu::uint32 startIndexLocation, gu::uint32 baseVertexLocation)
{
	_commandList->DrawIndexedInstanced(indexCount, 1, startIndexLocation, baseVertexLocation, 0);
}


void RHICommandList::DrawIndexedInstanced(gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation, gu::uint32 baseVertexLocation, gu::uint32 startInstanceLocation)
{
	_commandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void RHICommandList::DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount)
{
	const auto dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device);
	const auto dxBuffer = gu::StaticPointerCast<directX12::GPUBuffer>(argumentBuffer);
	_commandList->ExecuteIndirect(dxDevice->GetDefaultDrawIndexedIndirectCommandSignature().Get(), drawCallCount, dxBuffer->GetResourcePtr(), 0, nullptr, 0);
}

void RHICommandList::DispatchMesh(const gu::uint32 threadGroupCountX, const gu::uint32 threadGroupCountY, const gu::uint32 threadGroupCountZ)
{
	_commandList->DispatchMesh(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

void RHICommandList::Dispatch(gu::uint32 threadGroupCountX, gu::uint32 threadGroupCountY, gu::uint32 threadGroupCountZ)
{
	_commandList->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

#pragma region Transition Resource State
/*!**********************************************************************
*  @brief  単独のGPUリソースの状態遷移を示します. @n
*          リソースの使い方が変わるタイミングで呼び出します.
*  @param[in] const gu::SharedPointer<core::GPUResource> GPUリソース
*  @param[in] const core::ResourceState 遷移前のリソース状態
*  @param[in] const core::ResourceState 遷移後のリソース状態
*  @param[in] const gu::uint32 サブリソースを示すインデックス (デフォルトは全てのサブリソースを示します) 0xfffffffの場合は全てのインデックスで有効化されます
*  @return    void
*************************************************************************/
void RHICommandList::PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& resource, const core::ResourceState after, const gu::uint32 subresource)
{
	if (resource->GetResourceState() == after) { return; }

	_barrierBatcher->PushTransitionBarrier(resource, after, subresource );
}

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
void RHICommandList::PushAliasingBarrier(const gu::SharedPointer<core::GPUResource>& before, const gu::SharedPointer<core::GPUResource>& after)
{
	_barrierBatcher->PushAliasingBarrier
	(
		before->IsBuffer() ? gu::StaticPointerCast<directX12::GPUBuffer>(before)->GetResourcePtr() : gu::StaticPointerCast<directX12::GPUTexture>(before)->GetResourcePtr(),
		after ->IsBuffer() ? gu::StaticPointerCast<directX12::GPUBuffer>(after) ->GetResourcePtr() : gu::StaticPointerCast<directX12::GPUTexture>(after) ->GetResourcePtr()
	);
}
		
/*!**********************************************************************
*  @brief     Unordered access view専用の状態バリア @n
*             UAVの読み書き中にほかのUAVが対象リソースを読み書きする描画コマンドの実行を防ぐことを目的とします @n
*  @param[in] const gu::SharedPointer<core::GPUResource> Unordered access viewを持つGPUバッファ
*  @return    void
*************************************************************************/
void RHICommandList::PushUAVBarrier(const gu::SharedPointer<core::GPUResource>& resource)
{
	_barrierBatcher->PushUAVBarrier
	(
		resource->IsBuffer() ? gu::StaticPointerCast<directX12::GPUBuffer>(resource)->GetResourcePtr() : gu::StaticPointerCast<directX12::GPUTexture>(resource)->GetResourcePtr()
	);
}

/*!**********************************************************************
*  @brief  コマンドリストを使ってResourceBarrierをまとめて呼び出します.
*  @param[in] void
*  @return    void
*************************************************************************/
void RHICommandList::FlushResourceBarriers()
{
	_barrierBatcher->Flush(_commandList.Get());
}

#pragma endregion Transition Resource State
#pragma region Copy 
/****************************************************************************
*                     CopyResource
*************************************************************************//**
/* @brief   あるリソースの領域をまとめて別のリソースにコピーする.
*           組み合わせに応じて自動でバッファかテクスチャかを判定します
*
*  @param[in] const gu::SharedPointer<core::GPUTexture>& コピー先のバッファ
*  @param[in] const gu::SharedPointer<core::GPUTexture>& コピー元のバッファ

*  @return 　　void
*****************************************************************************/
void RHICommandList::CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source)
{
	CopyResource(gu::StaticPointerCast<core::GPUResource>(dest), gu::StaticPointerCast<core::GPUResource>(source));
}

/****************************************************************************
*                     CopyResource
*************************************************************************//**
/* @brief     あるリソースの領域をまとめて別のリソースにコピーする. 
*           組み合わせに応じて自動でバッファかテクスチャかを判定します
*
*  @param[in] gu::SharedPointer<core::GPUBuffer> コピー先のバッファ
*  @param[in] gu::SharedPointer<core::GPUBuffer>& コピー元のバッファ

*  @return 　　void
*****************************************************************************/
void RHICommandList::CopyResource(const gu::SharedPointer<core::GPUResource>& dest, const gu::SharedPointer<core::GPUResource>& source)
{
	gu::SharedPointer<core::GPUResource> resources[] = { dest, source };
	rhi::core::ResourceState befores[] = { dest->GetResourceState()            , source->GetResourceState() };
	rhi::core::ResourceState afters[] = { core::ResourceState::CopyDestination, core::ResourceState::CopySource };

	_barrierBatcher->PushTransitionBarrier(dest, afters[0]);
	_barrierBatcher->PushTransitionBarrier(source, afters[1]);
	_barrierBatcher->Flush(_commandList.Get());

	if (dest->IsTexture() && source->IsTexture())
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUTexture>(dest)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUTexture>(source)->GetResource().Get());
	}
	else if (dest->IsTexture() && !source->IsTexture())
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUTexture>(dest)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUBuffer>(source)->GetResource().Get());
	}
	else
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUBuffer>(dest)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUBuffer>(source)->GetResource().Get());
	}

	_barrierBatcher->PushTransitionBarrier(dest  , befores[0]);
	_barrierBatcher->PushTransitionBarrier(source, befores[1]);
	_barrierBatcher->Flush(_commandList.Get());
}

/****************************************************************************
*                     CopyBufferRegion
*************************************************************************//**
/*  @brief     GPUバッファの領域をあるGPUポインタから別のGPUポインタにコピーを行う. GPU版のmemcpy
*
*  @param[in] const gu::SharedPointer<core::GPUBuffer> : コピー先のバッファ
*  @param[in] const gu::uint64 : コピー先の初期書き取りポインタをずらすoffset byte
*  @param[in] const gu::SharedPointer<core::GPUBuffer> : コピー元のバッファ
*  @param[in] const gu::uint64 : コピー元の初期読み取りポインタをずらすoffset byte
*  @param[in] const gu::uint64 copyByteSize コピーしたいバイト数

*  @return 　　void
*****************************************************************************/
void RHICommandList::CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& destination, const gu::uint64 destinationOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize)
{
	using enum core::ResourceState;

	const auto rhiDestBuffer       = static_cast<directX12::GPUBuffer*>(destination.Get());
	const auto rhiSourceBuffer     = static_cast<directX12::GPUBuffer*>(source.Get());
	const auto destinationResource = rhiDestBuffer  ->GetResourcePtr();
	const auto sourceResource      = rhiSourceBuffer->GetResourcePtr();

	/*-------------------------------------------------------------------
	-       コピー可能かのチェック
	---------------------------------------------------------------------*/
	//"CopyBufferRegionを同じリソースで使用することはできません. 
	// これは、コピー元とコピー先の両方が同じリソースからサブアロケートされている場合に発生する可能性があります")；
	Checkf(sourceResource != destinationResource, "CopyBufferRegion cannot be used on the same resource. This can happen when both the source and the dest are suballocated from the same resource.");
	
	// バッファの範囲内に収まっているか
	Check(destinationOffset + copyByteSize <= rhiDestBuffer  ->GetTotalByteSize());
	Check(sourceOffset      + copyByteSize <= rhiSourceBuffer->GetTotalByteSize());

	/*-------------------------------------------------------------------
	-         コピーの初期状態と終了状態の定義
	---------------------------------------------------------------------*/
	core::ResourceState barrierAfterStates[]   = { destination->GetResourceState(), source->GetResourceState() };
	core::ResourceState barrierBeforeStates[]  = { CopyDestination, CopySource };

	// バリアにより, リソースの読み方を伝える
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(destination), barrierAfterStates[0]);
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(source     ), barrierAfterStates[1]);
	_barrierBatcher->Flush(_commandList.Get());

	/*-------------------------------------------------------------------
	-           GPU領域のコピー
	---------------------------------------------------------------------*/
	_commandList->CopyBufferRegion(destinationResource, destinationOffset, sourceResource, sourceOffset, copyByteSize);

	/*-------------------------------------------------------------------
	-          元の状態に戻す
	---------------------------------------------------------------------*/
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(destination), barrierBeforeStates[0]);
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(source)     , barrierBeforeStates[1]);
	_barrierBatcher->Flush(_commandList.Get());
}

/****************************************************************************
*                     CopyTextureRegion
*************************************************************************//**
/*  @brief     GPUテクスチャの領域をあるGPUポインタから別のGPUポインタにコピーを行う. GPU版のmemcpy
*
*  @param[in] const gu::SharedPointer<core::GPUTexture> : コピー先のテクスチャ
*  @param[in] const gu::SharedPointer<core::GPUTexture> : コピー元のテクスチャ
*  @param[in] const core::GPUTextureCopyInfo : コピーする際の情報

*  @return 　　void
*****************************************************************************/
void RHICommandList::CopyTextureRegion(const gu::SharedPointer<core::GPUTexture>& destination, const gu::SharedPointer<core::GPUTexture>& source, const core::GPUTextureCopyInfo& copyInfo)
{
	const auto  dxDestinationTexture  = static_cast<directX12::GPUTexture*>(destination.Get());
	const auto  dxSourceTexture       = static_cast<directX12::GPUTexture*>(source.Get());
	const auto& dxDestinationDesc     = dxDestinationTexture->GetMetaData();
	const auto& dxSourceDesc          = dxSourceTexture->GetMetaData();

	/*-------------------------------------------------------------------
	-         コピーの仕方を確認する
	---------------------------------------------------------------------*/
	// 全てのピクセルをコピーするかどうか
	const bool isAllPixel       = destination->GetByteSize() == source->GetByteSize() &&
		                          (copyInfo.TexelSize.IsZero() || copyInfo.TexelSize == source->GetTexelSize());

	// 全てのサブリソースIDをコピーするかどうか
	const bool isAllSubresource = (dxDestinationDesc.MipMapLevels == dxSourceDesc.MipMapLevels) && 
		                          (dxSourceDesc.MipMapLevels      == copyInfo.MipMapCount) && 
		                          (destination->GetArrayLength()  == source->GetArrayLength()) && 
		                          (source->GetArrayLength()       == copyInfo.ArraySliceCount);

	// GPUからCPUにReadbackが発生するかどうか
	const bool useReadback = gu::HasAnyFlags(destination->GetUsage(), core::TextureCreateFlags::CPUReadback);

	// 範囲チェック
	Checkf(copyInfo.ArraySliceCount + copyInfo.DestinationInitArraySlice <= destination->GetArrayLength()   , "This function exceeds the array slice count in the destination texture\n");
	Checkf(copyInfo.ArraySliceCount + copyInfo.SourceInitArraySlice      <= source     ->GetArrayLength()   , "This function exceeds the array slice count in the source texture\n");
	Checkf(copyInfo.MipMapCount     + copyInfo.DestinationInitMipMap     <= destination->GetMaxMipMapLevel(), "This function exceeds the mipmap count in the destination texture\n ");
	Checkf(copyInfo.MipMapCount     + copyInfo.SourceInitMipMap          <= source     ->GetMaxMipMapLevel(), "This function exceeds the mipmap count in the source texture \n");


	/*-------------------------------------------------------------------
	-         コピーの初期状態と終了状態の定義
	---------------------------------------------------------------------*/
	core::ResourceState barrierAfterStates[]  = { destination->GetResourceState(), source->GetResourceState() };
	core::ResourceState barrierBeforeStates[] = { core::ResourceState::CopyDestination, core::ResourceState::CopySource };

	// バリアにより, リソースの読み方を伝える
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(destination), barrierAfterStates[0]);
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(source)     , barrierAfterStates[1]);

	/*-------------------------------------------------------------------
	-       一部分のみコピーする場合
	---------------------------------------------------------------------*/
	if (!isAllPixel || !isAllSubresource || useReadback)
	{
		const auto copyTexelSize = copyInfo.TexelSize.IsZero() ? source->GetTexelSize() >> copyInfo.SourceInitMipMap : copyInfo.TexelSize;

		// テクスチャ配列の種類に応じてコピー
		for (gu::uint16 sliceIndex = 0; sliceIndex < copyInfo.ArraySliceCount; ++sliceIndex)
		{
			const auto sourceSliceIndex      = copyInfo.SourceInitArraySlice      + sliceIndex;
			const auto destinationSliceIndex = copyInfo.DestinationInitArraySlice + sliceIndex;

			// mipmapのレベルに応じてコピー
			for (gu::uint32 mipmapIndex = 0; mipmapIndex < copyInfo.MipMapCount; ++mipmapIndex)
			{
				const auto sourceMipmapIndex      = copyInfo.SourceInitMipMap      + mipmapIndex;
				const auto destinationMipmapIndex = copyInfo.DestinationInitMipMap + mipmapIndex;
				const auto endCopyPosition        = copyInfo.SourceInitTexelPosition + copyTexelSize;

				// Todo : Box (まだアラインメントの指定が入っていません)
				const D3D12_BOX sourceBox =
				{
					.left   = copyInfo.SourceInitTexelPosition.x >> mipmapIndex,
					.top    = copyInfo.SourceInitTexelPosition.y >> mipmapIndex,
					.front  = copyInfo.SourceInitTexelPosition.z >> mipmapIndex, 
					.right  = 0, //gu::Alignment::AlignUpArbitary(endCopyPosition.x > 1u << mipmapIndex ? 1u : endCopyPosition.x >> mipmapIndex),
					.bottom = 0,
					.back   = 0
				};

				// コピー元のCopyLocation
				D3D12_TEXTURE_COPY_LOCATION sourceCopyLocation{};
				sourceCopyLocation.pResource = dxSourceTexture->GetResourcePtr();
				sourceCopyLocation.Type      = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				sourceCopyLocation.SubresourceIndex = dxSourceTexture->GetSubresourceIndex(sourceMipmapIndex, sourceSliceIndex, 0);

				// コピー先のCopyLocation
				D3D12_TEXTURE_COPY_LOCATION destinationCopyLocation{};
				destinationCopyLocation.pResource = dxDestinationTexture->GetResourcePtr();
				destinationCopyLocation.Type      = useReadback ? D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT : D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				destinationCopyLocation.SubresourceIndex = dxDestinationTexture->GetSubresourceIndex(destinationMipmapIndex, destinationSliceIndex, 0);

				// readbackが指定される場合, footprintを行う
				if (useReadback)
				{
					// Todo
				}

				// コピーの実行
				_commandList->CopyTextureRegion(&destinationCopyLocation, 0, 0, 0, &sourceCopyLocation, &sourceBox);
			}
		}
	}
	/*-------------------------------------------------------------------
	-      　全ての領域をコピーする場合
	---------------------------------------------------------------------*/
	else
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUTexture>(destination)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUTexture>(source)     ->GetResource().Get());
	}

	/*-------------------------------------------------------------------
	-       元の状態に戻す
	---------------------------------------------------------------------*/
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(destination), barrierBeforeStates[0]);
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(source), barrierBeforeStates[1]);
	_barrierBatcher->Flush(_commandList.Get());
}

#pragma endregion Copy
#pragma endregion GPU Command
#pragma region Property
void RHICommandList::SetName(const gu::tstring& name)
{
	_commandList->SetName(name.CString());
}

#pragma endregion Property
#pragma region Private Function
/****************************************************************************
*                     BeginRenderPassImpl
*************************************************************************//**
*  @fn        void RHICommandList::BeginRenderPassImpl(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)<directX12::RHIFrameBuffer>& frameBuffer)
*  @brief     If you used render pass, this function is called. Set frame buffer (render target and depth stencil) to command buffer.
*             When you use core::AttachmentLoad::Clear, color or depthStencil frame buffer is cleared.
*  @param[in] const gu::SharedPointer<directX12::RHIRenderPass>& renderPass
*  @param[in] const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer
*  @return 　　void
*****************************************************************************/
void RHICommandList::BeginRenderPassImpl(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Get frame buffer resources
	---------------------------------------------------------------------*/
	const bool hasRTV  = frameBuffer->GetRenderTargetSize() != 0;
	const bool hasDSV  = frameBuffer->GetDepthStencil();
	/*-------------------------------------------------------------------
	-          Set CPU rtv and dsv descriptor handle
	---------------------------------------------------------------------*/
	// render target 
	gu::DynamicArray<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rtvDescs(frameBuffer->GetRenderTargetSize());
	for (size_t i = 0; i < rtvDescs.Size(); ++i)
	{
		// get rtv handle
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetRenderTargetView(i));
#ifdef _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::RenderTarget);
#endif

		const auto rtvHandle = view->GetCPUHandler();
		if (!rtvHandle.ptr) { continue; }

		// set render pass load op
		D3D12_RENDER_PASS_BEGINNING_ACCESS begin = {};
		begin.Type = EnumConverter::Convert(renderPass->GetColorAttachment(i)->LoadOp);
		begin.Clear.ClearValue.Color[0] = renderPass->GetClearColor()[i].Type.Color[core::ClearValue::Red];
		begin.Clear.ClearValue.Color[1] = renderPass->GetClearColor()[i].Type.Color[core::ClearValue::Green];
		begin.Clear.ClearValue.Color[2] = renderPass->GetClearColor()[i].Type.Color[core::ClearValue::Blue];
		begin.Clear.ClearValue.Color[3] = renderPass->GetClearColor()[i].Type.Color[core::ClearValue::Alpha];
		// set render pass store op
		D3D12_RENDER_PASS_ENDING_ACCESS end = {};
		end.Type = EnumConverter::Convert(renderPass->GetColorAttachment(i)->StoreOp);
		
		// set render pass descriptor
		rtvDescs[i].BeginningAccess = begin;
		rtvDescs[i].cpuDescriptor   = rtvHandle;
		rtvDescs[i].EndingAccess    = end;
	}
	// depth stencil
	D3D12_RENDER_PASS_DEPTH_STENCIL_DESC dsvDesc = {};
	if (hasDSV)
	{
		// get dsv handle
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetDepthStencilView());

#if __DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::DepthStencil);
#endif

		const auto dsvHandle = view->GetCPUHandler();

		// set depth render pass load op
		D3D12_RENDER_PASS_BEGINNING_ACCESS depthBegin   = { EnumConverter::Convert(renderPass->GetDepthAttachment()->LoadOp), {} };
		D3D12_RENDER_PASS_ENDING_ACCESS    depthEnd     = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StoreOp), {} };
		D3D12_RENDER_PASS_BEGINNING_ACCESS stencilBegin = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StencilLoad), {} };
		D3D12_RENDER_PASS_ENDING_ACCESS    stencilEnd   = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StencilStore), {} };
		depthBegin.Clear.ClearValue.DepthStencil.Depth     = renderPass->GetDepthClear()->Type.DSV.Depth;
		stencilBegin.Clear.ClearValue.DepthStencil.Stencil = renderPass->GetDepthClear()->Type.DSV.Stencil;
		
		// set depth stencil descriptor
		dsvDesc.DepthBeginningAccess   = depthBegin;
		dsvDesc.DepthEndingAccess      = depthEnd;
		dsvDesc.StencilBeginningAccess = stencilBegin;
		dsvDesc.StencilEndingAccess    = stencilEnd;
		dsvDesc.cpuDescriptor          = dsvHandle;
	}

	_commandList->BeginRenderPass(static_cast<gu::uint32>(rtvDescs.Size()), hasRTV ? rtvDescs.Data() : nullptr, hasDSV ? &dsvDesc : nullptr, D3D12_RENDER_PASS_FLAG_NONE);
}

/****************************************************************************
*                     OMSetFrameBuffer
*************************************************************************//**
*  @fn        void RHICommandList::OMSetFrameBuffer(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)
*  @brief     If not used render pass, this function is called. Set frame buffer (render target and depth stencil) to command buffer.
*             When you use core::AttachmentLoad::Clear, color or depthStencil frame buffer is cleared.
*  @param[in] const gu::SharedPointer<directX12::RHIRenderPass>& renderPass
*  @param[in] const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer
*  @return 　　void
*****************************************************************************/
void RHICommandList::OMSetFrameBuffer(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Get frame buffer resources
	---------------------------------------------------------------------*/
	auto renderTargets = frameBuffer->GetRenderTargets();
	auto depthStencil  = frameBuffer->GetDepthStencil();
	const bool hasRTV  = frameBuffer->GetRenderTargetSize() != 0;
	const bool hasDSV  = frameBuffer->GetDepthStencil();
	/*-------------------------------------------------------------------
	-          Set CPU rtv and dsv descriptor handle
	---------------------------------------------------------------------*/
	gu::DynamicArray<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandle(renderTargets.Size());
	for (size_t i = 0; i < rtvHandle.Size(); ++i)
	{
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetRenderTargetView(i));

#if _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::RenderTarget);
#endif 

		rtvHandle[i] = view->GetCPUHandler();
	}
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = {};
	if (hasDSV)
	{
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetDepthStencilView());
#if _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::DepthStencil);
#endif
		dsvHandle = view->GetCPUHandler();
	}
	/*-------------------------------------------------------------------
	-          Clear frame buffer resources
	---------------------------------------------------------------------*/
	// render target
	for (int i = 0; i < rtvHandle.Size(); ++i)
	{
		const auto colorAttachment = renderPass->GetColorAttachment(i);
		if (colorAttachment->LoadOp != core::AttachmentLoad::Clear) { continue; }
		_commandList->ClearRenderTargetView(rtvHandle[i], renderPass->GetClearColor()[i].Type.Color, 0, nullptr);
	}
	// depth stencil 
	if (hasDSV)
	{
		const auto depthAttachment = renderPass->GetDepthAttachment();

		// set clear flag 
		D3D12_CLEAR_FLAGS clearFlags = {};
		if (depthAttachment->LoadOp      == core::AttachmentLoad::Clear) { clearFlags |= D3D12_CLEAR_FLAG_DEPTH; }
		if (depthAttachment->StencilLoad == core::AttachmentLoad::Clear) { clearFlags |= D3D12_CLEAR_FLAG_STENCIL; }
		// clear depth stencil
		if (clearFlags)
		{
			_commandList->ClearDepthStencilView(dsvHandle, clearFlags, renderPass->GetDepthClear()->Type.DSV.Depth, renderPass->GetDepthClear()->Type.DSV.Stencil, 0, nullptr);
		}
	}
	/*-------------------------------------------------------------------
	-          Set render target and depth stencil
	---------------------------------------------------------------------*/
	_commandList->OMSetRenderTargets(static_cast<gu::uint32>(rtvHandle.Size()), hasRTV ? rtvHandle.Data() : nullptr, FALSE, hasDSV ? &dsvHandle : nullptr);
}

#pragma endregion Private Function