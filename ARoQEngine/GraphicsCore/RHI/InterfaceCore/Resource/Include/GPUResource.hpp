//////////////////////////////////////////////////////////////////////////////////
/// @file   GPUResource.hpp
/// @brief  GPUでメモリを確保するリソース情報. テクスチャとバッファそれぞれがこのクラスを派生します. 
/// @author Toide Yutaro
/// @date   2024_04_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RESOURCE_HPP
#define GPU_RESOURCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
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
	*				  			GPUResource
	*************************************************************************//**
	/* @class     GPUResource 
	*  @brief     GPUでメモリを確保するリソース情報. テクスチャとバッファそれぞれがこのクラスを派生します. 
	*****************************************************************************/
	class GPUResource : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     GPUのリソース状態を変更します. 
		*  @attention ここではリソースのメンバ変数を変更するだけのために使用するため, Barrierなどと併用して使用します.
		*  @param[in] const core::ResourceState GPU処理が完了した後のリソース状態
		*  @return    void
		*************************************************************************/
		virtual void TransitionResourceState(const core::ResourceState after) = 0;

		#pragma endregion

		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief     デバッグ名を設定します
		*  @param[in] const gu::tstring& デバッグ名
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		/*!**********************************************************************
		*  @brief     現時点のGPUResourceの扱い方 (IndexBufferとして使用するなど...)を取得します
		*  @param[in] void
		*  @return    core::ResourceState リソースの扱い方
		*************************************************************************/
		virtual core::ResourceState GetResourceState() const noexcept = 0;
		
		/*!**********************************************************************
		*  @brief     テクスチャとして使用している場合にtrueを返します
		*************************************************************************/
		bool IsTexture() const { return _isTexture; }

		/*!**********************************************************************
		*  @brief     バッファとして使用している場合にtrueを返します
		*************************************************************************/
		bool IsBuffer() const { return !_isTexture; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion 
		
	protected:
		#pragma region Protected Constructor and Destructor

		/*@brief : デフォルトコンストラクタ*/
		GPUResource() = default;

		/*! @brief デストラクタ*/
		virtual ~GPUResource()
		{
			if (_device) { _device.Reset(); }
		}

		/*! @brief 論理デバイスを使って初期化するコンストラクタ*/
		explicit GPUResource(const gu::SharedPointer<RHIDevice>& device) : _device(device) {};
		
		#pragma endregion 
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Member Variables
		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		// @brief テクスチャかバッファか
		bool _isTexture = true;
		#pragma endregion
	}; 
}


#endif