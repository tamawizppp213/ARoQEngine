//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonNumber.hpp
///  @brief  ���l�^��Json�̒l�������N���X
///  @author toide
///  @date   2024/06/08 22:50:12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_NUMBER_HPP
#define JSON_NUMBER_HPP

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
	*				  			   JsonNumber
	*************************************************************************//**
	/*  @brief  Json�̐��l�^�̒l�������N���X
	*****************************************************************************/
	struct JsonNumber : public JsonValue
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

		#pragma endregion GetValue

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		JsonNumber() : JsonValue(JsonValueType::Number) {}

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
		double _value = 0.0;

		#pragma endregion 

	};
}

#endif