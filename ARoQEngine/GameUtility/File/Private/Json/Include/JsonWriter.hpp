//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonWriter.hpp
///  @brief  Jsonファイルに書き込みを行うクラス.
///  @author toide
///  @date   2024/06/29 17:59:25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_WRITER_HPP
#define JSON_WRITER_HPP

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

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::file::json
{

	/****************************************************************************
	*				  			   JsonWriter
	****************************************************************************/
	/*  @brief   Jsonファイルに書き込みを行うクラス.
	*****************************************************************************/
	class JsonWriter : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     貯めた文字列データをファイルに流し込みます.
		*  @param[in] const gu::tstring filePath
		*  @param[in] const bool useAsync
		*  @return    bool
		*************************************************************************/
		bool Flush(const gu::tstring& filePath, const bool useAsync);

		/*!**********************************************************************
		*  @brief     JsonObjectの書き込みを開始します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteObjectStart();

		/*!**********************************************************************
		*  @brief     JsonObjectの書き込みを開始します 
		*  @param[in] const gu::tstring& key : キー名
		*  @return    void
		*************************************************************************/
		void WriteObjectStart(const gu::tstring& key);

		/*!**********************************************************************
		*  @brief     JsonObjectの書き込みを終了します.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteObjectEnd();

		/*!**********************************************************************
		*  @brief     JsonArrayの書き込みを開始します.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteArrayStart();

		/*!**********************************************************************
		*  @brief     JsonArrayの書き込みを開始します.
		*  @param[in] const gu::tstring& キー名
		*  @return    void
		*************************************************************************/
		void WriteArrayStart(const gu::tstring& key);

		/*!**********************************************************************
		*  @brief     JsonArrayの書き込みを終了します.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteArrayEnd();

		/*!**********************************************************************
		*  @brief     文字列データを書き込みます
		*  @param[in] const gu::tstring& key : キー名
		*  @param[in] const gu::tstring& value : バリュー名
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const gu::tstring& value);

		/*!**********************************************************************
		*  @brief     文字列データを書き込みます
		*  @param[in] const gu::tstring& key : キー名
		*  @param[in] const bool value : バリュー名
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const bool value);

		/*!**********************************************************************
		*  @brief     文字列データを書き込みます
		*  @param[in] const gu::tstring& key : キー名
		*  @param[in] const float value : バリュー名
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const float value);

		/*!**********************************************************************
		*  @brief     文字列データを書き込みます
		*  @param[in] const gu::tstring& key : キー名
		*  @param[in] const double value : バリュー名
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const double value);

		/*!**********************************************************************
		*  @brief     文字列データを書き込みます
		*  @param[in] const gu::tstring& key : キー名
		*  @param[in] const gu::int32 value : バリュー名
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const gu::int32 value);

		/*!**********************************************************************
		*  @brief     文字列データを書き込みます
		*  @param[in] const gu::tstring& key : キー名
		*  @param[in] const gu::tstring& value : 値
		*  @return    void
		*************************************************************************/
		void WriteNumberInternal(const gu::tstring& key, const gu::tstring& number);
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     現在書き込み中のJsonValueTypeを返します. 
		*  @param[in] void
		*  @return    JsonValueType
		*************************************************************************/
		JsonValueType GetCurrentValueType() const { return !_stackStates.IsEmpty() ? _stackStates.Top() : JsonValueType::Null; }

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		JsonWriter() = default;



		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		/*!**********************************************************************
		*  @brief     キー名の書き込みを行います.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteKey(const gu::tstring& key);

		/*!**********************************************************************
		*  @brief     エスケープ文字があった場合の文字列書き込み
		*  @param[in] const gu::tstring& 文字列
		*  @return    void
		*************************************************************************/
		void WriteEscapedString(const gu::tstring& value);

		/*!**********************************************************************
		*  @brief     カンマが必要なら記述
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteCommaIfNeeded();

		/*!**********************************************************************
		*  @brief     キーが必要ない場合にでもオブジェクトを書き込んでも良いか
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool EnableWriteWithoutKey() const
		{
			return _stackStates.IsEmpty() || _stackStates.Top() == JsonValueType::Array || _previousToken == JsonToken::Key || _previousToken == JsonToken::Colon;
		}
		#pragma endregion 

		#pragma region Protected Property
		/*! @brief Jsonの状態を保持しておく変数. (Json文字列階層構造を考慮したものです.*/
		gu::Stack<JsonValueType> _stackStates = {};

		/*! @brief 文字列*/
		gu::tstring _data = SP("");

		/*! @brief 階層レベル*/
		gu::int32 _indentLevel = 0;

		/*! @brief 一個前に記述したToken*/
		JsonToken _previousToken = JsonToken::None;
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