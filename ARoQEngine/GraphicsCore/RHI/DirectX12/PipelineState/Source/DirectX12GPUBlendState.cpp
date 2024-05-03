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
#include "GameUtility/Base/Include/GUAssert.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
/*!**********************************************************************
*  @brief     個別に設定する方式のブレンドステート
*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& 論理デバイス
*  @param[in] const gu::DynamicArray<rhi::core::BlendProperty>& 個別のブレンド設定
*  @param[in] const bool ピクセルシェーダーから出力されたα成分を取得し, マルチサンプリングアンチエイリアス処理を適用する機能を有効化
*************************************************************************/
GPUBlendState::GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::DynamicArray<rhi::core::BlendProperty>& blendProperties, const bool alphaToCoverageEnable)
: rhi::core::GPUBlendState(device, blendProperties, alphaToCoverageEnable)
{
	/*-------------------------------------------------------------------
	-        無効入力チェック
	---------------------------------------------------------------------*/
	Checkf(device, "Device is nullptr");
	Checkf(blendProperties.Size() > 0, "Blend properties are empty.");
	Checkf(blendProperties.Size() <= _countof(_blendState.RenderTarget), "BlendProperty exceeds the max render target");

	/*-------------------------------------------------------------------
	-        Blendingの設定
	---------------------------------------------------------------------*/
	// Explain https://bgolus.medium.com/anti-aliased-alpha-test-the-esoteric-alpha-to-coverage-8b177335ae4f
	_blendState.AlphaToCoverageEnable  = _alphaToConverageEnable;
	_blendState.IndependentBlendEnable = _isIndependentBlendEnable;

	for (uint64 index = 0; index < _blendProperties.Size(); index++)
	{
		const auto& blendProperty = _blendProperties[index];

		_blendState.RenderTarget[index].BlendEnable    = blendProperty.EnableBlend;
		_blendState.RenderTarget[index].LogicOpEnable  = false;
		_blendState.RenderTarget[index].SrcBlend       = EnumConverter::Convert(blendProperty.SourceRGB);
		_blendState.RenderTarget[index].DestBlend      = EnumConverter::Convert(blendProperty.DestinationRGB);
		_blendState.RenderTarget[index].BlendOp        = EnumConverter::Convert(blendProperty.ColorOperator);
		_blendState.RenderTarget[index].SrcBlendAlpha  = EnumConverter::Convert(blendProperty.SourceAlpha);
		_blendState.RenderTarget[index].DestBlendAlpha = EnumConverter::Convert(blendProperty.DestinationAlpha);
		_blendState.RenderTarget[index].BlendOpAlpha   = EnumConverter::Convert(blendProperty.AlphaOperator);
		_blendState.RenderTarget[index].LogicOp        = D3D12_LOGIC_OP_NOOP;
		_blendState.RenderTarget[index].RenderTargetWriteMask = static_cast<UINT8>(EnumConverter::Convert(blendProperty.ColorMask));
	}
}

/*!**********************************************************************
*  @brief     全てのBlendStateで共通のプロパティを使用する方式
*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& 論理デバイス
*  @param[in] const rhi::core::BlendProperty 共通のブレンド設定
*  @param[in] const bool ピクセルシェーダーから出力されたα成分を取得し, マルチサンプリングアンチエイリアス処理を適用する機能を有効化
*************************************************************************/
GPUBlendState::GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const rhi::core::BlendProperty& blendProperty, const bool alphaToCoverageEnable)
	: rhi::core::GPUBlendState(device, blendProperty, alphaToCoverageEnable)
{
	/*-------------------------------------------------------------------
	-        無効入力チェック
	---------------------------------------------------------------------*/
	Checkf(device, "Device is nullptr");

	/*-------------------------------------------------------------------
	-        Blendingの設定
	---------------------------------------------------------------------*/
	_blendState.AlphaToCoverageEnable  = _alphaToConverageEnable;
	_blendState.IndependentBlendEnable = _isIndependentBlendEnable;

	for (uint64 index = 0; index < _blendProperties.Size(); index++)
	{
		const auto& prop = _blendProperties[index];

		_blendState.RenderTarget[index].BlendEnable    = prop.EnableBlend;
		_blendState.RenderTarget[index].LogicOpEnable  = false;
		_blendState.RenderTarget[index].SrcBlend       = EnumConverter::Convert(prop.SourceRGB);
		_blendState.RenderTarget[index].DestBlend      = EnumConverter::Convert(prop.DestinationRGB);
		_blendState.RenderTarget[index].BlendOp        = EnumConverter::Convert(prop.ColorOperator);
		_blendState.RenderTarget[index].SrcBlendAlpha  = EnumConverter::Convert(prop.SourceAlpha);
		_blendState.RenderTarget[index].DestBlendAlpha = EnumConverter::Convert(prop.DestinationAlpha);
		_blendState.RenderTarget[index].BlendOpAlpha   = EnumConverter::Convert(prop.AlphaOperator);
		_blendState.RenderTarget[index].LogicOp        = D3D12_LOGIC_OP_NOOP;
		_blendState.RenderTarget[index].RenderTargetWriteMask = static_cast<UINT8>(EnumConverter::Convert(blendProperty.ColorMask));
	}
}
