//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12CommandContext.cpp
///             @brief  Command list context
///             @author Toide Yutaro
///             @date   2022_03_20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12CommandContext.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12GraphicsDevice.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12GPUResource.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


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
void CommandContext::CopyBuffer(GPUResource* dest, GPUResource* source)
{
	PrepareCopyBuffer(dest, source);
	_commandList->CopyResource(dest->GetResource(), source->GetResource());
	PrepareCopyBuffer(source, dest);
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
void CommandContext::CopyBufferRegion(GPUResource* dest, size_t destOffset, GPUResource* source, size_t sourceOffset, size_t numBytes)
{
	PrepareCopyBuffer(dest, source);
	_commandList->CopyBufferRegion(dest->GetResource(), destOffset, source->GetResource(), sourceOffset, numBytes);
	PrepareCopyBuffer(source, dest);
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
void CommandContext::CopyTextureRegion(GPUResource* dest, GPUResource* source, D3D12_BOX& rect, UINT x, UINT y, UINT z)
{
	PrepareCopyBuffer(dest, source);
	D3D12_TEXTURE_COPY_LOCATION destLocation   = TEXTURE_COPY_LOCATION(dest->GetResource(), 0);
	D3D12_TEXTURE_COPY_LOCATION sourceLocation = TEXTURE_COPY_LOCATION(source->GetResource(), 0);
	_commandList->CopyTextureRegion(&destLocation, x, y, z, &sourceLocation, &rect);
	PrepareCopyBuffer(source, dest);
}
#pragma region Protected Function
void CommandContext::PrepareCopyBuffer(GPUResource* dest, GPUResource* source)
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
#pragma endregion Protected Function