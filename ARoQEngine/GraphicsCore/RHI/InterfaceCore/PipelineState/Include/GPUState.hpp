//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUState.hpp
///  @brief  Shader pipelineで使用する各設定項目の基底クラスです. 
///  @author Toide Yutaro
///  @date   2024_04_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_STATE_HPP
#define GPU_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
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
	*				  			GPUState
	*************************************************************************//**
	/* @brief     Shader pipelineで使用する各設定項目の基底クラスです. 
	*****************************************************************************/
	class GPUState : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		#pragma endregion 

		#pragma region Public Member Variables
		#pragma endregion 

		#pragma region Public Constructor and Destructor
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief  デフォルトコンストラクタ*/
		GPUState() = default;

		/*! @brief デフォルトデストラクタ*/
		virtual ~GPUState() = default;

		/*! @brief 論理デバイスを使って初期化します. */
		explicit GPUState(const gu::SharedPointer<RHIDevice>& device) : _device(device) {};
		
		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Member Variables
		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice>  _device = nullptr;
		#pragma endregion	
	};

}
#endif