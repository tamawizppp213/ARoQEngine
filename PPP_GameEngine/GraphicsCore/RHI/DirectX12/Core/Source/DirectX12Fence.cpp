//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12Fence.cpp
///             @brief  CPU-GPU synchronization
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
#pragma region Constructor and Destructor
RHIFence::RHIFence(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::uint64_t initialValue, const std::wstring& name)
{
	auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(device.get())->GetDevice();
	
	ThrowIfFailed(dxDevice->CreateFence(initialValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
	
	_fence->SetName(name.c_str());

	_device = device;
}

RHIFence::~RHIFence()
{
	if (_fence) { _fence.Reset(); }
}
#pragma endregion Constructor and Destructor
#pragma region Public Function
/****************************************************************************
*                     GetCompletedValue
*************************************************************************//**
*  @fn        std::uint64_t RHIFence::GetCompletedValue()
* 
*  @brief     Return current fence value
* 
*  @param[in] void
* 
*  @return 　　std::uint64_t fenceValue
*****************************************************************************/
std::uint64_t RHIFence::GetCompletedValue()
{
	return _fence->GetCompletedValue();
}

/****************************************************************************
*                     Signal
*************************************************************************//**
*  @fn        void RHIFence::Signal(const std::shared_ptr<rhi::core::RHICommandQueue>& queue)
* 
*  @brief     @brief: Set fence value from CPU side. 
              (in case RHICommandQueue::Signal -> Set fence value from GPU side)
* 
*  @param[in] std::uint64_t value
* 
*  @return 　　void
*****************************************************************************/
void RHIFence::Signal(const std::uint64_t value)
{
	ThrowIfFailed(_fence->Signal(value));
}

/****************************************************************************
*                     Wait
*************************************************************************//**
*  @fn        void RHIFence::Wait(std::uint64_t value)
* 
*  @brief     Detect the completion of GPU processing on the CPU side. If not, it waits.
* 
*  @param[in] std::uint64_t value
* 
*  @return 　　void
*****************************************************************************/
void RHIFence::Wait(const std::uint64_t value)
{
	/*-------------------------------------------------------------------
	-   Wait until the GPU has completed commands up to this fence point
	    (GPU処理が完了していない場合, 待つ (command queueと併用))
	---------------------------------------------------------------------*/
	if (_fence->GetCompletedValue() >= value) { return; }

	const auto fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	ThrowIfFailed(_fence->SetEventOnCompletion(value, fenceEvent));

	if (fenceEvent != 0)
	{
		WaitForSingleObject(fenceEvent, INFINITE);
		CloseHandle(fenceEvent);
	}
}
#pragma endregion Public Function

#pragma region Property
void RHIFence::SetName(const std::wstring& name)
{
	_fence->SetName(name.c_str());
}

#pragma endregion Property