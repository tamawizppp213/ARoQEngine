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
	*************************************************************************//**
	/*  @class     JsonObject
	*   @brief     temp
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