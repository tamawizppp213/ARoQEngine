//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12BlendState.hpp
///  @brief  描画時に, どのように書き込む前のレンダーターゲットと書き込む先のレンダーターゲットの色を混合するかを定義するStateです@n
///          マルチレンダーターゲットにも対応可能です(最大数は8)
///  @author Toide Yutaro
///  @date   2024_05_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_BLEND_STATE_HPP
#define DIRECTX12_GPU_BLEND_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUBlendState.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			GPUBlendState
	*************************************************************************//**
	/*  @brief  描画時に, どのように書き込む前のレンダーターゲットと書き込む先のレンダーターゲットの色を混合するかを定義するStateです@n
	*           マルチレンダーターゲットにも対応可能です (最大数は8)
	*****************************************************************************/
	class GPUBlendState : public rhi::core::GPUBlendState
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		
		/*!**********************************************************************
		*  @brief  　　DirectX12のブレンドステート設定
		*************************************************************************/
		const D3D12_BLEND_DESC& GetDxBlendState() const { return _blendState; }

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		GPUBlendState() = default;
		
		/*! @brief デストラクタ*/
		~GPUBlendState() = default;
		
		/*!**********************************************************************
		*  @brief     個別に設定する方式のブレンドステート
		*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& 論理デバイス
		*  @param[in] const gu::DynamicArray<rhi::core::BlendProperty>& 個別のブレンド設定
		*  @param[in] const bool ピクセルシェーダーから出力されたα成分を取得し, マルチサンプリングアンチエイリアス処理を適用する機能を有効化
		*************************************************************************/
		explicit GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::DynamicArray<rhi::core::BlendProperty>& blendProperties, const bool alphaToCoverageEnable);
		
		/*!**********************************************************************
		*  @brief     全てのBlendStateで共通のプロパティを使用する方式
		*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& 論理デバイス
		*  @param[in] const rhi::core::BlendProperty 共通のブレンド設定
		*  @param[in] const bool ピクセルシェーダーから出力されたα成分を取得し, マルチサンプリングアンチエイリアス処理を適用する機能を有効化
		*************************************************************************/
		explicit GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const rhi::core::BlendProperty& blendProperty, const bool alphaToCoverageEnable);
		#pragma endregion
	
	protected:
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief DirectX12用のブレンドステート*/
		D3D12_BLEND_DESC _blendState = {};

		#pragma endregion
	};
}
#endif