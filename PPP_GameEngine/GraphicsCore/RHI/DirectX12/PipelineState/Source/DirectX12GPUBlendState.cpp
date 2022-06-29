//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12BlendState.cpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUBlendState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace gpu::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUBlendState::GPUBlendState(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::vector<rhi::core::BlendProperty>& blendProperties)
: rhi::core::GPUBlendState(device, blendProperties)
{
	_blendState.AlphaToCoverageEnable  = false;
	_blendState.IndependentBlendEnable = _isIndependentBlendEnable;

	for (size_t index = 0; index < _blendProperties.size(); index++)
	{
		const auto& blendProperty = _blendProperties[index];

		_blendState.RenderTarget[index].BlendEnable    = blendProperty.Enable;
		_blendState.RenderTarget[index].LogicOpEnable  = false;
		_blendState.RenderTarget[index].SrcBlend       = EnumConverter::Convert(blendProperty.Source);
		_blendState.RenderTarget[index].DestBlend      = EnumConverter::Convert(blendProperty.Destination);
		_blendState.RenderTarget[index].BlendOp        = EnumConverter::Convert(blendProperty.ColorOperator);
		_blendState.RenderTarget[index].SrcBlendAlpha  = EnumConverter::Convert(blendProperty.SourceAlpha);
		_blendState.RenderTarget[index].DestBlendAlpha = EnumConverter::Convert(blendProperty.DestinationAlpha);
		_blendState.RenderTarget[index].BlendOpAlpha   = EnumConverter::Convert(blendProperty.AlphaOperator);
		_blendState.RenderTarget[index].LogicOp        = D3D12_LOGIC_OP_NOOP;
		_blendState.RenderTarget[index].RenderTargetWriteMask = EnumConverter::Convert(blendProperty.ColorMask);
	}
}
GPUBlendState::GPUBlendState(const std::shared_ptr<rhi::core::RHIDevice>& device, const rhi::core::BlendProperty& blendProperty)
	: rhi::core::GPUBlendState(device, blendProperty)
{
	_blendState.AlphaToCoverageEnable = true;
	_blendState.IndependentBlendEnable = _isIndependentBlendEnable;

	for (size_t index = 0; index < _blendProperties.size(); index++)
	{
		const auto& blendProperty = _blendProperties[index];

		_blendState.RenderTarget[index].BlendEnable    = blendProperty.Enable;
		_blendState.RenderTarget[index].LogicOpEnable  = false;
		_blendState.RenderTarget[index].SrcBlend       = EnumConverter::Convert(blendProperty.Source);
		_blendState.RenderTarget[index].DestBlend      = EnumConverter::Convert(blendProperty.Destination);
		_blendState.RenderTarget[index].BlendOp        = EnumConverter::Convert(blendProperty.ColorOperator);
		_blendState.RenderTarget[index].SrcBlendAlpha  = EnumConverter::Convert(blendProperty.SourceAlpha);
		_blendState.RenderTarget[index].DestBlendAlpha = EnumConverter::Convert(blendProperty.DestinationAlpha);
		_blendState.RenderTarget[index].BlendOpAlpha   = EnumConverter::Convert(blendProperty.AlphaOperator);
		_blendState.RenderTarget[index].LogicOp        = D3D12_LOGIC_OP_NOOP;
		_blendState.RenderTarget[index].RenderTargetWriteMask = EnumConverter::Convert(blendProperty.ColorMask);
	}
}
