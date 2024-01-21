//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12RasterizerState.cpp
///             @brief  Rasterizer State
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPURasterizerState.hpp"
#include "../../Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPURasterizerState::GPURasterizerState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const rhi::core::RasterizerProperty& rasterizerProperty)
	: rhi::core::GPURasterizerState(device, rasterizerProperty)
{
	_rasterizerState.FillMode              = EnumConverter::Convert(_property.FillType);     // polygon filling mode
	_rasterizerState.CullMode              = EnumConverter::Convert(_property.CullingType);  // polygon culling mode
	_rasterizerState.FrontCounterClockwise = EnumConverter::Convert(_property.FaceType);     // polygon front face
	_rasterizerState.DepthBias             = static_cast<INT>(_property.DepthBias);                            // Depth value added to a given pixel. https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias
	_rasterizerState.DepthBiasClamp        = _property.ClampMaxDepthBias;                    // 0: Non clamp, +: min(DepthBiasClamp, Bias), -: max(DepthBiasClamp, Bias) 
	_rasterizerState.SlopeScaledDepthBias  = _property.SlopeScaleDepthBias;                  // Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
	_rasterizerState.DepthClipEnable       = _property.UseDepthClamp;                        // Specifies whether to enable clipping based on distance.  
	_rasterizerState.MultisampleEnable     = _property.UseMultiSample;                                      
	_rasterizerState.AntialiasedLineEnable = (!_property.UseMultiSample && _property.FillType == core::FillMode::WireFrame) && _property.UseAntiAliasLine; 
	_rasterizerState.ForcedSampleCount     = 0;
	_rasterizerState.ConservativeRaster = _property.UseConservativeRaster ? D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

}
