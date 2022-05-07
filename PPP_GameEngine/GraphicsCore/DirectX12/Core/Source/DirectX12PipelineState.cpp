//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12PipelineState.cpp
///             @brief  DirectX12PipelineState.cpp
///             @author Toide Yutaro
///             @date   2022_03_19
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12PipelineState.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12RootSignature.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GameUtility/Math/Include/GMHash.hpp"
#include <thread>
#include <map>
#include <mutex>
#pragma warning(disable: 6011)
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
static std::map<size_t, PipelineStateComPtr> s_GraphicsPSOHashMap; //文字列の代わりにhashIDで管理
static std::map<size_t, PipelineStateComPtr> s_ComputePSOHashMap;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
void PipelineState::DestroyAll()
{
	s_GraphicsPSOHashMap.clear();
	s_ComputePSOHashMap .clear();
}
PipelineState::~PipelineState()
{

}
GraphicsPipelineState::~GraphicsPipelineState()
{

}
#pragma region Graphics PSO
GraphicsPipelineState::GraphicsPipelineState() : PipelineState()
{
	ZeroMemory(&_psoDescriptor, sizeof(_psoDescriptor));
	_psoDescriptor.NodeMask                = 1;
	_psoDescriptor.pRootSignature          = nullptr;
	_psoDescriptor.SampleMask              = D3D12_DEFAULT_SAMPLE_MASK;
	_psoDescriptor.RasterizerState         = RASTERIZER_DESC(D3D12_DEFAULT);
	_psoDescriptor.BlendState              = BLEND_DESC(D3D12_DEFAULT);
	_psoDescriptor.PrimitiveTopologyType   = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_psoDescriptor.SampleDesc.Count        = 1;
	_psoDescriptor.SampleDesc.Quality      = 0;

}
/****************************************************************************
*                       SetRenderTargetFormats
*************************************************************************//**
*  @fn        void GraphicsPipelineState::SetRenderTargetFormats(UINT numRTVs, const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
*  @brief     Set render target format
*  @param[in] UINT num RTVs
*  @param[in] DXGI_FORMAT* 
*  @param[in] DXGI_FORMAT dsvFormat, 
*  @param[in] UINT msaaCount, 
*  @param[in] UINT msaaQuality
*  @return 　　void
*****************************************************************************/
void GraphicsPipelineState::SetRenderTargetFormats(UINT numRTVs, const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
{
	if (!(numRTVs == 0 || rtvFormats != nullptr)) { OutputDebugString(L"Null format array conflicts with non-zero length"); return; }

	for (UINT i = 0; i < numRTVs; ++i)
	{
		assert(rtvFormats[i] != DXGI_FORMAT_UNKNOWN);
		_psoDescriptor.RTVFormats[i] = rtvFormats[i];
	}
	for (UINT i = numRTVs; i < _psoDescriptor.NumRenderTargets; ++i)
	{
		_psoDescriptor.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
	}
	_psoDescriptor.NumRenderTargets   = numRTVs;
	_psoDescriptor.DSVFormat          = dsvFormat;
	_psoDescriptor.SampleDesc.Count   = msaaCount;
	_psoDescriptor.SampleDesc.Quality = msaaQuality;
}
/****************************************************************************
*                       SetInputLayouts
*************************************************************************//**
*  @fn        void GraphicsPipelineState::SetInputLayouts(UINT elementCount, const D3D12_INPUT_ELEMENT_DESC* inputElementDesc)
*  @brief     Set input layouts
*  @param[in] UINT elementCount
*  @param[in] const D3D12_INPUT_ELEMENT_DESC* inputElementDesc
*  @return 　　void
*****************************************************************************/
void GraphicsPipelineState::SetInputLayouts(const D3D12_INPUT_LAYOUT_DESC& desc)
{
	_psoDescriptor.InputLayout = desc;
}
/****************************************************************************
*                       CompleteSetting
*************************************************************************//**
*  @fn        void GraphicsPipelineState::CompleteSetting()
*  @brief     Complete setting PSO. Perform validation and compute a hash value for fast state block comparisons
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsPipelineState::CompleteSetting(IDevice* device, const std::wstring& name)
{
	/*-------------------------------------------------------------------
	-                      Set  RootSignature
	---------------------------------------------------------------------*/
	_psoDescriptor.pRootSignature = _rootSignature->GetSignature();
	assert(_psoDescriptor.pRootSignature != nullptr);
	/*-------------------------------------------------------------------
	-                      Set Input layout 
	---------------------------------------------------------------------*/
	size_t hashCode = gm::HashState(&_psoDescriptor);
	hashCode        = gm::HashState(_psoDescriptor.InputLayout.pInputElementDescs, _psoDescriptor.InputLayout.NumElements, hashCode);
	/*-------------------------------------------------------------------
	-                 Find pipelineState From HashMap
	---------------------------------------------------------------------*/
	IPipelineState** psoReference = nullptr;
	bool firstCompile = false;
	{
		static std::mutex hashMapMutex;
		std::lock_guard<std::mutex> lockGuard(hashMapMutex); // グローバル変数を扱うためにロック.
		
		auto iterator = s_GraphicsPSOHashMap.find(hashCode);
		if (iterator == s_GraphicsPSOHashMap.end())
		{
			firstCompile = true;
			psoReference = s_GraphicsPSOHashMap[hashCode].GetAddressOf();
		}
		else
		{
			psoReference = iterator->second.GetAddressOf();
		}

		// mutex unlock
	}
	/*-------------------------------------------------------------------
	-                Get PSO
	---------------------------------------------------------------------*/
	if (firstCompile)
	{

		assert(_psoDescriptor.DepthStencilState.DepthEnable != (_psoDescriptor.DSVFormat == DXGI_FORMAT_UNKNOWN));
		ThrowIfFailed(device->CreateGraphicsPipelineState(&_psoDescriptor, IID_PPV_ARGS(&_pipelineState)));
		s_GraphicsPSOHashMap[hashCode].Attach(_pipelineState);
		_pipelineState->SetName(name.c_str());
	}
	else
	{
		while (*psoReference == nullptr)
		{
			std::this_thread::yield(); 
		}
		_pipelineState = *psoReference;
	}
}
#pragma endregion Graphics PSO
#pragma region Compute PSO
ComputePipelineState::ComputePipelineState() : PipelineState()
{
	ZeroMemory(&_psoDescriptor, sizeof(_psoDescriptor));
	_psoDescriptor.NodeMask = 1;
}
/****************************************************************************
*                       CompleteSetting
*************************************************************************//**
*  @fn        void GraphicsPipelineState::CompleteSetting()
*  @brief     Complete setting PSO. Perform validation and compute a hash value for fast state block comparisons
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void ComputePipelineState::CompleteSetting(IDevice* device, const std::wstring& name)
{
	/*-------------------------------------------------------------------
	-                      Set  RootSignature
	---------------------------------------------------------------------*/
	_psoDescriptor.pRootSignature = _rootSignature->GetSignature();
	assert(_psoDescriptor.pRootSignature != nullptr);
	/*-------------------------------------------------------------------
	-                      Set Input layout 
	---------------------------------------------------------------------*/
	size_t hashCode = gm::HashState(&_psoDescriptor);
	/*-------------------------------------------------------------------
	-                 Find pipelineState From HashMap
	---------------------------------------------------------------------*/
	IPipelineState** psoReference = nullptr;
	bool firstCompile = false;
	{
		static std::mutex hashMapMutex;
		std::lock_guard<std::mutex> lockGuard(hashMapMutex);
		
		auto iterator = s_ComputePSOHashMap.find(hashCode);
		if (iterator == s_ComputePSOHashMap.end())
		{
			firstCompile = true;
			psoReference = s_ComputePSOHashMap[hashCode].GetAddressOf();
		}
		else
		{
			psoReference = iterator->second.GetAddressOf();
		}
	}
	/*-------------------------------------------------------------------
	-                Get PSO
	---------------------------------------------------------------------*/
	if (firstCompile)
	{
		ThrowIfFailed(device->CreateComputePipelineState(&_psoDescriptor, IID_PPV_ARGS(&_pipelineState)));
		s_ComputePSOHashMap[hashCode].Attach(_pipelineState);
		_pipelineState->SetName(name.c_str());
	}
	else
	{
		while (*psoReference == nullptr)
		{
			std::this_thread::yield();
		}
		_pipelineState = *psoReference;
	}
}
#pragma endregion Compute PSO