//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonWritePolicy.hpp
///  @brief  Jsonファイルの書き込み方針
///  @author toide
///  @date   2024/07/30 1:27:00
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_WRITE_POLICY_HPP
#define JSON_WRITE_POLICY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	/****************************************************************************
	*				  			   JsonWritePolicy
	****************************************************************************/
	/* @brief  Jsonファイルの書き込み方針
	*****************************************************************************/
	class JsonWritePolicy : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief        文字を一文字追加します. 
		*  @param[inout] gu::tstring& : 入力文字列
		*  @param[in]    const tchar   : 追加したい文字
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteChar(gu::tstring& string, const tchar character)
		{
			string += character;
		}

		/*!**********************************************************************
		*  @brief        文字列を追加します.
		*  @param[inout] gu::tstring& : 入力文字列
		*  @param[in]    const gu::tstring : 追加したい文字列
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteString(gu::tstring& string, const gu::tstring& addString)
		{
			string += addString;
		}

		/*!**********************************************************************
		*  @brief        改行を追加します.
		*  @param[inout] gu::tstring& : 入力文字列
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteLineTerminator(gu::tstring& string)
		{
			string += SP("\r\n");
		}

		/*!**********************************************************************
		*  @brief        スペースを追加します.
		*  @param[inout] gu::tstring& : 入力文字列
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteSpace(gu::tstring& string)
		{
			string += SP(" ");
		}

		/*!**********************************************************************
		*  @brief        スペースを追加します.
		*  @param[inout] gu::tstring& : 入力文字列
		*  @param[in]    gu::int32     : 追加したいスペースの数
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteTab(gu::tstring& string, const gu::int32 count)
		{
			for (gu::int32 i = 0; i < count; ++i)
			{
				string += SP("\t");
			}
		}

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