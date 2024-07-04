//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonReader.hpp
///  @brief  temp
///  @author toide
///  @date   2024/06/13 23:19:53
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_READER_HPP
#define JSON_READER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "JsonCommon.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace platform::core::file
{
	class IFileHandle;
}


//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::file::json
{
	struct JsonValue;
	struct JsonObject;

	/****************************************************************************
	*				  			   JsonReader
	****************************************************************************/
	/* @brief Jsonファイルを読み込みます.
	*         指定した文字コードはUTF-8を想定しています.
	*****************************************************************************/
	class JsonReader
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     次の値を読み込みます
		*  @param[out]JsonNotation&  
		*  @return    bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool ReadNext(JsonNotation& notation);

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		JsonReader(const gu::tstring& filePath, const bool useAsync = false)
		{
			Initialize(filePath, useAsync);
		}
		
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     Jsonファイルを最後まで読み込みます.
		*  @param[in] const gu::tstring& filePath : 読み込みたいJsonファイルのパス
		*  @param[in] const bool 非同期読み込みを行うか
		*  @return    bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool Initialize(const gu::tstring& filePath, const bool useAsync);

		/*!**********************************************************************
		*  @brief      次のトークンが見つかるまでファイルポインタを進めます. 
		*  @param[in]  const tstring& jsonファイルを文字列化したもの
		*  @param[out] JsonToken token : 次のトークン
		*  @return     bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool ParseNextToken(const tstring& json, JsonToken& token);

		/*!**********************************************************************
		*  @brief      数値型を読み取ります. 
		*  @param[in]  const tchar 文字列
		*  @return     bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool ParseNumberToken(const tstring& json);

		#pragma endregion 

		#pragma region Protected Property

		/*! @brief ファイル読み込み*/
		gu::SharedPointer<platform::core::file::IFileHandle> _fileSystem = nullptr;

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function
		/*!**********************************************************************
		*  @brief      空白文字かどうかを返します
		*  @param[out] tchar 文字
		*  @return     bool
		*************************************************************************/
		__forceinline bool IsWhiteSpace(const tchar character)
		{
			return character == SP(' ') || character == SP('\t') || character == SP('\n') || character == SP('\r');
		}

		/*!**********************************************************************
		*  @brief      Jsonの数値型の判定
		*  @param[out] const tchar character
		*  @return     bool
		*************************************************************************/
		__forceinline bool IsNumber(const tchar character)
		{
			return (SP('0') <= character && character <= SP('9')) || character == SP('-') || character == SP('.')
				|| character == SP('+') || character == SP('e') || character == SP('E');
		}

		#pragma endregion 

		#pragma region Private Property 
		
		/*! @brief Parseを行っているインデックス*/
		gu::uint64 _parseIndex = 0;

		/*! @brief キー名*/
		gu::tstring _key = SP("");

		/*! @brief 生データ*/
		gu::tstring _rawData = SP("");

		/*! @brief 現在のToken*/
		JsonToken _token = JsonToken::None;

		/*! @brief */

		#pragma endregion 

	};
}


#endif