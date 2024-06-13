//////////////////////////////////////////////////////////////////////////////////
///  @file   GUStringConverter.hpp
///  @brief  文字列を変換する機能を提供します.
///  @author toide
///  @date   2024/05/19 15:17:13
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_STRING_CONVERTER_HPP
#define GU_STRING_CONVERTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUStringConverter
	****************************************************************************/
	/* @class     GUStringConverter
	*   @brief     文字列を変換する機能を提供します.
	*****************************************************************************/
	class StringConverter : public NonCopyAndMove
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     UTF8からUTF16に文字列を変換します.
		*  @param[in] const u8string& utf8
		*  @return    u16string
		*************************************************************************/
		static u16string ConvertUTF8ToUTF16(const u8string& utf8);

		/*!**********************************************************************
		*  @brief     UTF8からUTF32に文字列を変換します.
		*  @param[in] const u8string& utf8
		*  @return    u16string
		*************************************************************************/
		static u32string ConvertUTF8ToUTF32(const u8string& utf8);

		/*!**********************************************************************
		*  @brief     UTF16からUTF32に文字列を変換します.
		*  @param[in] const u16string& utf16
		*  @return    u16string
		*************************************************************************/
		static u8string ConvertUTF16ToUTF8(const u16string& utf16);

		/*!**********************************************************************
		*  @brief     UTF16からUTF32に文字列を変換します.
		*  @param[in] const u16string& utf16
		*  @return    u16string
		*************************************************************************/
		static u32string ConvertUTF16ToUTF32(const u16string& utf16);

		/*!**********************************************************************
		*  @brief     UTF32からUTF8に文字列を変換します. 
		*  @param[in] const u32string& utf32
		*  @return    string
		*************************************************************************/
		static string ConvertUTF32ToUTF8(const u32string& utf32);

		/*!**********************************************************************
		*  @brief     UTF32からUTF16に文字列を変換します.
		*  @param[in] const u32string& utf32
		*  @return    u16string
		*************************************************************************/
		static u16string ConvertUTF32ToUTF16(const u32string& utf32);

		/*!**********************************************************************
		*  @brief     StringからにWStringに文字列を変換します.
		*  @param[in] const string& utf8
		*  @return    wstring
		*************************************************************************/
		static wstring ConvertStringToWString(const string& utf8);

		/*!**********************************************************************
		*  @brief     WStringからにStringに文字列を変換します.
		*  @param[in] const wstring& utf8
		*  @return    wstring
		*************************************************************************/
		static string ConvertWStringToString(const wstring& wideString);

		/*!**********************************************************************
		*  @brief     StringからにTStringに文字列を変換します.
		*  @param[in] const string& utf8
		*  @return    tstring
		*************************************************************************/
		static tstring ConvertStringToTString(const string& utf8);

		/*!**********************************************************************
		*  @brief     TStringからにStringに文字列を変換します.
		*  @param[in] const tstring& tString
		*  @return    tstring
		*************************************************************************/
		static string ConvertTStringToString(const tstring& tString);
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