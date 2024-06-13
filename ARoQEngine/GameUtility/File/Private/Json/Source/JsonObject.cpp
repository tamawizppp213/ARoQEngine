//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonObject.cpp
///  @brief  temp
///  @author toide
///  @date   2024/06/11 1:22:57
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonObject.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueNull.hpp"
#include "GameUtility/Base/Include/GUTypeCast.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::file::json;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function

/*!**********************************************************************
*  @brief     自身のMemberを取得します.
*  @param[in] const tstring& key : 取得したいFieldの名前
*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
*  @return    SharedPointer<JsonValue>
*************************************************************************/
SharedPointer<JsonValue> JsonObject::GetMember(const tstring& key, const JsonValueType type) const
{
	const auto member = _values.Find(key);

	// 見つからなかった場合はNull型のMemberとして返す
	if (member == _values.end())
	{
		return MakeShared<JsonValueNull>();
	}
	
	// 見つかった場合は, 型が一致しているかを確認
	if (member->Value->GetValueType() != type)
	{
		return MakeShared<JsonValueNull>();
	}

	return member->Value;
}

/*!**********************************************************************
*  @brief     既存のMemberに値を設定します.
*  @param[in] const tstring& key : 追加したいFieldの名前
*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
*  @return    void
*************************************************************************/
void JsonObject::SetMember(const tstring& key, const SharedPointer<JsonValue>& value)
{
	_values.At(key) = value;
}

/*!**********************************************************************
*  @brief     新しくMemberを追加します.
*  @param[in] const tstring& key : 追加したいFieldの名前
*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
*  @return    void
*************************************************************************/
void JsonObject::AddMember(const tstring& key, const SharedPointer<JsonValue>& value)
{
	_values[key] = value;
}

/*!**********************************************************************
*  @brief     新しくMemberを追加します.
*  @param[in] const tstring& key : 追加したいFieldの名前
*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
*  @return    void
*************************************************************************/
void JsonObject::AddMember(tstring&& key, const SharedPointer<JsonValue>& value)
{
	_values[gu::type::Forward<tstring>(key)] = value;
}

/*!**********************************************************************
*  @brief     自身のMemberを削除します.
*  @param[in] const tstring& key : 追加したいFieldの名前
*  @param[in] const JsonValueType type : 取得したいFieldのデータ型
*  @return    void
*************************************************************************/
void JsonObject::RemoveMember(const tstring& key)
{
	_values.Remove(key);
}

/*!**********************************************************************
*  @brief     指定した名前のFieldを持っているかを確認します
*  @param[in] const tstring& fieldName : 確認したいFieldの名前
*  @return    bool
*************************************************************************/
bool JsonObject::HasMember(const tstring& fieldName) const
{
	return _values.Contains(fieldName);
}

/*!**********************************************************************
*  @brief     指定した名前のFieldと型を持っているかを確認します
*  @param[in] const tstring& fieldName : 確認したいFieldの名前
*  @return    bool
*************************************************************************/
bool JsonObject::HasTypedMember(const tstring& fieldName, const JsonValueType type) const
{
	const auto member = _values.Find(fieldName);

	if(member == _values.end())
	{
		return false;
	}

	return member->Value->GetValueType() == type;
}
#pragma endregion Public Function