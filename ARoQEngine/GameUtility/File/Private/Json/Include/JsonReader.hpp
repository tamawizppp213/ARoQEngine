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
		*  @brief     Jsonファイルを最後まで読み込みます. 
		*  @param[in] const gu::tstring& filePath : 読み込みたいJsonファイルのパス
		*  @param[in] const bool 非同期読み込みを行うか
		*  @return    bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool Read(const gu::tstring& filePath, const bool useAsync);

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
		/*!**********************************************************************
		*  @brief      次のトークンが見つかるまでファイルポインタを進めます. 
		*  @param[in]  const tstring& jsonファイルを文字列化したもの
		*  @param[out] JsonToken token : 次のトークン
		*  @return     bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool ParseNextToken(const tstring& json, JsonToken& token);

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
		#pragma endregion 

		#pragma region Private Property 
		
		/*! @brief Parseを行っているインデックス*/
		gu::uint64 _parseIndex = 0;

		/*! @brief jsonの行数*/
		gu::uint32  _lineCount = 0;

		/*! @brief 各行の文字数*/
		gu::uint32 _characterCount = 0;

		#pragma endregion 

	};
}


#endif