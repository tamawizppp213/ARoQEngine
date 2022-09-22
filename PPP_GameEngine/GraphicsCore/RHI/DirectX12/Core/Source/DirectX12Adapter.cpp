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
	DXGI_ADAPTER_DESC desc = {}; adapter->GetDesc(&desc);
	_name     = unicode::ToUtf8String(desc.Description);
	_venderID = desc.VendorId;
	_deviceID = desc.DeviceId; 
}
RHIDisplayAdapter::~RHIDisplayAdapter()
{
	if (_adapter) { _adapter.Reset(); }
}
#pragma endregion Constructor and Destructor

#pragma region Factory
std::shared_ptr<core::RHIDevice> RHIDisplayAdapter::CreateDevice(const std::uint32_t frameCount)
{
	std::shared_ptr<core::RHIDevice> device = std::make_shared<RHIDevice>(shared_from_this(), frameCount);
	device->SetUp();
	return device;
}
#pragma endregion Factory

#pragma region Debug Function
/****************************************************************************
*                     PrintInfo
*************************************************************************//**
*  @fn        void RHIAdapter::PrintInfo()
*  @brief     Print physical device information
*  @param[in] void
*  @return @  void
*****************************************************************************/
void RHIDisplayAdapter::PrintInfo()
{
	DXGI_ADAPTER_DESC desc = {}; 
	_adapter->GetDesc(&desc);
	
	/*-------------------------------------------------------------------
	-                  Print Adapter Name
	---------------------------------------------------------------------*/
	std::wstring adapterName = L"\n\n***Adapter: ";
	adapterName += desc.Description;
	OutputDebugString(adapterName.c_str());

	/*-------------------------------------------------------------------
	-                  Print Display Name
	---------------------------------------------------------------------*/
	IOutput* output = nullptr;
	for(int i = 0; _adapter->EnumOutputs(i, (IDXGIOutput**)&output) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_OUTPUT_DESC outputDesc;
		output->GetDesc(&outputDesc);

		std::wstring text = L"\n***Output: ";
		text += outputDesc.DeviceName;
		text += L"n";
		OutputDebugString(text.c_str());
		SAFE_RELEASE(output);
	}
}
#pragma endregion Debug Function