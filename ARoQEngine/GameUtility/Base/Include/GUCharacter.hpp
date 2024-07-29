//////////////////////////////////////////////////////////////////////////////////
///  @file   GUCharacter.hpp
///  @brief  1�������̕������e���`����N���X
///  @author toide
///  @date   2024/07/28 22:08:49
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_CHARCTER_HPP
#define GU_CHARCTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUTypeTraits.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			  Character
	****************************************************************************/
	/* @brief  1�������̕������e���`����N���X
	*****************************************************************************/
	class Character
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ���s���ǂ����𔻒肵�܂�.
		*  @return    bool
		*************************************************************************/
		static bool IsNewLine(const char   character);
		static bool IsNewLine(const wchar  character);
		static bool IsNewLine(const char8  character);
		static bool IsNewLine(const char16 character);
		static bool IsNewLine(const char32 character);

		/*!**********************************************************************
		*  @brief     �󔒕������ǂ����𔻒肵�܂�.
		*  @return    bool
		*************************************************************************/
		static bool IsWhiteSpace(const char   character);
		static bool IsWhiteSpace(const wchar  character);
		static bool IsWhiteSpace(const char8  character);
		static bool IsWhiteSpace(const char16 character);
		static bool IsWhiteSpace(const char32 character);

		/*!**********************************************************************
		*  @brief     0����9�܂ł̐������ǂ����𔻒肵�܂�
		*  @return    bool
		*************************************************************************/
		static bool IsDigit(const char   character);
		static bool IsDigit(const wchar  character);
		static bool IsDigit(const char8  character);
		static bool IsDigit(const char16 character);
		static bool IsDigit(const char32 character);

		/*!**********************************************************************
		*  @brief     1����9�܂ł̐������ǂ����𔻒肵�܂�
		*  @return    bool
		*************************************************************************/
		static bool IsNonZeroDigit(const char   character);
		static bool IsNonZeroDigit(const wchar  character);
		static bool IsNonZeroDigit(const char8  character);
		static bool IsNonZeroDigit(const char16 character);
		static bool IsNonZeroDigit(const char32 character);

		/*!**********************************************************************
		*  @brief     Alphabet���ǂ���
		*  @return    bool
		*************************************************************************/
		static bool IsAlphabet(const char   character);
		static bool IsAlphabet(const wchar  character);
		static bool IsAlphabet(const char8  character);
		static bool IsAlphabet(const char16 character);
		static bool IsAlphabet(const char32 character);

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