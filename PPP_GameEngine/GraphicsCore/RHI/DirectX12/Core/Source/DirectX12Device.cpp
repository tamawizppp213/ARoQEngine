//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12Device.cpp
///             @brief  Logical Device : to use for create GPU resources 
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;
namespace
{
	enum class GPUVender
	{
		NVidia,
		AMD,
		Intel,
		CountOfGPUVender
	};
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHIDevice::RHIDevice()
{
	_apiVersion = rhi::core::APIVersion::DirectX12;
}
RHIDevice::~RHIDevice()
{
#ifdef _DEBUG
	ReportLiveObjects();
#endif
	if (_device) { _device = nullptr; }

}
/****************************************************************************
*                     Create
*************************************************************************//**
*  @fn        bool RHIDevice::Create()
*  @brief     Create DirectX12 Device
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool RHIDevice::Create(HWND hwnd, HINSTANCE hInstance, bool useRaytracing)
{
	_apiVersion = rhi::core::APIVersion::DirectX12;
	_hwnd = hwnd; _hInstance = hInstance;
	_enableRayTracing = useRaytracing;
	EnabledDebugLayer();
	/*-------------------------------------------------------------------
	-                   Create DXGIFactory
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_dxgiFactory)));
#else
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory)));
#endif
	/*-------------------------------------------------------------------
	-                   Search proper hardware adapter
	---------------------------------------------------------------------*/
	SearchHardwareAdapter();

	/*-------------------------------------------------------------------
	-                   Create Hardware Device
	---------------------------------------------------------------------*/
	HRESULT hardwareResult = D3D12CreateDevice(
		_useAdapter.Get(),      // default adapter
		D3D_FEATURE_LEVEL_12_0, // minimum feature level
		IID_PPV_ARGS(&_device));

	/*-------------------------------------------------------------------
	-                  Fallback to WARP Device
	-    (WARP: hight speed fully conformant software rasterizer)
	---------------------------------------------------------------------*/
	if (FAILED(hardwareResult))
	{
		AdapterComPtr warpAdapter;
		ThrowIfFailed(_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),        // video adapter
			D3D_FEATURE_LEVEL_12_0,   // minimum feature level
			IID_PPV_ARGS(&_device))); // device interface
		_isWarpAdapter = true;
	}
	_device->SetName(L"DirectX12::Device");
	/*-------------------------------------------------------------------
	-                   Log Adapters (for debug)
	---------------------------------------------------------------------*/
#if _DEBUG
	LogAdapters();
#endif

	if (useRaytracing) { CheckDXRSupport();}
	_isInitialize = true;
	return true;
}

std::shared_ptr<core::RHIFence> RHIDevice::CreateFence()
{
	return nullptr;
}
std::shared_ptr<core::RHICommandList>      RHIDevice::CreateCommandList()
{
	return nullptr;
}
std::shared_ptr<core::RHICommandQueue>     RHIDevice::CreateCommandQueue()
{
	return nullptr;
}
std::shared_ptr<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator()
{
	return nullptr;
}
std::shared_ptr<core::RHISwapchain>        RHIDevice::CreateSwapchain()
{
	return nullptr;
}

