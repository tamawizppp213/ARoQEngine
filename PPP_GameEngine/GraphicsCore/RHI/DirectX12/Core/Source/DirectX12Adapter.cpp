//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIAdapter.hpp
///             @brief  Physical Device (adapter), Describe gpu information 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Adapter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <d3d12.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIDisplayAdapter::RHIDisplayAdapter(const std::shared_ptr<core::RHIInstance>& instance, const AdapterComPtr& adapter) : core::RHIDisplayAdapter(instance),_adapter(adapter)
{
	assert(("adapter is nullptr.\n", _adapter));

	DXGI_ADAPTER_DESC desc = {}; adapter->GetDesc(&desc);
	_name     = unicode::ToUtf8String(desc.Description);
	_venderID = desc.VendorId;
	_deviceID = desc.DeviceId; 
	_isDiscreteGPU = desc.DedicatedVideoMemory != 0;
}

RHIDisplayAdapter::~RHIDisplayAdapter()
{
	if (_adapter) { _adapter.Reset(); }
}
#pragma endregion Constructor and Destructor

#pragma region Factory
/****************************************************************************
*                     CreateDevice
*************************************************************************//**
*  @fn        std::shared_ptr<core::RHIDevice> RHIDisplayAdapter::CreateDevice(const std::uint32_t frameCount)
* 
*  @brief     Return directX12 logical device.
* 
*  @param[in] std::uint32_t frameCount
* 
*  @return    std::shared_ptr<core::RHIDevice> (directX12)
*****************************************************************************/
std::shared_ptr<core::RHIDevice> RHIDisplayAdapter::CreateDevice()
{
	return std::make_shared<RHIDevice>(shared_from_this());
}
#pragma endregion Factory

#pragma region Debug Function
/****************************************************************************
*                     PrintInfo
*************************************************************************//**
*  @fn        void RHIAdapter::PrintInfo()
* 
*  @brief     Print physical device information
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void RHIDisplayAdapter::PrintInfo()
{
	DXGI_ADAPTER_DESC desc = {}; 
	_adapter->GetDesc(&desc);
	
	/*-------------------------------------------------------------------
	-                  Print Adapter Name
	---------------------------------------------------------------------*/
	std::wstring adapterName  
		         = L"\n//////////////////////////\n Adapter : ";
	adapterName += desc.Description;
	adapterName += L"\n//////////////////////////\n";
	OutputDebugString(adapterName.c_str());

	/*-------------------------------------------------------------------
	-                  memory description
	---------------------------------------------------------------------*/
	const std::wstring systemMemoryStr       = L"System memory: "         + std::to_wstring(desc.DedicatedSystemMemory) + L"\n";
	const std::wstring videoMemoryStr        = L"Video memory : "         + std::to_wstring(desc.DedicatedVideoMemory) + L"\n";
	const std::wstring sharedSystemMemoryStr = L"Shared system memory : " + std::to_wstring(desc.SharedSystemMemory) + L"\n";
	
	OutputDebugString(systemMemoryStr.c_str());
	OutputDebugString(videoMemoryStr.c_str());
	OutputDebugString(sharedSystemMemoryStr.c_str());

	/*-------------------------------------------------------------------
	-                  Print Display Name
	---------------------------------------------------------------------*/
	IOutput* output = nullptr;

	for(int i = 0; _adapter->EnumOutputs(i, (IDXGIOutput**)&output) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		// Get output descriptor
		DXGI_OUTPUT_DESC outputDesc;
		output->GetDesc(&outputDesc);

		// Show Device Name to Output Debugger
		std::wstring text = L"\n***Output: ";
		text += outputDesc.DeviceName;
		text += L"n";
		OutputDebugString(text.c_str());

		// Release IOutput Pointer
		SAFE_RELEASE(output);
	}
}
#pragma endregion Debug Function