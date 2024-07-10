//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPURasterizerState.cpp
///  @brief  �s�N�Z����`�悷�郉�X�^���C�U�X�e�[�g�ɂ�����ݒ荀�ڂ��L�q����N���X�ł�. 
///  @author Toide Yutaro
///  @date   2024_05_05
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
GPURasterizerState::GPURasterizerState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const rhi::core::RasterizerProperty& rasterizerProperty)
	: rhi::core::GPURasterizerState(device, rasterizerProperty)
{
	_rasterizerState.FillMode              = EnumConverter::Convert(_property.FillType); 
	_rasterizerState.CullMode              = EnumConverter::Convert(_property.CullingType); 
	_rasterizerState.FrontCounterClockwise = EnumConverter::Convert(_property.FaceType);
	_rasterizerState.DepthBias             = static_cast<gu::int32>(_property.DepthBias);
	_rasterizerState.DepthBiasClamp        = _property.ClampMaxDepthBias; 
	_rasterizerState.SlopeScaledDepthBias  = _property.SlopeScaleDepthBias; 
	_rasterizerState.DepthClipEnable       = _property.UseDepthClamp; 
	_rasterizerState.MultisampleEnable     = _property.UseMultiSample;                                      
	_rasterizerState.AntialiasedLineEnable = (!_property.UseMultiSample && _property.FillType == core::FillMode::WireFrame) && _property.UseAntiAliasLine; 
	_rasterizerState.ForcedSampleCount     = 0;
	_rasterizerState.ConservativeRaster = _property.UseConservativeRaster ? D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}
