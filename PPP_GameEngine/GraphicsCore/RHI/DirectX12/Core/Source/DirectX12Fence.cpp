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
RHIFence::RHIFence(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::uint64_t initialValue)
{
	auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(device.get())->GetDevice();
	ThrowIfFailed(dxDevice->CreateFence(initialValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
	_fence->SetName(L"directX12::Fence");

	_device = device;
}


std::uint64_t RHIFence::GetCompletedValue()
{
	return _fence->GetCompletedValue();
}

/****************************************************************************
*                     Signal
*************************************************************************//**
*  @fn        void RHIFence::Signal(const std::shared_ptr<rhi::core::RHICommandQueue>& queue)
*  @brief     Signal issued
*  @param[in] std::uint64_t value
*  @return 　　void
*****************************************************************************/
void RHIFence::Signal(const std::uint64_t value)
{
	_fence->Signal(value);
}

/****************************************************************************
*                     Wait
*************************************************************************//**
*  @fn        void RHIFence::Wait(std::uint64_t value)
*  @brief     ---
*  @param[in] std::uint64_t value
*  @return 　　void
*****************************************************************************/
void RHIFence::Wait(const std::uint64_t value)
{
	/*-------------------------------------------------------------------
	-   Wait until the GPU has completed commands up to this fence point
	---------------------------------------------------------------------*/
	if (_fence->GetCompletedValue() < value)
	{
		const auto fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		ThrowIfFailed(_fence->SetEventOnCompletion(value, fenceEvent));

		if (fenceEvent != 0)
		{
			WaitForSingleObject(fenceEvent, INFINITE);
			CloseHandle(fenceEvent);
		}
	}
}