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
#include "GameUtility/Container/Include/GUStack.hpp"

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
		*  @return    bool : 読み込み成功したかではなく, まだ後続の読み込みが存在するか
		*************************************************************************/
		bool ReadNext(JsonNotation& notation);

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     Jsonのキー名を取得します. 
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline const tstring& GetKey() const { return _key; }

		/*!**********************************************************************
		*  @brief     JsonのValueとして, Stringを取得します.
		*  @param[in] void
		*  @return    const tstring
		*************************************************************************/
		__forceinline const tstring& GetValueAsString() const
		{
			Check(_token == JsonToken::String);
			return _stringValue;
		}

		/*!**********************************************************************
		*  @brief     JsonのValueとして, Numberを取得します.
		*  @param[in] void
		*  @return    double
		*************************************************************************/
		__forceinline double GetValueAsNumber() const
		{
			Check(_token == JsonToken::Number);
			return _numberValue;
		}

		/*!**********************************************************************
		*  @brief     JsonのValueとして, Number文字列を取得します.
		*  @param[in] void
		*  @return    coknst tstring
		*************************************************************************/
		__forceinline const tstring& GetValueAsNumberString() const
		{
			Check(_token == JsonToken::String);
			return _stringValue;
		}

		/*!**********************************************************************
		*  @brief     JsonのValueとして, Number文字列を取得します.
		*  @param[in] void
		*  @return    coknst tstring
		*************************************************************************/
		__forceinline bool GetValueAsBoolean() const
		{
			Check(_token == JsonToken::True || _token == JsonToken::False);
			return _boolValue;
		}


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
		*  @brief     JsonTokenを取得します
		*  @param[out]JsonToken&
		*  @return    bool
		*************************************************************************/
		bool ReadStart(JsonToken& token);

		/*!**********************************************************************
		*  @brief     JsonObjectを取得します
		*  @param[out]JsonToken&
		*  @return    bool
		*************************************************************************/
		bool ReadNextObjectValue(JsonToken& token);

		/*!**********************************************************************
		*  @brief     JsonArrayを取得します
		*  @param[out]JsonToken&
		*  @return    bool
		*************************************************************************/
		bool ReadNextArrayValue(JsonToken& token);

		/*!**********************************************************************
		*  @brief      次のトークンが見つかるまでファイルポインタを進めます. 
		*  @param[out] JsonToken token : 次のトークン
		*  @return     bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool ParseNextToken(JsonToken& token);

		/*!**********************************************************************
		*  @brief      数値型を読み取ります. 
		*  @param[in]  void
		*  @return     bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool ParseNumberToken();

		/*!**********************************************************************
		*  @brief      文字列型を読み取ります.
		*  @param[in]  void
		*  @return     bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool ParseStringToken();

		/*!**********************************************************************
		*  @brief      空白文字列が続くまで進みます
		*  @param[in]  const tchar 文字列
		*  @return     bool : 読み込みに成功したかどうか
		*************************************************************************/
		bool ParseWhiteSpace();

		/*!**********************************************************************
		*  @brief      最後まで文字列を読み込んだか
		*  @param[in]  void
		*  @return     bool : 読み込みに成功したかどうか
		*************************************************************************/
		__forceinline bool ReadAtEnd() const
		{
			return (_parseIndex >= _rawData.Size() && _parseIndex != static_cast<gu::uint64>(-1));
		}

		/*!**********************************************************************
		*  @brief      次の文字に進めるかを確認し, 問題なければIndexを次に進める.
		*  @param[in]  void
		*  @return     bool
		*************************************************************************/
		__forceinline bool TryToNextIndex()
		{
			const auto index = _parseIndex + 1;
			const auto enable = index < _rawData.Size();
			if (enable)
			{
				_parseIndex = index;
			}
			return enable;
		}

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
		gu::uint64 _parseIndex = static_cast<gu::uint64>(-1);

		/*! @brief キー名*/
		gu::tstring _key = SP("");

		/*! @brief 生データ*/
		gu::tstring _rawData = SP("");

		/*! @brief Jsonの状態を保持しておく変数. (Json文字列階層構造を考慮したものです.)*/
		gu::Stack<JsonValueType> _parseStates = {};

		/*! @brief 現在のToken*/
		JsonToken _token = JsonToken::None;

		/*! @brief RootObjectが読み込み終了したかどうか*/
		bool _hasReadRootObject = false;

		/*! @brief ブール値*/
		bool _boolValue = false;

		/*! @brief 数値*/
		double _numberValue = 0.0;

		/*! @brief 文字列*/
		gu::tstring _stringValue = SP("");
		#pragma endregion 

	};
}


#endif