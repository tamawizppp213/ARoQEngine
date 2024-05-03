//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUState.hpp
///  @brief  Shader pipeline�Ŏg�p����e�ݒ荀�ڂ̊��N���X�ł�. 
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
	/* @brief     Shader pipeline�Ŏg�p����e�ݒ荀�ڂ̊��N���X�ł�. 
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

		/*! @brief  �f�t�H���g�R���X�g���N�^*/
		GPUState() = default;

		/*! @brief �f�t�H���g�f�X�g���N�^*/
		virtual ~GPUState() = default;

		/*! @brief �_���f�o�C�X���g���ď��������܂�. */
		explicit GPUState(const gu::SharedPointer<RHIDevice>& device) : _device(device) {};
		
		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Member Variables
		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice>  _device = nullptr;
		#pragma endregion	
	};

}
#endif