//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (このエンジンを使用時最初に呼び出す.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Instance.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Adapter.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include <d3d12.h>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

#if PLATFORM_CPU_X86_FAMILY && PLATFORM_DESKTOP && _WIN64
extern "C" { _declspec(dllexport) extern const UINT  D3D12SDKVersion = 611; }
extern "C" { _declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; }
#endif

namespace
{
	enum class GPUVender
	{
		Nvidia,
		AMD,
		Intel,
		CountOfGPUVender
	};
}
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIInstance::RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger, bool useGPUDebugBreak):
	core::RHIInstance(enableCPUDebugger, enableGPUDebugger, useGPUDebugBreak)
{
	/*-------------------------------------------------------------------
	-                   Enable CPU and GPU Debugger
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	if (_enableCPUDebugger) { EnabledDebugLayer(); }

	if (_enableGPUDebugger) { EnabledShaderBasedValidation();}

	/*-------------------------------------------------------------------
	-                   Create Factory
	---------------------------------------------------------------------*/
	ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_factory)));
#else
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_factory)));
#endif
	
	/*-------------------------------------------------------------------
	-                   Create DRED
	---------------------------------------------------------------------*/
	EnabledGPUClashDebuggingModes();

	printf("StartInstance\n");
}

RHIInstance::~RHIInstance()
{
	if (_factory) { _factory.Reset(); }

	printf("DestroyInstance\n");
}

#pragma endregion Constructor and Destructor

#pragma region Public Function
/****************************************************************************
*                     SearchHighPerformanceAdapter
*************************************************************************//**
*  @fn        gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
* 
*  @brief     (Supported GPU: NVidia, AMD, Intel) VideoMemoryの多いものから 
*             (High) xGPU, dGPU iGPU (Low) selected
* 
*  @param[in] void
* 
*  @return 　　gu::SharedPointer<core::RHIDisplayAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
{
	return SearchAdapter(DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE);
}

/****************************************************************************
*                     SearchMinimumPowerAdapter
*************************************************************************//**
*  @fn        gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchMinimumPowerAdapter()
* 
*  @brief     (Supported GPU: NVidia, AMD, Intel) VideoMemoryの少ないものから
*             (Low) iGPU, dGPU xGPU (High)
* 
*  @param[in] void
* 
*  @return 　　gu::SharedPointer<core::RHIDisplayAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchMinimumPowerAdapter()
{
	return SearchAdapter(DXGI_GPU_PREFERENCE_MINIMUM_POWER);
}

/****************************************************************************
*                     SearchAdapter
*************************************************************************//**
*  @fn        gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchAdapter(const DXGI_GPU_PREFERENCE preference)
* 
*  @brief     Search Proper Physical Device Adapter
* 
*  @param[in] const DXGI_GPU_PREFERENCE preference (high performance or minimum power)
* 
*  @return 　　gu::SharedPointer<core::RHIDisplayAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchAdapter(const DXGI_GPU_PREFERENCE preference)
{
	AdapterComPtr adapter = nullptr;

	// (High) xGPU, dGPU iGPU (Low) selected
#if DXGI_MAX_FACTORY_INTERFACE >= 6
	if (_factory->EnumAdapterByGpuPreference(0, preference, IID_PPV_ARGS(&adapter)) == DXGI_ERROR_NOT_FOUND)
	{
		throw std::runtime_error("failed to search adapter");
	}
#else
	for (UINT i = 0; _factory->EnumAdapters(i, (IDXGIAdapter**)adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC desc; adapter->GetDesc(&desc);
		const bool isDiscreteGPU = desc.DedicatedVideoMemory != 0;

		if ((preference == DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE &&  isDiscreteGPU) ||  
			(preference == DXGI_GPU_PREFERENCE_MINIMUM_POWER    && !isDiscreteGPU))
		{
			break;
		}
	}
#endif

	return gu::MakeShared<RHIDisplayAdapter>(SharedFromThis(), adapter);
}

/****************************************************************************
*                     EnumrateAdapters
*************************************************************************//**
*  @fn        gu::DynamicArray<gu::SharedPointer<core::RHIAdapter>> EnumrateAdapters()
* 
*  @brief     Return all availablle adapter lists
* 
*  @param[in] void
* 
*  @return 　　gu::DynamicArray<gu::SharedPointer<core::RHIAdapter>>
*****************************************************************************/
gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> RHIInstance::EnumrateAdapters()
{
	gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> adapterLists = {};

	/*-------------------------------------------------------------------
	-                  Define Proceed next adapter function
	---------------------------------------------------------------------*/
	auto ProceedNextAdapter = [&](std::uint32_t adapterIndex, AdapterComPtr& adapter)
	{
#if DXGI_MAX_FACTORY_INTERFACE >= 6
		return _factory->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
#else
		return _factory->EnumAdapters(adapterIndex, (IDXGIAdapter**)adapter.GetAddressOf());
#endif
	};

	/*-------------------------------------------------------------------
	-                  EmplaceBack Adapter List
	---------------------------------------------------------------------*/
	AdapterComPtr adapter = nullptr;
	for (int i = 0; ProceedNextAdapter(i, adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		const auto thisInstance = SharedFromThis();
		const auto rhiAdapter = gu::MakeShared<RHIDisplayAdapter>(thisInstance, adapter);
		adapterLists.Push(rhiAdapter);
		adapter.Reset(); // memory leakに対処
	}

	return adapterLists;
}

/****************************************************************************
*                     LogAdapters
*************************************************************************//**
*  @fn        void RHIInstance::LogAdapters()
* 
*  @brief     Show all available adapter information using Output debug string
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHIInstance::LogAdapters()
{
	auto adapterList = EnumrateAdapters();
	
	for (auto& adapter : adapterList)
	{
		adapter->PrintInfo();
	}

}

/****************************************************************************
*                     HasLoadedDirectXAgilitySDK
*************************************************************************//**
*  @fn        bool RHIInstance::HasLoadedDirectXAgilitySDK() const
*
*  @brief     DirectXのAgilitySDK (最新バージョンのDirectX12を使用するできるか)を返します
*
*  @param[in] void
*
*  @return 　　bool
*****************************************************************************/
#if D3D12_CORE_ENABLED
bool RHIInstance::HasLoadedDirectXAgilitySDK() const
{
	return ::GetModuleHandleA("D3D12Core.dll") != NULL;
}
#endif
#pragma endregion Public Function


#pragma region Debugger
/****************************************************************************
*                     EnabledDebugLayer
*************************************************************************//**
*  @fn        void DirectX12::EnabledDebugLayer(void)
* 
*  @brief     Enabled CPU debug layer(debug mode only use)
* 
*  @param[in] void
* 
*  @return 　　void
* 
*  @details   it must be called before the D3D12 device is created.
*****************************************************************************/
void RHIInstance::EnabledDebugLayer()
{
	DebugComPtr debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	
	/* Switch on Debug Layer*/
	debugController->EnableDebugLayer();

	/* Release Debugger pointer*/
	debugController.Reset();
}

/****************************************************************************
*                   EnabledShaderBasedValiation
*************************************************************************//**
*  @fn        void DirectX12::EnabledShaderBasedValidation(void)
* 
*  @brief     Enabled GPU debugger (fpsに大きな影響を与えます.)
* 
*  @param[in] void
* 
*  @return 　　void
* 
*  @details   GPU-based valiation helps to identify the following errors.@n
*             1. Use of uninitialized or incompatible descriptors in a shader.@n
*             2. Use of descriptors referencing deleted Resources in a shader.@n
*             3. Validation of promoted resource states and resource state decay.@n
*             4. Indexing beyond the end of the descriptor heap in a shader.@n
*             5. Shader accesses of resources in incompatible state.@n
*             6. Use of uninitialized or incompatible Samplers in a shader.@n
*****************************************************************************/
void RHIInstance::EnabledShaderBasedValidation()
{
#if D3D12_MAX_DEBUG_INTERFACE >= 3
	DebugComPtr debugController;

	// Get Debugger
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));

	/* Switch on GPU Debugger*/
	debugController->SetEnableGPUBasedValidation(true);

	/* Release Debug Pointer*/
	debugController.Reset();
