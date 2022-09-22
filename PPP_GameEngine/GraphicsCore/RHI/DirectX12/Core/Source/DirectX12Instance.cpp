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
#include <d3d12.h>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

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
#pragma region Constructor and Destructor
RHIInstance::RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger):
	core::RHIInstance(enableCPUDebugger, enableGPUDebugger)
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
	
}

RHIInstance::~RHIInstance()
{
	if (_factory) { _factory.Reset(); }
	printf("DestroyInstance");
}
#pragma endregion Constructor and Destructor
#pragma region Public Function
/****************************************************************************
*                     SearchHighPerformanceAdapter
*************************************************************************//**
*  @fn        std::shared_ptr<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
*  @brief     (Supported GPU: NVidia, AMD, Intel) VideoMemoryの多いものから 
*             (High) xGPU, dGPU iGPU (Low) selected
*  @param[in] void
*  @return 　　std::shared_ptr<core::RHIDisplayAddapter>
*****************************************************************************/
std::shared_ptr<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
{
	AdapterComPtr adapter = nullptr;
	if (_factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) == DXGI_ERROR_NOT_FOUND)
	{
		throw std::runtime_error("failed to search adapter");
	}
	return std::make_shared<RHIDisplayAdapter>(shared_from_this(), adapter);
}
/****************************************************************************
*                     EnumrateAdapters
*************************************************************************//**
*  @fn        std::vector<std::shared_ptr<core::RHIAdapter>> EnumrateAdapters()
*  @brief     Return all availablle adapter lists
*  @param[in] void
*  @return 　　std::vector<std::shared_ptr<core::RHIAdapter>>
*****************************************************************************/
std::vector<std::shared_ptr<core::RHIDisplayAdapter>> RHIInstance::EnumrateAdapters()
{
	std::vector<std::shared_ptr<core::RHIDisplayAdapter>> adapterLists = {};

	/*-------------------------------------------------------------------
	-                  Define Proceed next adapter function
	---------------------------------------------------------------------*/
	auto ProceedNextAdapter = [&](std::uint32_t adapterIndex, AdapterComPtr& adapter)
	{
		return _factory->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
	};

	/*-------------------------------------------------------------------
	-                  EmplaceBack Adapter List
	---------------------------------------------------------------------*/
	AdapterComPtr adapter = nullptr;
	for (int i = 0; ProceedNextAdapter(i, adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		adapterLists.emplace_back(std::make_shared<RHIDisplayAdapter>(shared_from_this(), adapter));
	}
	return adapterLists;
}
/****************************************************************************
*                     LogAdapters
*************************************************************************//**
*  @fn        void RHIInstance::LogAdapters()
*  @brief     Show all available adapter information using Output debug string
*  @param[in] void
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
#pragma endregion Public Function


#pragma region Debugger
/****************************************************************************
*                     EnabledDebugLayer
*************************************************************************//**
*  @fn        void DirectX12::EnabledDebugLayer(void)
*  @brief     Enabled CPU debug layer(debug mode only use)
*  @param[in] void
*  @return 　　void
*  @details   it must be called before the D3D12 device is created.
*****************************************************************************/
void RHIInstance::EnabledDebugLayer()
{
	DebugComPtr debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer(); // debug layer on
	debugController.Reset();
}
/****************************************************************************
*                   EnabledShaderBasedValiation
*************************************************************************//**
*  @fn        void DirectX12::EnabledShaderBasedValidation(void)
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
void RHIInstance::EnabledShaderBasedValidation()
{
	DebugComPtr debugController;
	ComPtr<ID3D12Debug3> debug3;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	ThrowIfFailed(debugController->QueryInterface(IID_PPV_ARGS(&debug3)));
	debug3->SetEnableGPUBasedValidation(true);
	debugController.Reset();
	debug3.Reset();
}
#pragma endregion Debugger
#pragma region Private Function