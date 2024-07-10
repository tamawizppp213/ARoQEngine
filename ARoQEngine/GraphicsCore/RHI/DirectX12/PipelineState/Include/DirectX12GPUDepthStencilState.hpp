//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUDepthStencilState.hpp
///  @brief  ピクセルに対して深度テストやステンシルテストを行うための設定項目を記述するクラスです.
///  @author Toide Yutaro
///  @date   2024_07_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_DEPTH_STENCIL_STATE_HPP
#define DIRECTX12_GPU_DEPTH_STENCIL_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUDepthStencilState.hpp"
#define COM_NO_WINDOWS_H
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
	*				  			GPUDepthStencilState
	****************************************************************************/
	/* @brief  ピクセルに対して深度テストやステンシルテストを行うための設定項目を記述するクラスです.
	*****************************************************************************/
	class GPUDepthStencilState : public rhi::core::GPUDepthStencilState
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     DirectX12専用の設定項目を返します
		*  @param[in] void
		*  @return    const D3D12_DEPTH_STENCIL_DESC& : DirectX12専用の設定項目
		*************************************************************************/
		const D3D12_DEPTH_STENCIL_DESC& GetDepthStencilState() const noexcept { return _depthStencilDesc.Desc0; }
		
		/*!**********************************************************************
		*  @brief     DirectX12専用の設定項目を返します
		*  @param[in] void
		*  @return    const D3D12_DEPTH_STENCIL_DESC& : DirectX12専用の設定項目
		*************************************************************************/
		const D3D12_DEPTH_STENCIL_DESC1& GetDepthStencilState1() const noexcept { return _depthStencilDesc.Desc1; }
		
		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief ラスタライザの基本設定を用いて作成するコンストラクタ*/
		explicit GPUDepthStencilState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::DepthStencilProperty& depthStencilProperty
		);

		/*! @brief デストラクタ*/
		~GPUDepthStencilState() = default;
		
		/*! @brief デフォルトコンストラクタ*/
		GPUDepthStencilState() = default;
		#pragma endregion
	
	protected:
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! DirectX12のDepthStencil設定*/
		union DepthStencilDesc
		{
			D3D12_DEPTH_STENCIL_DESC  Desc0;
			D3D12_DEPTH_STENCIL_DESC1 Desc1;
		} _depthStencilDesc;
		#pragma endregion
	};
}
#endif