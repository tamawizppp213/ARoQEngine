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
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
PipelineState::~PipelineState()
{
	if (_pipelineState) { _pipelineState->Release(); }
	
}

#pragma region Graphics PSO
void GraphicsPipelineState::SetRenderTargetFormats(UINT numRTVs, const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
{
	if (numRTVs == 0 || rtvFormats == nullptr) { OutputDebugString(L"Null format array conflicts with non-zero length"); return; }

	for (UINT i = 0; i < numRTVs; ++i)
	{
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
void GraphicsPipelineState::SetInputLayouts(UINT elementCount, const D3D12_INPUT_ELEMENT_DESC* inputElementDesc)
{
	_psoDescriptor.InputLayout.NumElements = elementCount;

	if (elementCount > 0)
	{
		size_t memorySize = sizeof(D3D12_INPUT_ELEMENT_DESC) * elementCount;
		D3D12_INPUT_ELEMENT_DESC* newElements = (D3D12_INPUT_ELEMENT_DESC*)std::malloc(memorySize);
		if (newElements) { std::memcpy(newElements, inputElementDesc, memorySize); };
		_inputLayouts.reset(newElements,std::default_delete<D3D12_INPUT_ELEMENT_DESC>());
	}
	else
	{
		_inputLayouts = nullptr;
	}
}
#pragma endregion Graphics PSO