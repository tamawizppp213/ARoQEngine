//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsOS.hpp
///  @brief  OS�����܂Ƃ߂��N���X. ���W�X�g���L�[�̎擾�Ȃǂ��s���܂�.
///  @author toide
///  @date   2024/06/23 23:42:51
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_OS_HPP
#define WINDOWS_OS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
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
	*				  			   WindowsOS
	*************************************************************************//**
	/*  @brief   OS�����܂Ƃ߂��N���X
	*****************************************************************************/
	class WindowsOS
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ���W�X�g���L�[��T���܂�.
		*  @param[in] void* hKey ���[�g�L�[
		*  @param[in] const tstring& subKey �T�u�L�[
		*  @param[in] const tstring& valueName �l��
		*  @param[out] tstring& �o�͕�����
		*  @return    bool �L�[�����݂��邩�ǂ���
		*************************************************************************/
		static bool QueryRegistryKey(void* hKey, const gu::tstring& subKey, const gu::tstring& valueName, gu::tstring& outData);

		/*!**********************************************************************
		*  @brief     Visual studio��CommonTools�̃p�X���擾���܂� (�擾�ł��Ȃ��ꍇ�͋�̕������Ԃ��܂�.)
		*  @param[in] const gu::uint32 Visual Studio�̃��W���[�o�[�W���� (VS2022�̏ꍇ��17)
		*  @return    tstring CommonTools�̃p�X
		*************************************************************************/
		static gu::tstring GetVisualStudioCommonToolsPath(const gu::uint32 majorVersion);

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

		#pragma endregion 

	};
}

#endif