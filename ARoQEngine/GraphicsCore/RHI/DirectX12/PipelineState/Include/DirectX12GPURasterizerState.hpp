//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPURasterizerState.hpp
///  @brief  ピクセルを描画するラスタライザステートにおける設定項目を記述するクラスです. 
///  @author Toide Yutaro
///  @date   2024_05_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_RASTERIZER_STATE_HPP
#define DIRECTX12_GPU_RASTERIZER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPURasterizerState.hpp"
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
	*				  			GPURasterizerState
	*************************************************************************//**
	/*  @brief     ピクセルを描画するラスタライザステートにおける設定項目を記述するクラスです. 
	*****************************************************************************/
	class GPURasterizerState : public rhi::core::GPURasterizerState
	{
	public:
		#pragma region Public Function
		#pragma endregion 

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief   DirectX12専用の設定項目を返します
		*************************************************************************/
		const D3D12_RASTERIZER_DESC& GetDxRasterizerDesc() const noexcept { return _rasterizerState; }
		
		#pragma endregion 
		
		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPURasterizerState() = default;
		
		/*! @brief デストラクタ*/
		~GPURasterizerState() = default;
		
		/*! @brief ラスタライザの基本設定を用いて作成するコンストラクタ*/
		explicit GPURasterizerState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const rhi::core::RasterizerProperty& rasterizerProperty);

		#pragma endregion 

	protected:
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Property

		/*! @brief DirectX12専用の設定項目 */
		D3D12_RASTERIZER_DESC _rasterizerState = {};

		#pragma endregion
	};
}
#endif