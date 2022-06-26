//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12Device.cpp
///             @brief  Logical Device : to use for create GPU resources 
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Fence.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHIFence::RHIFence(const std::shared_ptr<rhi::core::RHIDevice>& device)
{
	auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(device.get())->GetDevice();
	ThrowIfFailed(dxDevice->CreateFence(_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
	_fence->SetName(L"directX12::Fence");

	_device = device;
}
/****************************************************************************
*                     Signal
*************************************************************************//**
*  @fn        void RHIFence::Signal(const std::shared_ptr<rhi::core::RHICommandQueue>& queue)
*  @brief     Signal issued, and waiting for GPU processing
*  @param[in] const std::shared_ptr<rhi::core::RHICommandQueue>& queue
*  @return Å@Å@void
*****************************************************************************/
void RHIFence::Signal(const std::shared_ptr<rhi::core::RHICommandQueue>& queue)
{
	auto commandQueue = static_cast<rhi::directX12::RHICommandQueue*>(queue.get())->GetCommandQueue();
	commandQueue->Signal(_fence.Get(), ++_fenceValue);
	/*-------------------------------------------------------------------
	-   Wait until the GPU has completed commands up to this fence point
	---------------------------------------------------------------------*/
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		const auto fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		ThrowIfFailed(_fence->SetEventOnCompletion(_fenceValue, fenceEvent));

		if (fenceEvent != 0)
		{
			WaitForSingleObject(fenceEvent, INFINITE);
			CloseHandle(fenceEvent);
		}
	}
}