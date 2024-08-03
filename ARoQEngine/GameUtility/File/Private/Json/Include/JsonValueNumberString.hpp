//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueNumberString.hpp
///  @brief  ������\������Ȃ鐔�l�^��Json�̒l�������N���X
///  @author toide
///  @date   2024/06/08 22:50:12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_VALUE_NUMBER_STRING_HPP
#define JSON_VALUE_NUMBER_STRING_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonValue.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	/****************************************************************************
	*				  		JsonValueNumber
	****************************************************************************/
	/*  @brief  Json�̐��l�^�̒l�������N���X
	*****************************************************************************/
	struct JsonValueNumberString : public JsonValue
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property

		#pragma region GetValue
		/*!**********************************************************************
		*  @brief     String�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    tstring : ������̌���
		*************************************************************************/
		virtual tstring GetString() const override;

		/*!**********************************************************************
		*  @brief     Int8�̒l���擾���܂�
		*  @param[in] void
		*  @return    int8
		*************************************************************************/
		virtual int8 GetInt8() const override;

		/*!**********************************************************************
		*  @brief     Int16�̒l���擾���܂�
		*  @param[in] void
		*  @return    int16
		*************************************************************************/
		virtual int16 GetInt16() const override;

		/*!**********************************************************************
		*  @brief     Int32�̒l���擾���܂�
		*  @param[in] void
		*  @return    int32
		*************************************************************************/
		virtual int32 GetInt32() const override;

		/*!**********************************************************************
		*  @brief     Int64�̒l���擾���܂�
		*  @param[in] void
		*  @return    int64
		*************************************************************************/
		virtual int64 GetInt64() const override;

		/*!**********************************************************************
		*  @brief     UInt8�̒l���擾���܂�
		*  @param[in] void
		*  @return    uint8
		*************************************************************************/
		virtual uint8 GetUInt8() const override;

		/*!**********************************************************************
		*  @brief     UInt16�̒l���擾���܂�
		*  @param[in] void
		*  @return    uint16
		*************************************************************************/
		virtual uint16 GetUInt16() const override;

		/*!**********************************************************************
		*  @brief     UInt32�̒l���擾���܂�
		*  @param[in] void
		*  @return    uint32
		*************************************************************************/
		virtual uint32 GetUInt32() const override;

		/*!**********************************************************************
		*  @brief     UInt64�̒l���擾���܂�
		*  @param[in] void
		*  @return    uint64
		*************************************************************************/
		virtual uint64 GetUInt64() const override;

		/*!**********************************************************************
		*  @brief     Float�̒l���擾���܂�
		*  @param[in] void
		*  @return    float
		*************************************************************************/
		virtual float GetFloat() const;

		/*!**********************************************************************
		*  @brief     Double�̒l���擾���܂�
		*  @param[in] void
		*  @return    double
		*************************************************************************/
		virtual double GetDouble() const;

		/*!**********************************************************************
		*  @brief     ���g�̌^�łȂ�, ������\���Ƃ��ė��Ƃ����݂������ǂ��� (JsonWriter�Ɏg�p���܂�)
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline virtual bool UseStringRepresentation() const override{ return true; }

		#pragma endregion GetValue

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		JsonValueNumberString() : JsonValue(JsonValueType::Number) {}

		/*! @brief double�l�Ŏ擾*/
		JsonValueNumberString(const gu::tstring& value) : JsonValue(JsonValueType::Number), _value(value) {}
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

		/*! @brief �ۑ�����l*/
		gu::tstring _value = SP("");

		#pragma endregion 

	};
}

#endif