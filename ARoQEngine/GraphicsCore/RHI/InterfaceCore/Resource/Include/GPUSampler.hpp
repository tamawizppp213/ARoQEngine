//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUSampler.hpp
///  @brief  テクスチャサンプリングの方法の設定項目を記述するクラスです.
///  @author Toide Yutaro
///  @date   2024_07_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_SAMPLER_HPP
#define GPU_SAMPLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
	*				  			GPUSampler
	****************************************************************************/
	/* @brief   テクスチャサンプリングの方法の設定項目を記述するクラスです.
	*****************************************************************************/
	class GPUSampler : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     テクスチャサンプリングの設定項目
		*  @param[in] void
		*  @return    const SamplerInfo& サンプラー情報
		*************************************************************************/
		const SamplerInfo& GetSamplerInfo  () const noexcept { return _samplerInfo; }
		
		/*!**********************************************************************
		*  @brief     フィルタリングの種別
		*  @param[in] void
		*  @return    FilterOption フィルタリングの種別
		*************************************************************************/
		FilterOption  GetFilterOption () const noexcept { return _samplerInfo.Filter; }
		
		/*!**********************************************************************
		*  @brief     U方向のテクスチャアドレッシングモードを返します.
		*  @param[in] void
		*  @return    SamplerAddressMode U方向のテクスチャアドレッシングモード
		*************************************************************************/
		SamplerAddressMode GetAddressModeU () const noexcept { return _samplerInfo.AddressModeU; }
		
		/*!**********************************************************************
		*  @brief     V方向のテクスチャアドレッシングモードを返します.
		*  @param[in] void
		*  @return    SamplerAddressMode V方向のテクスチャアドレッシングモード
		*************************************************************************/
		SamplerAddressMode GetAddressModeV () const noexcept { return _samplerInfo.AddressModeV; }
		
		/*!**********************************************************************
		*  @brief     W方向のテクスチャアドレッシングモードを返します.	
		*  @param[in] void
		*  @return    SamplerAddressMode W方向のテクスチャアドレッシングモード
		*************************************************************************/
		SamplerAddressMode GetAddressModeW () const noexcept { return _samplerInfo.AddressModeW; }
		
		/*!**********************************************************************
		*  @brief     1～16までの値で使用されるクランプ値
		*  @param[in] void
		*  @return    gu::uint8 クランプ値
		*************************************************************************/
		gu::uint8   GetMaxAnisotropy() const noexcept { return _samplerInfo.MaxAnisotropy; }
		
		/*!**********************************************************************
		*  @brief     テクスチャの境界色を返します.
		*  @param[in] void
		*  @return    BorderColor ボーダーカラー
		*************************************************************************/
		BorderColor GetBorderColor() const noexcept { return _samplerInfo.Border; }

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPUSampler() = default;

		/*! @brief デストラクタ*/
		virtual ~GPUSampler() = default;

		/*! @brief 論理デバイスとサンプラー情報で初期化*/
		explicit GPUSampler(const gu::SharedPointer<RHIDevice>& device, const SamplerInfo& samplerInfo) : _device(device), _samplerInfo(samplerInfo) {};

		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*! @brief サンプラー情報*/
		SamplerInfo _samplerInfo = {};
		#pragma endregion
	};
}


#endif