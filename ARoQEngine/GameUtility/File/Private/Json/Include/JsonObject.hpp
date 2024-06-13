//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonObject.hpp
///  @brief  本クラスは, Jsonのオブジェクトを扱うクラスです.@n
///          Jsonのオブジェクトとは, {"key":"value"}のような形式のデータです.@n
///  @author toide
///  @date   2024/06/08 18:02:51
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_OBJECT_HPP
#define JSON_OBJECT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "JsonType.hpp"
#include "GameUtility/Container/Include/GUHashMap.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::file::json
{
	struct JsonValue;

	/****************************************************************************
	*				  			   JsonObject
	****************************************************************************/
	/*  @brief  Jsonのオブジェクトを扱うクラス
	*           Jsonのオブジェクトとは, {"key":"value"}のような形式のデータです.@n
	*****************************************************************************/
	struct JsonObject
	{
	private:
		using ObjectMap = HashMap<tstring, SharedPointer<JsonValue>>;

	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     生のHashMapを返します.
		*  @param[in] void
		*  @return    HashMap<tstring, SharedPointer<JsonValue>>
		*************************************************************************/
		ObjectMap& Data() { return _values; }

		/*!**********************************************************************
		*  @brief     生のHashMapをconst参照付きで返します.
		*  @param[in] void
		*  @return    HashMap<tstring, SharedPointer<JsonValue>>
		*************************************************************************/
		const ObjectMap& Data() const { return _values; }

		#pragma region Get Field
		/*!**********************************************************************
		*  @brief     自身のMemberを取得します.
		*  @param[in] const tstring& key : 取得したいFieldの名前
		*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
		*  @return    SharedPointer<JsonValue>
		*************************************************************************/
		SharedPointer<JsonValue> GetMember(const tstring& key, const JsonValueType type) const;

		/*!**********************************************************************
		*  @brief     既存のMemberに値を設定します.
		*  @param[in] const tstring& key : 追加したいFieldの名前
		*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
		*  @return    void
		*************************************************************************/
		void SetMember(const tstring& key, const SharedPointer<JsonValue>& value);

		/*!**********************************************************************
		*  @brief     新しくMemberを追加します.
		*  @param[in] const tstring& key : 追加したいFieldの名前
		*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
		*  @return    void
		*************************************************************************/
		void AddMember(const tstring& key, const SharedPointer<JsonValue>& value);

		/*!**********************************************************************
		*  @brief     新しくMemberを追加します.
		*  @param[in] tstring&& key : 追加したいFieldの名前
		*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
		*  @return    void
		*************************************************************************/
		void AddMember(tstring&& key, const SharedPointer<JsonValue>& value);

		/*!**********************************************************************
		*  @brief     自身のMemberを削除します.
		*  @param[in] const tstring& key : 追加したいFieldの名前
		*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
		*  @return    void
		*************************************************************************/
		void RemoveMember(const tstring& key);

		/*!**********************************************************************
		*  @brief     指定した名前のFieldを持っているかを確認します
		*  @param[in] const tstring& fieldName : 確認したいFieldの名前
		*  @return    bool
		*************************************************************************/
		bool HasMember(const tstring& fieldName) const;

		/*!**********************************************************************
		*  @brief     指定した名前のFieldと型を持っているかを確認します
		*  @param[in] const tstring& fieldName : 確認したいFieldの名前
		*  @return    bool
		*************************************************************************/
		bool HasTypedMember(const tstring& fieldName, const JsonValueType type) const;

		#pragma endregion Get Field
		
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
		/*! @brief オブジェクトを格納するキーバリューペア*/
		ObjectMap _values = {};
		#pragma endregion 

	};

}
#endif