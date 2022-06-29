//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12RasterizerState.cpp
///             @brief  Rasterizer State
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPURasterizerState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPURasterizerState::GPURasterizerState(const std::shared_ptr<rhi::core::RHIDevice>& device, const core::FrontFace   frontFace, const core::CullingMode cullingMode, const core::FillMode fillMode, bool useDepthClamp)
	: rhi::core::GPURasterizerState(device, frontFace, cullingMode, fillMode, useDepthClamp)
{
	_rasterizerState.FillMode              = EnumConverter::Convert(_fillMode);
	_rasterizerState.CullMode              = EnumConverter::Convert(_cullingMode);
	_rasterizerState.FrontCounterClockwise = EnumConverter::Convert(_frontFace);
	_rasterizerState.DepthBias             = D3D12_DEFAULT_DEPTH_BIAS;
	_rasterizerState.DepthBiasClamp        = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	_rasterizerState.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	_rasterizerState.DepthClipEnable       = _useDepthClamp;
	_rasterizerState.MultisampleEnable     = FALSE;
	_rasterizerState.AntialiasedLineEnable = FALSE;
	_rasterizerState.ForcedSampleCount     = 0;
	_rasterizerState.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}
