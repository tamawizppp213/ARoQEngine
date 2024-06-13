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
*  @brief     ���g��Member���擾���܂�.
*  @param[in] const tstring& key : �擾������Field�̖��O
*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
*  @return    SharedPointer<JsonValue>
*************************************************************************/
SharedPointer<JsonValue> JsonObject::GetMember(const tstring& key, const JsonValueType type) const
{
	const auto member = _values.Find(key);

	// ������Ȃ������ꍇ��Null�^��Member�Ƃ��ĕԂ�
	if (member == _values.end())
	{
		return MakeShared<JsonValueNull>();
	}
	
	// ���������ꍇ��, �^����v���Ă��邩���m�F
	if (member->Value->GetValueType() != type)
	{
		return MakeShared<JsonValueNull>();
	}

	return member->Value;
}

/*!**********************************************************************
*  @brief     ������Member�ɒl��ݒ肵�܂�.
*  @param[in] const tstring& key : �ǉ�������Field�̖��O
*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
*  @return    void
*************************************************************************/
void JsonObject::SetMember(const tstring& key, const SharedPointer<JsonValue>& value)
{
	_values.At(key) = value;
}

/*!**********************************************************************
*  @brief     �V����Member��ǉ����܂�.
*  @param[in] const tstring& key : �ǉ�������Field�̖��O
*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
*  @return    void
*************************************************************************/
void JsonObject::AddMember(const tstring& key, const SharedPointer<JsonValue>& value)
{
	_values[key] = value;
}

/*!**********************************************************************
*  @brief     �V����Member��ǉ����܂�.
*  @param[in] const tstring& key : �ǉ�������Field�̖��O
*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
*  @return    void
*************************************************************************/
void JsonObject::AddMember(tstring&& key, const SharedPointer<JsonValue>& value)
{
	_values[gu::type::Forward<tstring>(key)] = value;
}

/*!**********************************************************************
*  @brief     ���g��Member���폜���܂�.
*  @param[in] const tstring& key : �ǉ�������Field�̖��O
*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
*  @return    void
*************************************************************************/
void JsonObject::RemoveMember(const tstring& key)
{
	_values.Remove(key);
}

/*!**********************************************************************
*  @brief     �w�肵�����O��Field�������Ă��邩���m�F���܂�
*  @param[in] const tstring& fieldName : �m�F������Field�̖��O
*  @return    bool
*************************************************************************/
bool JsonObject::HasMember(const tstring& fieldName) const
{
	return _values.Contains(fieldName);
}

/*!**********************************************************************
*  @brief     �w�肵�����O��Field�ƌ^�������Ă��邩���m�F���܂�
*  @param[in] const tstring& fieldName : �m�F������Field�̖��O
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