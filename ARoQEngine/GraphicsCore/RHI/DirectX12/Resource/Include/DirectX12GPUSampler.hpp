//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUSampler.hpp
///  @brief  テクスチャサンプリングの方法の設定項目を記述するクラスです.
///  @author Toide Yutaro
///  @date   2024_07_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_SAMPLER_HPP
#define DIRECTX12_GPU_SAMPLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUSampler.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
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
	*				  			GPUSampler
	****************************************************************************/
	/* @brief  テクスチャサンプリングの方法の設定項目を記述するクラスです.
	*****************************************************************************/
	class GPUSampler : public core::GPUSampler
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     DirectX12のサンプラー情報を返します.
		*  @param[in] void
		*  @return    D3D12_STATIC_SAMPLER_DESC サンプラー情報
		*************************************************************************/
		__forceinline D3D12_STATIC_SAMPLER_DESC& GetSamplerDesc() { return _samplerDesc; }
		/*!**********************************************************************
	    *  @brief     DirectX12のサンプラー情報を返します.
	    *  @param[in] void
	    *  @return    const D3D12_STATIC_SAMPLER_DESC サンプラー情報
	    *************************************************************************/
		__forceinline const D3D12_STATIC_SAMPLER_DESC& GetSamplerDesc() const noexcept { return _samplerDesc; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief デフォルトコンストラクタ*/
		GPUSampler() = default;

		/*! @brief デストラクタ*/
		~GPUSampler() = default;

		/*! @brief 論理デバイスとサンプラー情報で初期化*/
		explicit GPUSampler(const gu::SharedPointer<core::RHIDevice>& device, const core::SamplerInfo& samplerInfo);

		#pragma endregion
	
	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief サンプラー情報*/
		D3D12_STATIC_SAMPLER_DESC _samplerDesc = {};
		#pragma endregion	
	};
}
#endif