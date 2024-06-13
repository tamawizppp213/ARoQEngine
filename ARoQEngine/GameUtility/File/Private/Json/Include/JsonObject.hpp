//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonObject.hpp
///  @brief  �{�N���X��, Json�̃I�u�W�F�N�g�������N���X�ł�.@n
///          Json�̃I�u�W�F�N�g�Ƃ�, {"key":"value"}�̂悤�Ȍ`���̃f�[�^�ł�.@n
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
	/*  @brief  Json�̃I�u�W�F�N�g�������N���X
	*           Json�̃I�u�W�F�N�g�Ƃ�, {"key":"value"}�̂悤�Ȍ`���̃f�[�^�ł�.@n
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
		*  @brief     ����HashMap��Ԃ��܂�.
		*  @param[in] void
		*  @return    HashMap<tstring, SharedPointer<JsonValue>>
		*************************************************************************/
		ObjectMap& Data() { return _values; }

		/*!**********************************************************************
		*  @brief     ����HashMap��const�Q�ƕt���ŕԂ��܂�.
		*  @param[in] void
		*  @return    HashMap<tstring, SharedPointer<JsonValue>>
		*************************************************************************/
		const ObjectMap& Data() const { return _values; }

		#pragma region Get Field
		/*!**********************************************************************
		*  @brief     ���g��Member���擾���܂�.
		*  @param[in] const tstring& key : �擾������Field�̖��O
		*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
		*  @return    SharedPointer<JsonValue>
		*************************************************************************/
		SharedPointer<JsonValue> GetMember(const tstring& key, const JsonValueType type) const;

		/*!**********************************************************************
		*  @brief     ������Member�ɒl��ݒ肵�܂�.
		*  @param[in] const tstring& key : �ǉ�������Field�̖��O
		*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
		*  @return    void
		*************************************************************************/
		void SetMember(const tstring& key, const SharedPointer<JsonValue>& value);

		/*!**********************************************************************
		*  @brief     �V����Member��ǉ����܂�.
		*  @param[in] const tstring& key : �ǉ�������Field�̖��O
		*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
		*  @return    void
		*************************************************************************/
		void AddMember(const tstring& key, const SharedPointer<JsonValue>& value);

		/*!**********************************************************************
		*  @brief     �V����Member��ǉ����܂�.
		*  @param[in] tstring&& key : �ǉ�������Field�̖��O
		*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
		*  @return    void
		*************************************************************************/
		void AddMember(tstring&& key, const SharedPointer<JsonValue>& value);

		/*!**********************************************************************
		*  @brief     ���g��Member���폜���܂�.
		*  @param[in] const tstring& key : �ǉ�������Field�̖��O
		*  @param[in] const JsonValueType type : �擾������Field�̃f�[�^�^
		*  @return    void
		*************************************************************************/
		void RemoveMember(const tstring& key);

		/*!**********************************************************************
		*  @brief     �w�肵�����O��Field�������Ă��邩���m�F���܂�
		*  @param[in] const tstring& fieldName : �m�F������Field�̖��O
		*  @return    bool
		*************************************************************************/
		bool HasMember(const tstring& fieldName) const;

		/*!**********************************************************************
		*  @brief     �w�肵�����O��Field�ƌ^�������Ă��邩���m�F���܂�
		*  @param[in] const tstring& fieldName : �m�F������Field�̖��O
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
		/*! @brief �I�u�W�F�N�g���i�[����L�[�o�����[�y�A*/
		ObjectMap _values = {};
		#pragma endregion 

	};

}
#endif