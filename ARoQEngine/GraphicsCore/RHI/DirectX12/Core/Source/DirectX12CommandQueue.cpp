//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12CommandQueue.hpp
///             @brief  Send drawing commands to the GPU,
///                     provide methods for synchronous processing of drawing command execution
///                     There are three types : Graphics, Compute, Copy command queue.
///             @author Toide Yutaro
///             @date   2022_09_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Fence.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Adapter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Instance.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
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
RHICommandQueue::RHICommandQueue(const gu::SharedPointer<rhi::core::RHIDevice>& device, core::CommandListType type, const gu::tstring& name) : rhi::core::RHICommandQueue(device, type)
{
	Check(device);
	Check(type != core::CommandListType::Unknown);

	const auto dxDevice   = static_cast<RHIDevice*>(device.Get())->GetDevice();
	const auto dxAdapter  = gu::StaticPointerCast<RHIDisplayAdapter>(device->GetDisplayAdapter());
	const auto dxInstance = static_cast<RHIInstance*>(dxAdapter->GetInstance());

	/*-------------------------------------------------------------------
	-                   Set up command queue info
	---------------------------------------------------------------------*/
	const auto dxCommandListType        = EnumConverter::Convert(_commandListType);
	const auto fullSupportDebuggingMode = dxInstance->GetGPUCrashDebuggingMode() == GPUCrashDebuggingMode::All;
	const D3D12_COMMAND_QUEUE_DESC cmdQDesc =
	{
		.Type     = dxCommandListType,                    // Enable to execute all command 
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,  // Command queue priority (����ς��邩��)
		                                                  // �f�o�b�O���Ă���^�C�~���O�ł�, GPU�̃^�C���A�E�g��off�ɂ��邱�ƂŃf�o�b�O��r���Ő؂�Ȃ�.
		.Flags    = fullSupportDebuggingMode ? D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT : D3D12_COMMAND_QUEUE_FLAG_NONE,        // Timeout�͏�ɗL���ɂ��Ă���.
		.NodeMask = device->GetGPUMask().Value()          //�@GPU�}�X�N���g���ă}���`GPU�ɂ��Ή��ł���悤�ɕύX
	};

	/*-------------------------------------------------------------------
	-                   Create command queue
	---------------------------------------------------------------------*/
	ThrowIfFailed(dxDevice->CreateCommandQueue(&cmdQDesc, IID_PPV_ARGS(&_commandQueue)));
	_commandQueue->SetName(name.CString());
}

RHICommandQueue::~RHICommandQueue()
{
	if (_commandQueue) { _commandQueue.Reset(); }
}

#pragma endregion Constructor and Destructor
#pragma region Execute Function
/****************************************************************************
*							Wait
*************************************************************************//**
*  @fn        void RHICommandQueue::Wait(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t value)
* 
*  @brief     Used to wait for another Command queue to complete execution. (in GPU)
*             Fence��Wait��CPU�����������~�܂�܂���, CommandQueue��Wait��, �w�肵��Value�ȏ�̒l�ɂȂ�܂�, GPU���݂̂ŏ������~�߂邱�ƂɂȂ�܂�. 
* 
*  @param[in] const gu::SharedPointer<core::RHIFence>& fence
*  @param[in] std::uint64_t value
* 
*  @return �@�@void
*****************************************************************************/
void RHICommandQueue::Wait(const gu::SharedPointer<core::RHIFence>& fence, gu::uint64 value)
{
	FenceComPtr dxFence = gu::StaticPointerCast<RHIFence>(fence)->GetFence();
	ThrowIfFailed(_commandQueue->Wait(dxFence.Get(), value));
}

/****************************************************************************
*							Signal
*************************************************************************//**
*  @fn        void RHICommandQueue::Signal(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t value)
* 
*  @brief     Update value when the Command Queue execution completes.
*             GPU���ŏ������������܂�.�@
* 
*  @param[in] const gu::SharedPointer<core::RHIFence>& fence
* 
*  @param[in] std::uint64_t value
* 
*  @return �@�@void
*****************************************************************************/
void RHICommandQueue::Signal(const gu::SharedPointer<core::RHIFence>& fence, gu::uint64 value)
{
	FenceComPtr dxFence = gu::StaticPointerCast<RHIFence>(fence)->GetFence();
	ThrowIfFailed(_commandQueue->Signal(dxFence.Get(), value));
}