#endif
}

/****************************************************************************
*                     EnabledGPUClashDebuggingModes
*************************************************************************//**
*  @fn        void RHIInstance::EnabledGPUClashDebuggingModes()
*
*  @brief     GPU Clashの解析を行うため,　予期しないデバイス削除エラーが検出された後にDREDデータにアクセスし, エラー原因を解析できるようにします.
* 　　　　　　　　この関数はあくまで有効化するだけです.実際の検知は別で行います. 
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIInstance::EnabledGPUClashDebuggingModes()
{
	/*-------------------------------------------------------------------
	-                Dred setting default
	---------------------------------------------------------------------*/
#ifdef  __ID3D12DeviceRemovedExtendedDataSettings_INTERFACE_DEFINED__
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dredSettings = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(dredSettings.GetAddressOf()))))
	{
		// Turn on AutoBreadCrumbs and Page Fault reporting.
		dredSettings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);

		_useDRED = true;
		OutputDebugStringA("dred enabled\n");
	}
	else
	{
		OutputDebugStringA(" DRED requested but interface was not found, hresult: %x. DRED only works on Windows 10 1903+.");
	}
#endif

	/*-------------------------------------------------------------------
	-                Dred setting 1
	---------------------------------------------------------------------*/
#ifdef  __ID3D12DeviceRemovedExtendedDataSettings1_INTERFACE_DEFINED__
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings1> dredSettings1 = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(dredSettings1.GetAddressOf()))))
	{
		dredSettings1->SetBreadcrumbContextEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		_useDREDContext = true;
		OutputDebugStringA("dred breadcrumb context enabled\n");
	}
#endif

	/*-------------------------------------------------------------------
	-                Dred setting 2
	---------------------------------------------------------------------*/
#ifdef __ID3D12DeviceRemovedExtendedDataSettings2_INTERFACE_DEFINED__
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings2> dredSettings2 = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(dredSettings2.GetAddressOf()))))
	{
		dredSettings2->SetBreadcrumbContextEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings2->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings2->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings2->UseMarkersOnlyAutoBreadcrumbs(true);
		_useDREDContext = true;
		_useLightWeightDRED = true;
		OutputDebugStringA("dred breadcrumb context enabled\n");
	}
#endif
}
#pragma endregion Debugger
#pragma region Private Function