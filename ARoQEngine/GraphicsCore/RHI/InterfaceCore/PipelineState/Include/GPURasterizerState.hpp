//////////////////////////////////////////////////////////////////////////////////
///  @file   GPURasterizerState.hpp
///  @brief  ピクセルを描画するラスタライザステートにおける設定項目を記述するクラスです. 
///  @author Toide Yutaro
///  @date   2024_05_03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RASTERIZER_STATE_HPP
#define GPU_RASTERIZER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GPUState.hpp"
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
	*				  			GPURasterizerState
	*************************************************************************//**
	/*  @brief  ピクセルを描画するラスタライザステートにおける設定項目を記述するクラスです. 
	*****************************************************************************/
	class GPURasterizerState : public GPUState
	{
	public:
		#pragma region Public Function
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief   ラスタライザの設定項目を全て返します.
		*************************************************************************/
		const RasterizerProperty& GetProperty() const { return _property; }

		/*!**********************************************************************
		*  @brief   前面が反時計回りか時計回りかを決定します.
		*************************************************************************/
		__forceinline FrontFace GetFrontFace  () const noexcept { return _property.FaceType; }

		/*!**********************************************************************
		*  @brief   指定された面(表面, 裏面)に対する三角形ポリゴンの描画可否を返します. 
		*  @return  None : 表裏両方描画, Front : 裏のみ描画, Back : 表のみ描画
		*************************************************************************/
		__forceinline CullingMode GetCullingMode() const noexcept { return _property.CullingType; }

		/*!**********************************************************************
		*  @brief   ポリゴンの三角形内部の描画設定方法を返します.
		*  @return  Solid : 三角形内部を描画, WireFrame : 線のみ描画, Point : 頂点だけ描画 [Vulkanのみ]
		*************************************************************************/
		__forceinline FillMode GetFillMode() const noexcept { return _property.FillType; }

		/*!**********************************************************************
		*  @brief   深度の大きさに応じてClamp処理(ピクセルの描画を行わない)を実行しているかを確認します.
		*************************************************************************/
		__forceinline bool UseDepthClamp() const noexcept { return _property.UseDepthClamp; }

		/*!**********************************************************************
		*  @brief   ラスタライズにおいて, マルチサンプリングが有効になっているかを確認します.
		*************************************************************************/
		__forceinline bool UseMultiSample() const { return _property.UseMultiSample; }

		/*!**********************************************************************
		*  @brief   DirectX12のみで設定可能です. 線のアンチエイリアスが有効化されているかを確認します.
		*  @return  trueで有効化設定がonとなっています. (ただし, DirectX12のみが正しい値を示します.)
		*************************************************************************/
		__forceinline bool UseAntiAliasLine() const { return _property.UseAntiAliasLine; }

		/*!**********************************************************************
		*  @brief   少しでもピクセルの描画範囲に入ったらラスタライズを実行する機能が有効化されているかを確認します
		*************************************************************************/
		__forceinline bool UseConservativeRaster() const { return _property.UseConservativeRaster; }

		/*!**********************************************************************
		*  @brief   与えられたピクセルに加えられるDepth値を返します. 
		*  @note    計算方法 : https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias?redirectedfrom=MSDN
		*************************************************************************/
		__forceinline float GetDepthBias() const { return _property.DepthBias; }

		/*!**********************************************************************
		*  @brief    MaxDepthSlopeに対する補正項の値を返します.
		*  @note     計算方法 : https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias?redirectedfrom=MSDN
		*************************************************************************/
		__forceinline float GetSlopeScaleDepthBias() const { return _property.SlopeScaleDepthBias; }

		/*!**********************************************************************
		*  @brief   最終的なBiasに対してClampされるBias値を返します.
		*  @note    計算方法 : https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias?redirectedfrom=MSDN
		*************************************************************************/
		__forceinline float GetClampMaxDepthBias() const { return _property.ClampMaxDepthBias; }
		#pragma endregion 

		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPURasterizerState() = default;

		/*! @brief デストラクタ*/
		virtual ~GPURasterizerState() = default;

		/*! @brief RasterizerPropertyを使って作成します. */
		explicit GPURasterizerState(
			const gu::SharedPointer<RHIDevice>& device,
			const RasterizerProperty& rasterizerProperty) 
			: GPUState(device), _property(rasterizerProperty) { };
	
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Property

		/*! @brief シェーダパイプラインにおける, ラスタライザステートに関する設定項目です.*/
		RasterizerProperty _property = RasterizerProperty();

		#pragma endregion

	};

}
#endif