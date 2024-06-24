//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsHardware.hpp
///  @brief  CPU��GPU�Ȃǂ̃n�[�h�E�F�A�����擾����N���X�ł�.
///  @author toide
///  @date   2024/06/23 18:40:50
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_HARDWARE_HPP
#define WINDOWS_HARDWARE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			   WindowsHardware
	*************************************************************************//**
	/*   @brief  CPU��GPU�Ȃǂ̃n�[�h�E�F�A�����擾����N���X�ł�.
	*****************************************************************************/
	class CoreHardware
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �����R�A�����擾���܂�
		*  @param[in] void
		*  @return    gu::uint32 �����R�A��
		*************************************************************************/
		static gu::uint32 GetPhysicalCoreCount();

		/*!**********************************************************************
		*  @brief     �_���R�A�����擾���܂�
		*  @param[in] void
		*  @return    gu::uint32 �����R�A��
		*************************************************************************/
		static gu::uint32 GetLogicalCoreCount();

		/*!**********************************************************************
		*  @brief     CPU�̃x���_�[�����擾���܂�
		*  @param[in] void
		*  @return    tstring �x���_�[��
		*************************************************************************/
		static gu::tstring GetCPUVendorName();

		/*!**********************************************************************
		*  @brief     CPU�̃��f�������擾���܂�
		*  @param[in] void
		*  @return    tstring ���f����
		*************************************************************************/
		static gu::tstring GetCPUModelName();

		/*!**********************************************************************
		*  @brief     CPU�̃N���b�N���g�����擾���܂�
		*  @param[in] void
		*  @return    gu::uint32 �N���b�N���g��
		*************************************************************************/
		static gu::uint32 GetCPUClockFrequency();

		/*!**********************************************************************
		*  @brief     CPU�̃L���b�V�����C���T�C�Y���擾���܂�. �L���b�V���Ɋi�[����P�ʃf�[�^�̑傫��.[Byte]
		*  @param[in] void
		*  @return    gu::uint32 �L���b�V�����C���T�C�Y
		*************************************************************************/
		static gu::uint32 GetCPUCacheLineSize();

		/*!**********************************************************************
		*  @brief     GPU�̃x���_�[�����擾���܂�
		*  @param[in] void
		*  @return    tstring �x���_�[��
		*************************************************************************/
		static gu::tstring GetGPUVendorName();

		/*!**********************************************************************
		*  @brief     GPU�̃��f�������擾���܂�
		*  @param[in] void
		*  @return    tstring ���f����
		*************************************************************************/
		static gu::tstring GetGPUModelName();

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property
		
		/*! @brief CPU�̃x���_�[��*/
		static gu::tstring CPU_VENDER_NAME;

		/*! @brief CPU�̃u�����h��*/
		static gu::tstring CPU_MODEL_NAME;

		/*! @brief GPU�̃x���_�[��*/
		static gu::tstring GPU_VENDER_NAME;

		#pragma endregion 

	};
}

#endif