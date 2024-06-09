//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueObject.hpp
///  @brief  Object�^��Json�̒l�������N���X
///  @author toide
///  @date   2024/06/09 0:47:13
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_NULL_HPP
#define JSON_NULL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "JsonValue.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	/****************************************************************************
	*				  			   JsonValueObject
	*************************************************************************//**
	/*  @brief  Object�^��Json�̒l�������N���X
	*****************************************************************************/
	struct JsonValueObject : public JsonValue
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     JsonObject�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    JsonObject
		*************************************************************************/
		virtual SharedPointer<JsonObject> GetObject() override;

		/*!**********************************************************************
		*  @brief     JsonObject�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    JsonObject
		*************************************************************************/
		virtual const SharedPointer<JsonObject> GetObject() const override;

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		JsonValueObject() : JsonValue(JsonValueType::Object) {}

		/*! @brief �I�u�W�F�N�g�^���g���ď�����*/
		JsonValueObject(SharedPointer<JsonObject>& object) : JsonValue(JsonValueType::Object), _object(object) {}

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
		/*! @brief �ۑ�����I�u�W�F�N�g*/
		SharedPointer<JsonObject> _object = nullptr;

		#pragma endregion 
	};
}
#endif