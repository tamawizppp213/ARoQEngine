//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12BlendState.cpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUBlendState.hpp"
#include "../../Core/Include/DirectX12EnumConverter.hpp"
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUBlendState::GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const std::vector<rhi::core::BlendProperty>& blendProperties)
: rhi::core::GPUBlendState(device, blendProperties)
{
#ifdef _DEBUG
	assert(device);
	assert(blendProperties.size() > 0);
	assert(blendProperties.size() <= _countof(_blendState.RenderTarget));
#endif
	// Explain https://bgolus.medium.com/anti-aliased-alpha-test-the-esoteric-alpha-to-coverage-8b177335ae4f
	_blendState.AlphaToCoverageEnable  = blendProperties[0].AlphaToConverageEnable;
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
		_blendState.RenderTarget[index].RenderTargetWriteMask = static_cast<UINT8>(EnumConverter::Convert(blendProperty.ColorMask));
	}
}

GPUBlendState::GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const rhi::core::BlendProperty& blendProperty)
	: rhi::core::GPUBlendState(device, blendProperty)
{
#ifdef _DEBUG
	assert(device);
#endif

	_blendState.AlphaToCoverageEnable  = blendProperty.AlphaToConverageEnable;
	_blendState.IndependentBlendEnable = _isIndependentBlendEnable;

	for (size_t index = 0; index < _blendProperties.size(); index++)
	{
		const auto& prop = _blendProperties[index];

		_blendState.RenderTarget[index].BlendEnable    = prop.Enable;
		_blendState.RenderTarget[index].LogicOpEnable  = false;
		_blendState.RenderTarget[index].SrcBlend       = EnumConverter::Convert(prop.Source);
		_blendState.RenderTarget[index].DestBlend      = EnumConverter::Convert(prop.Destination);
		_blendState.RenderTarget[index].BlendOp        = EnumConverter::Convert(prop.ColorOperator);
		_blendState.RenderTarget[index].SrcBlendAlpha  = EnumConverter::Convert(prop.SourceAlpha);
		_blendState.RenderTarget[index].DestBlendAlpha = EnumConverter::Convert(prop.DestinationAlpha);
		_blendState.RenderTarget[index].BlendOpAlpha   = EnumConverter::Convert(prop.AlphaOperator);
		_blendState.RenderTarget[index].LogicOp        = D3D12_LOGIC_OP_NOOP;
		_blendState.RenderTarget[index].RenderTargetWriteMask = static_cast<UINT8>(EnumConverter::Convert(blendProperty.ColorMask));
	}
}
