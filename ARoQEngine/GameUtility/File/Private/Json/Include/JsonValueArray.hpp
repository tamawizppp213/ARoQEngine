//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueArray.hpp
///  @brief  �z��^��Json�̒l�������N���X
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
#include "GameUtility/Base/Include/GUTypeCast.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	/****************************************************************************
	*				  		JsonValueArray
	*************************************************************************//**
	/*  @brief  �z��^��Json�̒l�������N���X
	*****************************************************************************/
	struct JsonValueArray : public JsonValue
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     Array�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    DynamicArray<SharedPointer<JsonValue>>
		*************************************************************************/
		virtual DynamicArray<SharedPointer<JsonValue>>& GetArray() override;

		/*!**********************************************************************
		*  @brief     Array�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    DynamicArray<SharedPointer<JsonValue>>
		*************************************************************************/
		virtual const DynamicArray<SharedPointer<JsonValue>>& GetArray() const override;

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		JsonValueArray() : JsonValue(JsonValueType::Array) {}

		/*! @brief �f�X�g���N�^*/
		virtual ~JsonValueArray()
		{
			_array.Clear(); _array.ShrinkToFit();
		}

		/*! @brief �R�s�[�R���X�g���N�^*/
		JsonValueArray(const JsonValueArray& other) : JsonValue(JsonValueType::Array)
		{
			_array = other._array;
		}

		/*! @brief ���[�u�R���X�g���N�^*/
		JsonValueArray(JsonValueArray&& other) noexcept : JsonValue(JsonValueType::Array)
		{
			_array = gu::type::Forward<DynamicArray<SharedPointer<JsonValue>>>(other._array);
		}

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

		/*! @brief Json�̔z��*/
		DynamicArray<SharedPointer<JsonValue>> _array = {};

		#pragma endregion 
	};
}
#endif