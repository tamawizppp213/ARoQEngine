//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_WINDOW_MESSAGE_HANDLER_HPP
#define WINDOWS_WINDOW_MESSAGE_HANDLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "GameUtility/Base/Include/GUType.hpp"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  		CoreWindowMessageHandler
	****************************************************************************/
	/* @class     CoreWindowMessageHandler
	*  @brief     Cursor interface
	*****************************************************************************/
	class WindowsMessage
	{
	public:
		#pragma region Static Function
		/*!***********************************************************************
		*  @brief      �L�[�{�[�h�ɂ����̓��b�Z�[�W���ǂ���
		*  @param[in]  const uint32 ���b�Z�[�W
		*  @return     bool
		**************************************************************************/
		static bool IsKeyboardMessage(const gu::uint32 message);

		/*!***********************************************************************
		*  @brief      �}�E�X�ɂ����̓��b�Z�[�W���ǂ���
		*  @param[in]  const uint32 ���b�Z�[�W
		*  @return     bool
		**************************************************************************/
		static bool IsMouseMessage(const gu::uint32 message);

		/*!***********************************************************************
		*  @brief      uint32�ł��邽��, ���������̓��b�Z�[�W�ɕϊ��\��
		*  @param[in]  const uint32 ���b�Z�[�W
		*  @return     bool
		**************************************************************************/
		static bool IsInputMessage(const gu::uint32 message);

		#pragma endregion

		#pragma region Public Function
		
		#pragma endregion 

		#pragma region Public Property
		
		#pragma endregion Public Property

		#pragma region Public Constructor and Destructor
		WindowsMessage();

		~WindowsMessage();
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Property
		#pragma endregion


	};
}
#endif
#endif