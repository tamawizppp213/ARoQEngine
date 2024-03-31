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
#include "GameUtility/Base/Include/GUParse.hpp"
#include "GameUtility/Base/Include/GUCommandLine.hpp"
#include <d3d12.h>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace gu;

#if PLATFORM_CPU_X86_FAMILY && PLATFORM_DESKTOP && _WIN64
extern "C" { _declspec(dllexport) extern const UINT  D3D12SDKVersion = 613; }
extern "C" { _declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; }
#endif

bool RHIInstance::SHOULD_CREATE_WITH_WARP  = false;
bool RHIInstance::ALLOW_SOFTWARE_RENDERING = false;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIInstance::RHIInstance(const core::RHIDebugCreateInfo& debugCreateInfo):
	core::RHIInstance(debugCreateInfo)
{
	/*-------------------------------------------------------------------
	-                   Enable CPU and GPU Debugger
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	if (debugCreateInfo.EnableCPUDebugger) { EnabledDebugLayer(); }

	if (debugCreateInfo.EnableGPUDebugger) { EnabledShaderBasedValidation();}

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

	/*-------------------------------------------------------------------
	-           CommandLineから必要なコマンドを取得する
	---------------------------------------------------------------------*/
	SHOULD_CREATE_WITH_WARP  = Parse::Contains(CommandLine::Get(), SP("warp"));
	ALLOW_SOFTWARE_RENDERING = Parse::Contains(CommandLine::Get(), SP("allowSoftwareRendering"));

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
*  @fn        gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchAdapter(const DXGI_GPU_PREFERENCE preference) const
* 
*  @brief     高性能または最小電力を示すAdapterを選択（（高）xGPU、dGPU iGPU（低）
* 
*  @param[in] const DXGI_GPU_PREFERENCE preference (high performance or minimum power)
* 
*  @return 　　gu::SharedPointer<core::RHIDisplayAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchAdapter(const DXGI_GPU_PREFERENCE gpuPreference) const
{
	/*-------------------------------------------------------------------
	-      次の物理デバイスをどの順番で調べるかを決定する関数
	---------------------------------------------------------------------*/
	auto ProceedNextAdapter = [&](const DXGI_GPU_PREFERENCE preference, const uint32 adapterIndex, AdapterComPtr& adapter)
	{
		#if DXGI_MAX_FACTORY_INTERFACE >= 6
			return _factory->EnumAdapterByGpuPreference(adapterIndex, preference, IID_PPV_ARGS(&adapter));
		#else
			return _factory->EnumAdapters(adapterIndex, (IDXGIAdapter**)adapter.GetAddressOf());
		#endif
	};

	// ProceedNextAdapterで一時的に引数として使用する物理デバイス
	AdapterComPtr tempAdapter = nullptr;
	AdapterComPtr firstSelectedAdapter   = nullptr; // 最初に取得出来た物理デバイス
	AdapterComPtr bestIntegratedAdapter = nullptr; // 最大容量のVideoMemoryを持った統合GPU
	AdapterComPtr bestDiscreteAdapter   = nullptr; // 最大容量のVideoMemoryを持ったDiscreteGPU

	/*-------------------------------------------------------------------
	-          物理デバイスの一覧を取得する 
	-   (見つけ次第ポインタを返すでも良いが, 数が少ない点とより詳細にカスタマイズが出来る点から)
	-   ProceedNextAdapterによって, DXGI_MAX_FACTIRY_INTERFACE >= 6である限りはソートされています.
	---------------------------------------------------------------------*/
	for (uint32 i = 0; ProceedNextAdapter(gpuPreference, i, tempAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		if (tempAdapter == nullptr) { continue; }

		// 物理デバイスの情報を取得します. 
		DXGI_ADAPTER_DESC adapterDesc = {};
		tempAdapter->GetDesc(&adapterDesc);

		// 自分好みのベンダータイプがあれば取得しておく
		const auto preferredVendor = GetPreferredAdapterVendor();

		/* WARP (Windows Advanced Rasterization Platform)アダプタかどうかを調べます. 
		*  WARPとは, D３Dハードウェアを使用できない場合に, 高速なレンダリングを有効化することが出来ます. */
		const bool isWARP = adapterDesc.VendorId == (uint32)core::DisplayAdapterVendorType::Microsoft;

		// WARPが要求された場合, 他のすべてのアダプタを拒否する. 
		// WARPが要求されていない場合、ソフトウェアレンダリングサポートが許可されていなければ、WARPデバイスを拒否する。
		const bool skipWARPProcess = (SHOULD_CREATE_WITH_WARP && !isWARP) || 
			                         (!SHOULD_CREATE_WITH_WARP && isWARP && !ALLOW_SOFTWARE_RENDERING);

		// 後続の処理を実行するかを決める
		const bool skipAdapterProcess = skipWARPProcess;

		/*-------------------------------------------------------------------
		-      対象の物理デバイスを見つける
		---------------------------------------------------------------------*/
		if (skipAdapterProcess) { continue; }

		if (firstSelectedAdapter == nullptr)
		{
			firstSelectedAdapter = tempAdapter;
		}

		const bool isDiscreteGPU = adapterDesc.DedicatedVideoMemory != 0;

		/*-------------------------------------------------------------------
		-     Discrete GPUの場合
		---------------------------------------------------------------------*/
		if (isDiscreteGPU)
		{
			const bool isFirstSelected   = bestDiscreteAdapter == nullptr;
			const bool isPreferredVender = preferredVendor != core::DisplayAdapterVendorType::Unknown && (uint32)preferredVendor == adapterDesc.VendorId;
			
			// まだDicreteGPUがない場合は強制的に見つけたことにする. 
			if (isFirstSelected) 
			{ 
				bestDiscreteAdapter = tempAdapter; 
				
				if (isPreferredVender) { break;    }
				else                   { continue; }
			}

			DXGI_ADAPTER_DESC bestDiscreteDesc = {};
			bestDiscreteAdapter->GetDesc(&bestDiscreteDesc);

			/*-------------------------------------------------------------------
			-     自分好みのベンダーがある場合はそれを使用して強制終了
			---------------------------------------------------------------------*/
			if (isPreferredVender)
			{
				bestDiscreteAdapter = tempAdapter;
				break;
			}
			/*-------------------------------------------------------------------
			-     自分好みのベンダーがない場合は最大容量のメモリを持つ物理デバイスを使用する
			---------------------------------------------------------------------*/
			else
			{
				if (adapterDesc.DedicatedVideoMemory > bestDiscreteDesc.DedicatedVideoMemory)
				{
					bestDiscreteAdapter = tempAdapter;
				}
			}
		}
		/*-------------------------------------------------------------------
		-     統合GPUの場合: そのまま最初に見つけたものを代入する
		---------------------------------------------------------------------*/
		else
		{
			bestIntegratedAdapter = tempAdapter;
		}
	}

	/*-------------------------------------------------------------------
	-        GPU Preferenceにあった物理デバイスを取得する. 
	        見つからない場合は強制的に最初に選んだものを使用する. 
	---------------------------------------------------------------------*/
	if (gpuPreference == DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_MINIMUM_POWER)
	{
		tempAdapter = bestIntegratedAdapter;
	}
	else if (gpuPreference == DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE)
	{
		tempAdapter = bestDiscreteAdapter;
	}
	else
	{
		tempAdapter = firstSelectedAdapter;
	}

	if (tempAdapter == nullptr) 
	{
		tempAdapter = firstSelectedAdapter;
	}

	// Adapterが見つからなかった場合はWARPデバイスを使用する
	return firstSelectedAdapter ? gu::MakeShared<RHIDisplayAdapter>(SharedFromThis(), tempAdapter) : nullptr;
}

/****************************************************************************
*                     EnumrateAdapters
*************************************************************************//**
*  @brief     全ての利用可能な物理デバイスを配列の形で返します.
* 
*  @param[in] void
* 
*  @return 　　gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> 物理デバイスの配列
*****************************************************************************/
gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> RHIInstance::EnumrateAdapters()
{
	gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> adapterLists = {};

	/*-------------------------------------------------------------------
	-                  Define Proceed next adapter function
	---------------------------------------------------------------------*/
	auto ProceedNextAdapter = [&](uint32 adapterIndex, AdapterComPtr& adapter)
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