/****************************************************************************
*							Execute
*************************************************************************//**
*  @fn        void RHICommandQueue::Execute(const gu::DynamicArray<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists)
* 
*  @brief     Execute command list contents. normally set graphics, compute, transfer commandlist
* 
*  @param[in] const gu::DynamicArray<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists
* 
*  @return �@�@void
*****************************************************************************/
void RHICommandQueue::Execute(const gu::DynamicArray<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists)
{
	if (commandLists.IsEmpty()) { return; }

	/*-------------------------------------------------------------------
	-           Push back command list pointer 
	-      to use directX12 commandlist (ID3D12CommandList pointer)
	---------------------------------------------------------------------*/
	gu::DynamicArray<ID3D12CommandList*> dxCommandLists;
	for (auto& list : commandLists)
	{
		dxCommandLists.Push(gu::StaticPointerCast<rhi::directX12::RHICommandList>(list)->GetCommandList().Get());
	}

	/*-------------------------------------------------------------------
	-                   Execute command lists
	---------------------------------------------------------------------*/
	if (dxCommandLists.IsEmpty()) { return; }
	_commandQueue->ExecuteCommandLists(static_cast<UINT>(dxCommandLists.Size()), dxCommandLists.Data());
}

/****************************************************************************
*							GetTimestampFrequency
*************************************************************************//**
*  @fn        gu::uint64 RHICommandQueue::GetTimestampFrequency()
*
*  @brief     �R�}���h�L���[����GPU�^�C���X�^���v��Hz�P�ʂŕԂ��܂�.
*
*  @param[in] void
*
*  @return �@�@gu::uint64 timestamp[Hz]
*****************************************************************************/
gu::uint64 RHICommandQueue::GetTimestampFrequency()
{
	gu::uint64 frequency = 0;
	_commandQueue->GetTimestampFrequency(&frequency);
	return frequency;
}


/****************************************************************************
*							GetCalibrationTimestamp
*************************************************************************//**
*  @fn        core::GPUTimingCalibrationTimestamp RHICommandQueue::GetCalibrationTimestamp()
*
*  @brief     GPU��CPU�̌v�����Ԃ��擾���܂�
*
*  @param[in] void
*
*  @return �@�@core::GPUTimingCalibrationTimestamp
*****************************************************************************/
core::GPUTimingCalibrationTimestamp RHICommandQueue::GetCalibrationTimestamp()
{
	/*-------------------------------------------------------------------
	-                  Frequency�̎擾
	---------------------------------------------------------------------*/
	auto gpuTimestampFrequency = GetTimestampFrequency();
	
	LARGE_INTEGER cpuTimestampFrequency ;
	QueryPerformanceFrequency(&cpuTimestampFrequency);

	/*-------------------------------------------------------------------
	-                  Timestamp�̎擾
	---------------------------------------------------------------------*/
	gu::uint64 gpuTimestamp = 0;
	gu::uint64 cpuTimestamp = 0;

	ThrowIfFailed(_commandQueue->GetClockCalibration(&gpuTimestamp, &cpuTimestamp));

	/*-------------------------------------------------------------------
	-                  �v�����Ԃ̎擾
	---------------------------------------------------------------------*/
	core::GPUTimingCalibrationTimestamp timestampResult;
	timestampResult.GPUMicroseconds = static_cast<gu::uint64>(gpuTimestamp * (1e6 / gpuTimestampFrequency));
	timestampResult.CPUMicroseconds = static_cast<gu::uint64>(cpuTimestamp * (1e6 / cpuTimestampFrequency.QuadPart));
	return timestampResult;
}
#pragma endregion Execute

#pragma region Property
void RHICommandQueue::SetName(const gu::tstring& name)
{
	_commandQueue->SetName(name.CString());
}

#pragma endregion Property