#pragma region Private Function
/****************************************************************************
*                     EnabledDebugLayer
*************************************************************************//**
*  @fn        void DirectX12::EnabledDebugLayer(void)
*  @brief     Enabled CPU debug layer
*  @param[in] void
*  @return 　　void 
*  @details   it must be called before the D3D12 device is created.
*****************************************************************************/
void RHIDevice::EnabledDebugLayer()
{
	DebugComPtr debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
	debugController.Reset();
}
/****************************************************************************
*                     SearchHardwareAdapter
*************************************************************************//**
*  @fn        void RHIDevice::SearchHardwareAdapter()
*  @brief     Search hardware adapter (Supported GPU: NVidia, AMD, Intel)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::SearchHardwareAdapter()
{
	/*-------------------------------------------------------------------
	-                   Search Hardware Adapter
	---------------------------------------------------------------------*/
	AdapterComPtr adapter;
	AdapterComPtr adapterVender[(int)GPUVender::CountOfGPUVender];
	size_t maxVideoMemory[(int)GPUVender::CountOfGPUVender] = { 0 };
	for (int i = 0; _dxgiFactory->EnumAdapters1(i, (IDXGIAdapter1**)adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		adapter->GetDesc1(&adapterDesc);
		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) { continue; }
		if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) { continue; }
		if (wcsstr(adapterDesc.Description, L"NVIDIA") != nullptr)
		{
			if (adapterDesc.DedicatedVideoMemory > maxVideoMemory[(int)GPUVender::NVidia])
			{
				if (adapterVender[(int)GPUVender::NVidia]) { adapterVender[(int)GPUVender::NVidia]->Release(); }
				adapterVender[(int)GPUVender::NVidia] = adapter;
				adapterVender[(int)GPUVender::NVidia]->AddRef();
			}
		}
		else if (wcsstr(adapterDesc.Description, L"AMD") != nullptr)
		{
			if (adapterDesc.DedicatedVideoMemory > maxVideoMemory[(int)GPUVender::AMD])
			{
				if (adapterVender[(int)GPUVender::AMD]) { adapterVender[(int)GPUVender::AMD]->Release(); }
				adapterVender[(int)GPUVender::AMD] = adapter;
				adapterVender[(int)GPUVender::AMD]->AddRef();
			}
		}
		else if (wcsstr(adapterDesc.Description, L"Intel") != nullptr)
		{
			if (adapterDesc.DedicatedVideoMemory > maxVideoMemory[(int)GPUVender::Intel])
			{
				if (adapterVender[(int)GPUVender::Intel]) { adapterVender[(int)GPUVender::Intel]->Release(); }
				adapterVender[(int)GPUVender::Intel] = adapter;
				adapterVender[(int)GPUVender::Intel]->AddRef();
			}
		}
		//adapter->Release();
	}
	for (int i = 0; i < (int)GPUVender::CountOfGPUVender; ++i)
	{
		if (adapterVender[i] != nullptr)
		{
			_useAdapter = adapterVender[i].Detach();
			break;
		}
	}

	

	for (int i = 0; i < (int)GPUVender::CountOfGPUVender; ++i)
	{
		if (adapterVender[i] != nullptr)
		{
			adapterVender[i]->Release();
		}
	}
	
}
/****************************************************************************
*                     LogAdapters
*************************************************************************//**
*  @fn        void DirectX12::LogAdapters(void)
*  @brief     Show all adapter name (GeForce... )
*  @param[in] Adapter* adapter
*  @return 　　void
*****************************************************************************/
void RHIDevice::LogAdapters()
{
	UINT i = 0;
	IAdapter* adapter = nullptr;
	std::vector<IAdapter*> adapterList;

	/*-------------------------------------------------------------------
	-                  Enum Adapter List
	---------------------------------------------------------------------*/
	while (_dxgiFactory->EnumAdapters(i, (IDXGIAdapter**)&adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	/*-------------------------------------------------------------------
	-                  Show Adapter List
	---------------------------------------------------------------------*/
	for (size_t j = 0; j < adapterList.size(); ++j)
	{
		LogAdapterOutputs(adapterList[j]);
		SAFE_RELEASE(adapterList[j]);
	}
}

/****************************************************************************
*                     LogAdapterOutputs
*************************************************************************//**
*  @fn        void DirectX12::LogAdapterOutputs(Adapter* adapter)
*  @brief     Show all display output name
*  @param[in] Adapter* adapter
*  @return 　　void
*****************************************************************************/
void RHIDevice::LogAdapterOutputs(IAdapter* adapter)
{
	/*-------------------------------------------------------------------
	-                     Initialize
	---------------------------------------------------------------------*/
	UINT i = 0;
	IOutput* output = nullptr;

	/*-------------------------------------------------------------------
	-                Show all adapter name
	---------------------------------------------------------------------*/
	while (adapter->EnumOutputs(i, (IDXGIOutput**)&output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, _backBufferFormat);

		SAFE_RELEASE(output);

		++i;
	}
}

/****************************************************************************
*                     LogOutputDisplayModes
*************************************************************************//**
*  @fn        void DirectX12::LogOutputDisplayModes(Output* output, DXGI_FORMAT format)
*  @brief     Show display modes (output screen width, height and refresh rates)
*  @param[in] Output* output: for display modelist
*  @param[in] DXGI_FORMAT   : format
*  @return 　　void
*****************************************************************************/
void RHIDevice::LogOutputDisplayModes(IOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	/*-------------------------------------------------------------------
	-             Call with nullptr to get list count
	---------------------------------------------------------------------*/
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	/*-------------------------------------------------------------------
	-                 Show display modes
	---------------------------------------------------------------------*/
	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}
/****************************************************************************
*                     ReportLiveObjects
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::ReportLiveObjects()
*  @brief     ReportLiveObjects
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::ReportLiveObjects()
{
	ID3D12DebugDevice2* debugInterface = nullptr;
	if (SUCCEEDED(_device.Get()->QueryInterface(&debugInterface)))
	{
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL); // Only Objects with external reference counts
		debugInterface->Release();
	}
}
/****************************************************************************
*						CheckDXRSupport
*************************************************************************//**
*  @fn        void DirectX12::CheckDXRSupport
*  @brief     Check DXRSupport
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckDXRSupport()
{
	bool enableRayTracing = true;
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options{};
	HRESULT result = _device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, UINT(sizeof(options)));
	if (FAILED(result) || options.RaytracingTier == D3D12_RAYTRACING_TIER_NOT_SUPPORTED)
	{
		std::wstring errorMessage = L"DirectX Raytracing not supported.";	
		OutputDebugString(errorMessage.c_str());
		enableRayTracing = false;
	}

	_enableRayTracing = enableRayTracing;
}
#pragma endregion Private Function