//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUBlendState.hpp
///  @brief  描画時に, どのように書き込む前のレンダーターゲットと書き込む先のレンダーターゲットの色を混合するかを定義するStateです@n
///          マルチレンダーターゲットにも対応可能です (最大数は8)
///  @author Toide Yutaro
///  @date   2024_05_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_BLEND_STATE_HPP
#define GPU_BLEND_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GPUState.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;

	/****************************************************************************
	*				  			GPUBlendState
	*************************************************************************//**
	/* @brief  描画時に, どのように書き込む前のレンダーターゲットと書き込む先のレンダーターゲットの色を混合するかを定義するStateです@n
	*          マルチレンダーターゲットにも対応可能です (最大数は8)
	*****************************************************************************/
	class GPUBlendState : public GPUState
	{
	public:
		#pragma region Public Function
		#pragma endregion
		
		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief  　　描画時の基本設定を返します.
		*  @param[in] const gu::uint32 配列のインデックス (基本は0で, 最大は8でお願いします.)
		*************************************************************************/
		const core::BlendProperty& GetProperty(const gu::uint32 index = 0) { return _blendProperties[index]; }

		/*!**********************************************************************
		*  @brief  　　BlendPropertyの要素数を取得します
		*************************************************************************/
		const gu::uint64 Size() const { return _blendProperties.Size(); }

		/*!**********************************************************************
		*  @brief  　　ブレンディングを有効化している場合trueになります
		*  @return    個別設定の場合はindexの値が, 共通設定の場合は0の値のBlendPropertyのブレンディング有効化設定が確認できます.
		*************************************************************************/
		__forceinline bool EnableBlend(const gu::uint32 index) const 
		{
			Check(index < _blendProperties.Size());
			return _isIndependentBlendEnable ? _blendProperties[index].EnableBlend :_blendProperties[0].EnableBlend;
		}

		/*!**********************************************************************
		*  @brief  　　ブレンディングを有効化している場合trueになります
		*  @return    個別設定の場合はindexの値が, 共通設定の場合は0の値のBlendPropertyのブレンディング有効化設定が確認できます.
		*************************************************************************/
		__forceinline ColorMask GetColorMask(const gu::uint32 index) const
		{
			Check(index < _blendProperties.Size());
			return _isIndependentBlendEnable ? _blendProperties[index].ColorMask : _blendProperties[0].ColorMask;
		}

		/*!**********************************************************************
		*  @brief  　　ピクセルシェーダーから出力されたα成分を取得し, マルチサンプリングアンチエイリアス処理を適用する機能が有効化されているか
		*  @return    trueで有効化済み
		*************************************************************************/
		__forceinline bool UseAlphaToCoverage() const { return _alphaToConverageEnable; }

		/*!**********************************************************************
		*  @brief  　　0～7までのレンダーターゲットに対して個別にブレンド設定を行うか
		*  @return    trueで個別にブレンド設定を行い, falseでproperty[0]の設定を共通のものとして定義します.
		*************************************************************************/
		__forceinline bool IsIndependentBlendEnable() const { return _isIndependentBlendEnable; }

		#pragma endregion

		#pragma	region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		GPUBlendState() = default;

		/*! @brief デストラクタ*/
		virtual ~GPUBlendState();

		/*!**********************************************************************
		*  @brief     個別に設定する方式のブレンドステート
		*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& 論理デバイス
		*  @param[in] const gu::DynamicArray<rhi::core::BlendProperty>& 個別のブレンド設定
		*  @param[in] const bool ピクセルシェーダーから出力されたα成分を取得し, マルチサンプリングアンチエイリアス処理を適用する機能を有効化
		*************************************************************************/
		explicit GPUBlendState(
			const gu::SharedPointer<RHIDevice>& device, 
			const gu::DynamicArray<BlendProperty>& properties,
			const bool alphaToCoverageEnable) 
			: GPUState(device), _blendProperties(properties), _isIndependentBlendEnable(true), _alphaToConverageEnable(alphaToCoverageEnable) 
		{

		};
		
		/*!**********************************************************************
		*  @brief     全てのBlendStateで共通のプロパティを使用する方式
		*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& 論理デバイス
		*  @param[in] const rhi::core::BlendProperty 共通のブレンド設定
		*  @param[in] const bool ピクセルシェーダーから出力されたα成分を取得し, マルチサンプリングアンチエイリアス処理を適用する機能を有効化
		*************************************************************************/
		explicit GPUBlendState(const gu::SharedPointer<RHIDevice>& device, const BlendProperty& blendProperty, const bool alphaToConverageEnable) : GPUState(device),
			_isIndependentBlendEnable(false),
			_alphaToConverageEnable(alphaToConverageEnable)
		{
			_blendProperties.Push(blendProperty);
		}

		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Member Variables
		/* @brief : 描画時の設定です. Source : これからレンダリングする色 (ピクセルシェーダー), Destination : レンダリング先 (レンダーターゲット)*/
		gu::DynamicArray<core::BlendProperty> _blendProperties;
		
		/*! @brief ピクセルシェーダーから出力されたα成分を取得し, マルチサンプリングアンチエイリアス処理を適用する機能を有効化*/
		bool _alphaToConverageEnable = false;

		/*! @brief 0～7までのレンダーターゲットに対して個別にブレンド設定を行うか*/
		bool _isIndependentBlendEnable = false;
		#pragma endregion
	};

}
#endif