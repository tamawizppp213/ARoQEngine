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
/*!**********************************************************************
*  @brief     ComputePipelineが作成できたタイミングで動作させます. Descriptorを作成します.
*  @param[in] const gu::tstring デバッグ表示名
*  @return    void
*************************************************************************/
void GPUGraphicsPipelineState::CompleteSetting(const gu::tstring& name)
{
	DeviceComPtr dxDevice = nullptr;
	dxDevice = static_cast<RHIDevice*>(_device.Get())->GetDevice();

	/*-------------------------------------------------------------------
	-                      Set Descriptor
	---------------------------------------------------------------------*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.PrimitiveTopologyType = EnumConverter::Convert1(_inputAssemblyState->GetPrimitiveTopology());
	desc.pRootSignature        = _resourceLayout      ? static_cast<RHIResourceLayout*>(_resourceLayout.Get())->GetRootSignature().Get() : nullptr;
	desc.InputLayout           = _inputAssemblyState  ? static_cast<GPUInputAssemblyState*>(_inputAssemblyState.Get())->GetDxLayoutDesc()      : D3D12_INPUT_LAYOUT_DESC();
	desc.VS                    = _vertexShaderState   ? static_cast<GPUShaderState*>      (_vertexShaderState  .Get())->GetDxShader()          : D3D12_SHADER_BYTECODE();
	desc.PS                    = _pixelShaderState    ? static_cast<GPUShaderState*>      (_pixelShaderState   .Get())->GetDxShader()          : D3D12_SHADER_BYTECODE();
	desc.HS                    = _hullShaderState     ? static_cast<GPUShaderState*>      (_hullShaderState    .Get())->GetDxShader()          : D3D12_SHADER_BYTECODE();
	desc.DS                    = _domainShaderState   ? static_cast<GPUShaderState*>      (_domainShaderState  .Get())->GetDxShader()          : D3D12_SHADER_BYTECODE();
	desc.GS                    = _geometryShaderState ? static_cast<GPUShaderState*>      (_geometryShaderState.Get())->GetDxShader()          : D3D12_SHADER_BYTECODE();
	desc.DepthStencilState     = _depthStencilState   ? static_cast<GPUDepthStencilState*>(_depthStencilState  .Get())->GetDepthStencilState() : D3D12_DEPTH_STENCIL_DESC();
	desc.BlendState            = _blendState          ? static_cast<GPUBlendState*>       (_blendState         .Get())->GetDxBlendState()      : D3D12_BLEND_DESC();
	desc.RasterizerState       = _rasterizerState     ? static_cast<GPURasterizerState*>  (_rasterizerState    .Get())->GetDxRasterizerDesc()  : D3D12_RASTERIZER_DESC();
	desc.Flags                 = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.DSVFormat             = _renderPass->GetDepthAttachment().HasValue() ?
		                         (DXGI_FORMAT)core::PixelFormatInfo::GetConst(_renderPass->GetDepthAttachment()->Format).PlatformFormat : 
		                         (DXGI_FORMAT)core::PixelFormatInfo::GetConst(core::PixelFormat::Unknown).PlatformFormat;
	desc.IBStripCutValue       = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	desc.NodeMask              = _device->GetGPUMask().Value();
	desc.NumRenderTargets      = static_cast<gu::uint32>(_renderPass->GetColorAttachmentSize());
	desc.SampleDesc.Count      = static_cast<gu::uint32>(_renderPass->GetMaxSample());
	desc.SampleDesc.Quality    = 0;
	desc.SampleMask            = UINT_MAX;

	if (_cachePSO)
	{
		desc.CachedPSO = D3D12_CACHED_PIPELINE_STATE{ _cachePSO->GetBufferPointer(), _cachePSO->GetBufferSize() };
	}

	for (gu::uint32 i = 0; i < _renderPass->GetColorAttachmentSize(); ++i)
	{
		const auto format = _renderPass->GetColorAttachment(i)->Format;
		desc.RTVFormats[i] = (DXGI_FORMAT)core::PixelFormatInfo::GetConst(format).PlatformFormat;
	}
	
	/*-------------------------------------------------------------------
	-                      Create Graphic pipelineState
	---------------------------------------------------------------------*/
	ThrowIfFailed(dxDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(_graphicsPipeline.GetAddressOf())));
	_graphicsPipeline->GetCachedBlob(_cachePSO.GetAddressOf());
	SetName(name);
}
#pragma endregion Graphic PSO

#pragma region Compute PSO
/*!**********************************************************************
*  @brief     ComputePipelineが作成できたタイミングで動作させます. Descriptorを作成します.
*  @param[in] const gu::tstring デバッグ表示名
*  @return    void
*************************************************************************/
void GPUComputePipelineState::CompleteSetting(const gu::tstring& name)
{
	const auto dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device)->GetDevice();
	const auto dxLayout = gu::StaticPointerCast<directX12::RHIResourceLayout>(_resourceLayout);
	const auto dxShader = gu::StaticPointerCast<directX12::GPUShaderState>(_computeShaderState);

	/*-------------------------------------------------------------------
	-                      Set Descriptor
	---------------------------------------------------------------------*/
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
	desc.CS             = dxShader->GetDxShader();
	desc.Flags          = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.pRootSignature = _resourceLayout ? dxLayout->GetRootSignature().Get() : nullptr;
	desc.NodeMask       = _device->GetGPUMask().Value();
	
	if (_cachePSO)
	{
		desc.CachedPSO = D3D12_CACHED_PIPELINE_STATE{ _cachePSO->GetBufferPointer(), _cachePSO->GetBufferSize() };
	}
	
	ThrowIfFailed(dxDevice->CreateComputePipelineState(&desc, IID_PPV_ARGS(&_computePipeline)));
	_computePipeline->GetCachedBlob(_cachePSO.GetAddressOf());
	SetName(name);
}

/*!**********************************************************************
*  @brief     デバッグ時に使用する名前
*  @param[in] const gu::tstring& name
*  @return    void
*************************************************************************/
void GPUComputePipelineState::SetName(const gu::tstring& name)
{
	_computePipeline->SetName(name.CString());
}
#pragma endregion Compute PSO
