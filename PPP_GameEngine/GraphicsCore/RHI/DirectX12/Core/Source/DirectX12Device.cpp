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
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Swapchain.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Fence.hpp"
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
bool RHIDevice::Create(HWND hwnd, HINSTANCE hInstance, bool useHDR, bool useRaytracing)
{
	_apiVersion = rhi::core::APIVersion::DirectX12;
	_hwnd = hwnd; _hInstance = hInstance;
	_enableRayTracing        = useRaytracing;
	_isHDRSupport            = useHDR;
	/*-------------------------------------------------------------------
	-                   Create DXGIFactory
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	EnabledDebugLayer();
	//EnabledGPUBasedValidation();
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
	/*-------------------------------------------------------------------
	-                  Tearing support check
	---------------------------------------------------------------------*/
	CheckTearingSupport();
	if (useHDR) { CheckHDRDisplaySupport(); }
	_isInitialize = true;
	return true;
}

std::shared_ptr<core::RHIFence> RHIDevice::CreateFence()
{
	// https://suzulang.com/stdshared_ptr%E3%81%A7this%E3%82%92%E4%BD%BF%E3%81%84%E3%81%9F%E3%81%84%E6%99%82%E3%81%AB%E6%B3%A8%E6%84%8F%E3%81%99%E3%82%8B%E3%81%93%E3%81%A8/
	return std::static_pointer_cast<core::RHIFence>(std::make_shared<directX12::RHIFence>(shared_from_this()));
}
std::shared_ptr<core::RHICommandList> RHIDevice::CreateCommandList(const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator)
{
	return std::static_pointer_cast<core::RHICommandList>(std::make_shared<directX12::RHICommandList>(shared_from_this(),commandAllocator));
}
std::shared_ptr<core::RHICommandQueue> RHIDevice::CreateCommandQueue()
{
	return std::static_pointer_cast<core::RHICommandQueue>(std::make_shared<directX12::RHICommandQueue>(shared_from_this()));
}
std::shared_ptr<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator()
{
	return std::static_pointer_cast<core::RHICommandAllocator>(std::make_shared<directX12::RHICommandAllocator>(shared_from_this()));
}
std::shared_ptr<core::RHISwapchain> RHIDevice::CreateSwapchain(const std::shared_ptr<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount, const std::uint32_t vsync)
{
	return std::static_pointer_cast<core::RHISwapchain>(std::make_shared<directX12::RHISwapchain>(shared_from_this(), commandQueue, windowInfo, pixelFormat, frameBufferCount, vsync));
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
*                   EnabledGPUBasedValiation
*************************************************************************//**
*  @fn        void DirectX12::EnabledGPUBasedValidation(void)
*  @brief     Enabled GPU debugger (fpsに大きな影響を与えます.)
*  @param[in] void
*  @return 　　void
*  @details   GPU-based valiation helps to identify the following errors.@n
*             1. Use of uninitialized or incompatible descriptors in a shader.@n
*             2. Use of descriptors referencing deleted Resources in a shader.@n
*             3. Validation of promoted resource states and resource state decay.@n
*             4. Indexing beyond the end of the descriptor heap in a shader.@n
*             5. Shader accesses of resources in incompatible state.@n
*             6. Use of uninitialized or incompatible Samplers in a shader.@n
*****************************************************************************/
void RHIDevice::EnabledGPUBasedValidation()
{
	DebugComPtr debugController;
	ComPtr<ID3D12Debug3> debug3;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	ThrowIfFailed(debugController->QueryInterface(IID_PPV_ARGS(&debug3)));
	debug3->SetEnableGPUBasedValidation(true);
	debugController.Reset();
	debug3.Reset();


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
/****************************************************************************
*                     CheckTearingSupport
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CheckTearingSupport()
*  @brief     Tearing support
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckTearingSupport()
{
	if (!_dxgiFactory) { _isTearingSupport = false; return; }

	int allowTearing = false;
	if (FAILED(_dxgiFactory->CheckFeatureSupport(
		DXGI_FEATURE_PRESENT_ALLOW_TEARING,
		&allowTearing, sizeof(allowTearing))))
	{
		_isTearingSupport = false;
	}
	else
	{
		_isTearingSupport = true;
	}

}
/****************************************************************************
*                     CheckVRSSupport
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CheckVRSSupport()
*  @brief     Variable Rate Shading support
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckVRSSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS6 options = {};
	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &options, sizeof(options))))
	{
		if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_1)
		{
			OutputDebugStringA("Gpu api: Variable Rate Shading Tier1 supported");
			_isVariableRateShadingTier1Supported = true;
			_isVariableRateShadingTier2Supported = false;
		}
		else if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_2)
		{
			OutputDebugStringA("Gpu api: Valiable Rate Shading Tier2 supported");
			_isVariableRateShadingTier1Supported = true;
			_isVariableRateShadingTier2Supported = true;
			_variableRateShadingImageTileSize = options.ShadingRateImageTileSize;
		}
	}
	else
	{
		OutputDebugStringA("GpuApi : Variable Rate Shading note supported on current gpu hardware.");
		_isVariableRateShadingTier1Supported = false;
		_isVariableRateShadingTier2Supported = false;
	}
}
/****************************************************************************
*                     MultiSampleQualityLevels
*************************************************************************//**
*  @fn        void DirectX12::CheckMultiSampleQualityLevels(void)
*  @brief     Multi Sample Quality Levels for 4xMsaa (Anti-Alias)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckMultiSampleQualityLevels()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels = {};
	msQualityLevels.Format = _backBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	ThrowIfFailed(_device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

	_4xMsaaQuality = msQualityLevels.NumQualityLevels;
#ifdef _DEBUG
	assert(_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
#endif
}

/****************************************************************************
*                     CheckHDRDisplaySupport
*************************************************************************//**
*  @fn        bool DirectX12::CheckHDRDisplaySupport()
*  @brief     CheckHDRDisplaySupport()
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckHDRDisplaySupport()
{
	bool isEnabledHDR =
		_backBufferFormat == DXGI_FORMAT_R16G16B16A16_FLOAT ||
		_backBufferFormat == DXGI_FORMAT_R10G10B10A2_UNORM;

	if (isEnabledHDR)
	{
		bool isDisplayHDR10 = false;
		UINT index = 0;

		ComPtr<IDXGIOutput> currentOutput = nullptr;
		ComPtr<IDXGIOutput> bestOutput = nullptr;
		while (_useAdapter->EnumOutputs(index, &currentOutput) != DXGI_ERROR_NOT_FOUND)
		{
			OutputComPtr output;
			currentOutput.As(&output);

			DXGI_OUTPUT_DESC1 desc;
			output->GetDesc1(&desc);

			isDisplayHDR10 = (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
			++index;
			if (isDisplayHDR10) { break; }
		}

		if (!isDisplayHDR10)
		{
			//_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			isEnabledHDR = false;
		}
	}

	_isHDRSupport = isEnabledHDR;

}
#pragma endregion Private Function