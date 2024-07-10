//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUDepthStencilState.hpp
///  @brief  ピクセルに対して深度テストやステンシルテストを行うための設定項目を記述するクラスです.
///  @author Toide Yutaro
///  @date   2024_07_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_DEPTH_STENCIL_STATE_HPP
#define GPU_DEPTH_STENCIL_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/RHICommonState.hpp"
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
	*				  			GPUDepthStencilState
	****************************************************************************/
	/* @brief   ピクセルに対して深度テストやステンシルテストを行うための設定項目を記述するクラスです.
	*****************************************************************************/
	class GPUDepthStencilState : public GPUState
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     深度テストを行うかどうかを返します. ピクセルを描画するかを決定するためのテストです.
		*  @param[in] void
		*  @return    bool : trueで深度テストを行う.
		*************************************************************************/
		__forceinline bool UseDepthTest() const noexcept { return _property.DepthOperator != rhi::core::CompareOperator::Always || _property.DepthWriteEnable; }

		/*!**********************************************************************
		*  @brief     描画マスクを行うステンシルテストを行うかどうかを返します. trueにする場合は必ずStencilに対応するピクセルフォーマットにすること
		*  @param[in] void
		*  @return    bool : trueでステンシルテストを行う. 
		*************************************************************************/
		__forceinline bool UseStencilTest() const noexcept { return _property.StencilWriteEnable; }

		/*!**********************************************************************
		*  @brief     DepthBoundsTestを行うかを返します. 行う場合は必ずMinDepthBounds, MaxDepthBoundsを設定してください.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool UseDepthBoundsTest() const noexcept { return _property.UseDepthBoundsTest; }

		/*!**********************************************************************
		*  @brief     DepthBufferに書き込みを行うかを返します.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool GetDepthWriteEnable() const noexcept { return _property.DepthWriteEnable; }

		/*!**********************************************************************
		*  @brief     DepthTestを実行する際の比較演算子を返します. LessEqualとかであれば今回書き込むピクセルの深度値が以前の値以下である場合に描画します.
		*  @param[in] void
		*  @return    const CompareOperator
		*************************************************************************/
		__forceinline CompareOperator GetDepthOperator() const noexcept { return _property.DepthOperator; }
		
		/*!**********************************************************************
		*  @brief     FrontFaceのステンシルテストの設定を返します.
		*  @param[in] void
		*  @return    const StencilOperatorInfo
		*************************************************************************/
		__forceinline const StencilOperatorInfo& GetFrontFaceStencil() const noexcept { return _property.Front; }
		
		/*!**********************************************************************
		*  @brief     BackFaceのステンシルテストの設定を返します.
		*  @param[in] void
		*  @return    const StencilOperatorInfo
		*************************************************************************/
		__forceinline const StencilOperatorInfo& GetBackFaceStencil () const noexcept { return _property.Back; }

		/*!**********************************************************************
		*  @brief     DepthBoundsTestの最小デプス値を返します. 
		*  @param[in] void
		*  @return    float
		*************************************************************************/
		__forceinline float GetMinDepthBounds() const noexcept { return _property.MinDepthBounds; }

		/*!**********************************************************************
		*  @brief     DepthBoundsTestの最大デプス値を返します.
		*  @param[in] void
		*  @return    float
		*************************************************************************/
		__forceinline float GetMaxDepthBounds() const noexcept { return _property.MaxDepthBounds; }

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		GPUDepthStencilState() = default;

		/*! @brief デストラクタ*/
		virtual ~GPUDepthStencilState() = default;

		/*! @brief DepthStencilPropertyを使って作成します. */
		explicit GPUDepthStencilState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const DepthStencilProperty& depthStencilProperty
		) : GPUState(device), _property(depthStencilProperty){ };

		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Property
		/*! @brief デプスステンシルの設定項目 */
		DepthStencilProperty _property = DepthStencilProperty();
		#pragma endregion
	};
}
#endif