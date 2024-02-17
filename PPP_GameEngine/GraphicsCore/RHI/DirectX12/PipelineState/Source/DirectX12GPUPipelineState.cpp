//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GPUPipelineState.cpp
///             @brief  Pipeline State (Graphics and Compute)
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUPipelineState.hpp"
#include "../Include/DirectX12GPUBlendState.hpp"
#include "../Include/DirectX12GPUDepthStencilState.hpp"
#include "../Include/DirectX12GPUInputAssemblyState.hpp"
#include "../Include/DirectX12GPURasterizerState.hpp"
#include "../Include/DirectX12GPUShaderState.hpp"
#include "../../Core/Include/DirectX12ResourceLayout.hpp"
#include "../../Core/Include/DirectX12Device.hpp"
#include "../../Core/Include/DirectX12Debug.hpp"
#include "../../Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Graphic PSO
void GPUGraphicsPipelineState::CompleteSetting()
{
	DeviceComPtr dxDevice = nullptr;
	dxDevice = static_cast<RHIDevice*>(_device.Get())->GetDevice();

	/*-------------------------------------------------------------------
	-                      Set Descriptor
	---------------------------------------------------------------------*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.PrimitiveTopologyType = EnumConverter::Convert1(_inputAssemblyState->GetPrimitiveTopology());
	desc.pRootSignature        = _resourceLayout      ? static_cast<RHIResourceLayout*>(_resourceLayout.Get())->GetRootSignature().Get() : nullptr;
	desc.InputLayout           = _inputAssemblyState  ? static_cast<GPUInputAssemblyState*>(_inputAssemblyState.Get())->GetLayout()            : D3D12_INPUT_LAYOUT_DESC();
	desc.VS                    = _vertexShaderState   ? static_cast<GPUShaderState*>      (_vertexShaderState  .Get())->GetShader()            : D3D12_SHADER_BYTECODE();
	desc.PS                    = _pixelShaderState    ? static_cast<GPUShaderState*>      (_pixelShaderState   .Get())->GetShader()            : D3D12_SHADER_BYTECODE();
	desc.HS                    = _hullShaderState     ? static_cast<GPUShaderState*>      (_hullShaderState    .Get())->GetShader()            : D3D12_SHADER_BYTECODE();
	desc.DS                    = _domainShaderState   ? static_cast<GPUShaderState*>      (_domainShaderState  .Get())->GetShader()            : D3D12_SHADER_BYTECODE();
	desc.GS                    = _geometryShaderState ? static_cast<GPUShaderState*>      (_geometryShaderState.Get())->GetShader()            : D3D12_SHADER_BYTECODE();
	desc.DepthStencilState     = _depthStencilState   ? static_cast<GPUDepthStencilState*>(_depthStencilState  .Get())->GetDepthStencilState() : D3D12_DEPTH_STENCIL_DESC();
	desc.BlendState            = _blendState          ? static_cast<GPUBlendState*>       (_blendState         .Get())->GetBlendState()        : D3D12_BLEND_DESC();
	desc.RasterizerState       = _rasterizerState     ? static_cast<GPURasterizerState*>  (_rasterizerState    .Get())->GetRasterizerState()   : D3D12_RASTERIZER_DESC();
	desc.Flags                 = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.DSVFormat             = EnumConverter::Convert(_renderPass->GetDepthAttachment().has_value() ? _renderPass->GetDepthAttachment()->Format : core::PixelFormat::Unknown);
	desc.IBStripCutValue       = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	desc.NodeMask              = _device->GetGPUMask().Value();
	desc.NumRenderTargets      = static_cast<UINT>(_renderPass->GetColorAttachmentSize());
	desc.SampleDesc.Count      = static_cast<UINT>(_renderPass->GetMaxSample());
	desc.SampleDesc.Quality    = 0;
	desc.SampleMask            = UINT_MAX;

	for (int i = 0; i < _renderPass->GetColorAttachmentSize(); ++i)
	{
		desc.RTVFormats[i] = EnumConverter::Convert(_renderPass->GetColorAttachment(i)->Format);
	}
	
	/*-------------------------------------------------------------------
	-                      Create Graphic pipelineState
	---------------------------------------------------------------------*/
	ThrowIfFailed(dxDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(_graphicsPipeline.GetAddressOf())));
}
#pragma endregion Graphic PSO

#pragma region Compute PSO
void GPUComputePipelineState::CompleteSetting()
{
	const auto dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device)->GetDevice();
	const auto dxLayout = gu::StaticPointerCast<directX12::RHIResourceLayout>(_resourceLayout);
	const auto dxShader = gu::StaticPointerCast<directX12::GPUShaderState>(_computeShaderState);

	/*-------------------------------------------------------------------
	-                      Set Descriptor
	---------------------------------------------------------------------*/
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
	desc.CS             = _computeShaderState ? dxShader->GetShader() : D3D12_SHADER_BYTECODE();
	desc.Flags          = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.pRootSignature = _resourceLayout ? dxLayout->GetRootSignature().Get() : nullptr;
	desc.NodeMask       = 0;
	
	ThrowIfFailed(dxDevice->CreateComputePipelineState(&desc, IID_PPV_ARGS(&_computePipeline)));

}

void GPUComputePipelineState::SetName(const gu::tstring& name)
{
	_computePipeline->SetName(name.CString());
}
#pragma endregion Compute PSO
