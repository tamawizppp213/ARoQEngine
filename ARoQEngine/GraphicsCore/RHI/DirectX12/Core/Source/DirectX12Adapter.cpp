//////////////////////////////////////////////////////////////////////////////////
///          DirectX12Adapter.cpp
///  @brief  論理デバイスに渡す物理デバイス(Apdapter)の設定, GPU情報を取得
///  @author Toide Yutaro
///  @date   2024_03_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Adapter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include <d3d12.h> // これ自体windows.h依存なのか

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIDisplayAdapter::RHIDisplayAdapter(const gu::SharedPointer<core::RHIInstance>& instance, const AdapterComPtr& adapter) : core::RHIDisplayAdapter(instance),_adapter(adapter)
{
	Checkf(instance, "instance is nullptr");
	Checkf(_adapter, "adapter is nullptr.\n");

	DXGI_ADAPTER_DESC3 desc = {};
	adapter->GetDesc3(&desc);
	
	const auto utf8Name = unicode::ToUtf8String(desc.Description);
	_name     = gu::string(utf8Name.c_str(), utf8Name.length());
	_venderID = desc.VendorId;
	_deviceID = desc.DeviceId;
	_subSysID = desc.SubSysId;
	_dedicatedVideoMemory  = desc.DedicatedVideoMemory;
	_dedicatedSystemMemory = desc.DedicatedSystemMemory;
	_sharedSystemMemory    = desc.SharedSystemMemory;

	_isDiscreteGPU = desc.DedicatedVideoMemory != 0;
}

RHIDisplayAdapter::~RHIDisplayAdapter()
{
	if (_adapter) { _adapter.Reset(); }
}
#pragma endregion Constructor and Destructor

#pragma region Factory
/*!**********************************************************************
*  @brief 自身の物理デバイスに基づいて論理デバイスを生成し, そのSharedPointerを渡します.
*  @param[in] void
*  @return gu::SharedPointer<core::RHIDevice> 論理デバイスのポインタ
*************************************************************************/
gu::SharedPointer<core::RHIDevice> RHIDisplayAdapter::CreateDevice()
{
	return gu::MakeShared<RHIDevice>(SharedFromThis());
}
#pragma endregion Factory

#pragma region Debug Function
/*!**********************************************************************
*  @brief 物理デバイスの名前とスペックを出力に表示します@n
*         基本的に実行時のログとして使用するものになります. @n
*         ファイルや文字列に出力は行わないです.
*  @param[in] void
*  @return    void
*************************************************************************/
void RHIDisplayAdapter::PrintInfo() const
{
	DXGI_ADAPTER_DESC desc = {}; 
	_adapter->GetDesc(&desc);
	
	/*-------------------------------------------------------------------
	-                  Print Adapter Name
	---------------------------------------------------------------------*/
	gu::wstring adapterName  
		         = L"\n//////////////////////////\n Adapter : ";
	adapterName += desc.Description;
	adapterName += L"\n//////////////////////////\n";

	_RPTWN(_CRT_WARN, L"%s", adapterName.CString());

	/*-------------------------------------------------------------------
	-                  memory description
	---------------------------------------------------------------------*/
	const auto systemMemoryStr       = gu::wstring(L"System memory: ") + gu::wstring::FromNumber(desc.DedicatedSystemMemory);
	const auto videoMemoryStr        = gu::wstring(L"Video memory : ") + gu::wstring::FromNumber(desc.DedicatedVideoMemory);
	const auto sharedSystemMemoryStr = gu::wstring(L"Shared system memory :") + gu::wstring::FromNumber(desc.SharedSystemMemory);
	
	_RPTWN(_CRT_WARN, L"%s\n", systemMemoryStr.CString());
	_RPTWN(_CRT_WARN, L"%s\n", videoMemoryStr.CString());
	_RPTWN(_CRT_WARN, L"%s\n", sharedSystemMemoryStr.CString());

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
		gu::wstring text(outputDesc.DeviceName);

		_RPTWN(_CRT_WARN, L"%s\n", text.CString());

		// Release IOutput Pointer
		SAFE_RELEASE(output);
	}
}

/*!**********************************************************************
*  @brief     アダプタに直接接続されている出力の数 (モニターなど)を返します.
*  @param[in] void
*  @return    gu::uint64 アダプタに直接されている出力の数
*************************************************************************/
uint64 RHIDisplayAdapter::GetOutputCount() const 
{
	uint64   result = 0;
	IOutput* output = nullptr;

	for (uint32 i = 0; _adapter->EnumOutputs(i, (IDXGIOutput**)&output) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		result++;
	}

	return result;
}

#pragma endregion Debug Function