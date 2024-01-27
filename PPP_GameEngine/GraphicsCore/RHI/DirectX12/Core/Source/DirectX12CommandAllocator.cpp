//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12CommandAllocator.hpp
///             @brief  DirectX12CommandAllocator
///                     This class is used to store the command list memory.
///                     Every time a command list is reset, it will allocate a block of memory.
///                     Even if a command list is reset, these memories aren't lost. 
///                     When the Reset function in this class is called, these memories cleans up. 
///                     To acieve the maximum frame rate, you should create each command list one by one.
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12CommandAllocator.hpp"
#include "../Include/DirectX12Device.hpp"
#include "../Include/DirectX12EnumConverter.hpp"
#include "../Include/DirectX12Debug.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHICommandAllocator::~RHICommandAllocator()
{
	if (_commandAllocator) { _commandAllocator.Reset(); }
}

RHICommandAllocator::RHICommandAllocator(const gu::SharedPointer<rhi::core::RHIDevice>& device, const core::CommandListType type, const gu::wstring& name) : rhi::core::RHICommandAllocator(device, type)
{
	const auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(_device.Get())->GetDevice();

	ThrowIfFailed(dxDevice->CreateCommandAllocator(
		EnumConverter::Convert(type),              // Enable to execute all command 
		IID_PPV_ARGS(&_commandAllocator)));

	_commandAllocator->SetName(name.CString());
}
#pragma endregion Constructor and Desturctor

#pragma region Main Function
/****************************************************************************
*                     CleanUp
*************************************************************************//**
*  @fn        void RHICommandAllocator::CleanUp()
*
*  @brief     コマンドアロケーターをリセットします (コマンドバッファの削除)
			  あらゆるバインドされたコマンドリストはこの関数を呼ばれる前に閉じておく必要があります
			　加えて, GPU上のコマンドが実行されるまで, この関数は呼んではなりません
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandAllocator::CleanUp()
{
	ThrowIfFailed(_commandAllocator->Reset());
}

/****************************************************************************
*                     SetName
*************************************************************************//**
*  @fn        void RHICommandAllocator::CleanUp()
*
*  @brief     デバッグ表示名の設定
*
*  @param[in] const gu::wstring& name
*
*  @return    void
*****************************************************************************/
void RHICommandAllocator::SetName(const gu::wstring& name)
{
	_commandAllocator->SetName(name.CString());
}

#pragma endregion Main Function