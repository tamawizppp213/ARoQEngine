//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12CommandContext.cpp
///             @brief  Command list context
///             @author Toide Yutaro
///             @date   2022_03_20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandContext.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12GraphicsDevice.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12GPUResource.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
void CommandContext::Initialize(GraphicsDeviceDirectX12* device)
{
	_device = device;
	_commandList = device->GetCommandList();
}
void CommandContext::WaitUntilFinishDrawingToRenderTarget(Resource* renderTarget)
{
	BARRIER barrier = BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	_commandList->ResourceBarrier(1, &barrier);
}
void CommandContext::WaitUntilToPossibleSetRenderTarget(Resource* renderTarget)
{
	BARRIER barrier = BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	_commandList->ResourceBarrier(1, &barrier);
}
#pragma region Command
void CommandContext::SetPrimitiveTopology(PrimitiveTopology topology)
{
	static D3D12_PRIMITIVE_TOPOLOGY topologies[] =
	{
		D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	};
	_commandList->IASetPrimitiveTopology(topologies[(int)topology]);
}
void CommandContext::SetViewport(const rhi::core::Viewport* viewport, UINT numViewport)
{
	std::vector<D3D12_VIEWPORT> v(numViewport);
	for (UINT i = 0; i < numViewport; ++i)
	{
		v[i].TopLeftX = viewport->TopLeftX;
		v[i].TopLeftY = viewport->TopLeftY;
		v[i].Width    = viewport->Width;
		v[i].Height   = viewport->Height;
		v[i].MaxDepth = viewport->MaxDepth;
		v[i].MinDepth = viewport->MinDepth;
	}
	_commandList->RSSetViewports(numViewport, v.data());
}
void CommandContext::SetScissor(const ScissorRect* rect, UINT numRect)
{
	std::vector<D3D12_RECT> r(numRect);
	for (UINT i = 0; i < numRect; ++i)
	{
		r[i].left   = rect->Left;
		r[i].right  = rect->Right;
		r[i].bottom = rect->Bottom;
		r[i].top    = rect->Top;
	}
	_commandList->RSSetScissorRects(numRect, r.data());
}
void CommandContext::SetViewportAndScissor(const Viewport& viewport, const ScissorRect& rect)
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
#pragma endregion Command
/****************************************************************************
*                     TransitionResourceState
*************************************************************************//**
*  @fn        void CommandContext::TransitionResourceState(Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
*  @brief     Transion resource state
*  @param[in,out] GPUResource* resource
*  @param[in    ] D3D12_RESOURCE_STATES before
*  @param[in    ] D3D12_RESOURCE_STATES after
*  @return 　　void
*****************************************************************************/
void CommandContext::TransitionResourceState(Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	BARRIER barrier = BARRIER::Transition(resource, before, after);
	_commandList->ResourceBarrier(1, &barrier);
}
/****************************************************************************
*                     CopyBuffer
*************************************************************************//**
*  @fn        void CommandContext::CopyBuffer(GPUResource* dest, GPUResource* source)
*  @brief     Prepare copy buffer
*  @param[in,out] GPUResource* dest
*  @param[in,out] GPUResource* source
*  @return 　　void
*****************************************************************************/
void CommandContext::CopyBuffer(rhi::directX12::GPUResource* dest, rhi::directX12::GPUResource* source)
{
	auto initSourceState = source->GetUsageState();
	auto initDestState   = dest->GetUsageState();
	PrepareCopyBuffer(dest, source);
	_commandList->CopyResource(dest->GetResource(), source->GetResource());
	CompleteCopyBuffer(dest, source, initDestState, initSourceState);
}
/****************************************************************************
*                   CopyBufferRegion
*************************************************************************//**
*  @fn        void CommandContext::CopyBufferRegion(GPUResource* dest, size_t destOffset, GPUResource* source, size_t sourceOffset, size_t numBytes)
*  @brief     Copy buffer region 
*  @param[in,out] GPUResource* dest
*  @param[in    ] size_t destOffset
*  @param[in,out] GPUResource* source
*  @param[in    ] size_t sourceOffset
*  @param[in    ] size_t numBytes
*  @return 　　void
*****************************************************************************/
void CommandContext::CopyBufferRegion(rhi::directX12::GPUResource* dest, size_t destOffset, rhi::directX12::GPUResource* source, size_t sourceOffset, size_t numBytes)
{
	auto initSourceState = source->GetUsageState();
	auto initDestState   = dest->GetUsageState();
	PrepareCopyBuffer(dest, source);
	_commandList->CopyBufferRegion(dest->GetResource(), destOffset, source->GetResource(), sourceOffset, numBytes);
	CompleteCopyBuffer(dest, source, initDestState, initSourceState);
}
/****************************************************************************
*                   CopyTextureRegion
*************************************************************************//**
*  @fn        void CommandContext::CopyTextureRegion(GPUResource* dest,GPUResource* source, D3D12_BOX& rect, UINT x, UINT y, UINT z)
*  @brief     Copy texture region to GPU Buffer
*  @param[in,out] GPUResource* dest
*  @param[in,out] GPUResource* source
*  @param[in]     D3D12_BOX& rect
*  @param[in]     UINT x = 1
*  @param[in]     UINT y = 1,
*  @param[in]     UINT z = 1,
*  @return 　　    void
*****************************************************************************/
void CommandContext::CopyTextureRegion(rhi::directX12::GPUResource* dest, rhi::directX12::GPUResource* source, D3D12_BOX& rect, UINT x, UINT y, UINT z)
{
	auto initSourceState = source->GetUsageState();
	auto initDestState   = dest->GetUsageState();
	PrepareCopyBuffer(dest, source);
	D3D12_TEXTURE_COPY_LOCATION destLocation   = TEXTURE_COPY_LOCATION(dest->GetResource(), 0);
	D3D12_TEXTURE_COPY_LOCATION sourceLocation = TEXTURE_COPY_LOCATION(source->GetResource(), 0);
	_commandList->CopyTextureRegion(&destLocation, x, y, z, &sourceLocation, &rect);
	CompleteCopyBuffer(dest, source, initDestState, initSourceState);
}
#pragma region Protected Function
void CommandContext::PrepareCopyBuffer(rhi::directX12::GPUResource* dest, rhi::directX12::GPUResource* source)
{
	BARRIER barrier[] =
	{
		BARRIER::Transition(source->GetResource(), source->GetUsageState(), D3D12_RESOURCE_STATE_COPY_SOURCE),
		BARRIER::Transition(dest->GetResource()  , dest->GetUsageState()  , D3D12_RESOURCE_STATE_COPY_DEST)
	};
	source->TransitionState(D3D12_RESOURCE_STATE_COPY_SOURCE);
	dest->TransitionState(D3D12_RESOURCE_STATE_COPY_DEST);
	ResourceBarriers(_countof(barrier), barrier);
}

void CommandContext::CompleteCopyBuffer(rhi::directX12::GPUResource* dest, rhi::directX12::GPUResource* source, const D3D12_RESOURCE_STATES& toDestState, const D3D12_RESOURCE_STATES& toSourceState)
{
	BARRIER barrier[] =
	{
		BARRIER::Transition(source->GetResource(), source->GetUsageState(), toSourceState),
		BARRIER::Transition(dest->GetResource()  , dest->GetUsageState()  , toDestState)
	};
	source->TransitionState(toSourceState);
	dest->TransitionState(toDestState);
	ResourceBarriers(_countof(barrier), barrier);
}
#pragma endregion Protected Function