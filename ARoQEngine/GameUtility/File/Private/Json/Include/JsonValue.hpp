//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValue.hpp
///  @brief  temp
///  @author toide
///  @date   2024/06/08 13:38:55
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "JsonObject.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	/****************************************************************************
	*				  			   JsonValue
	*************************************************************************//**
	/*  @brief  Json�̒l�̎擾�Ȃǂ������N���X
	*****************************************************************************/
	struct JsonValue
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property

		#pragma region GetValue
		/*!**********************************************************************
		*  @brief     Json�̎�ނ��擾���܂�.
		*  @param[in] void
		*  @return    JsonValueType
		*************************************************************************/
		JsonValueType GetValueType() const { return _type; }

		/*!**********************************************************************
		*  @brief     String�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    tstring : ������̌���
		*************************************************************************/
		virtual tstring GetString() const;

		/*!**********************************************************************
		*  @brief     JsonObject�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    JsonObject
		*************************************************************************/
		virtual SharedPointer<JsonObject> GetObject() { return nullptr; }

		/*!**********************************************************************
		*  @brief     JsonObject�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    JsonObject
		*************************************************************************/
		virtual const SharedPointer<JsonObject> GetObject() const { return nullptr; }

		/*!**********************************************************************
		*  @brief     Array�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    DynamicArray<SharedPointer<JsonValue>>
		*************************************************************************/
		virtual DynamicArray<SharedPointer<JsonValue>>& GetArray() { throw "failed to get array"; }

		/*!**********************************************************************
		*  @brief     Array�^�̒l���擾���܂�.
		*  @param[in] void
		*  @return    DynamicArray<SharedPointer<JsonValue>>
		*************************************************************************/
		virtual const DynamicArray<SharedPointer<JsonValue>>& GetArray() const { throw "failed to get array"; }

		/*!**********************************************************************
		*  @brief     bool�̒l���擾���܂�
		*  @param[in] void
		*  @return    int8
		*************************************************************************/
		virtual bool GetBool() const { throw "failed to get bool"; }

		/*!**********************************************************************
		*  @brief     Int8�̒l���擾���܂�
		*  @param[in] void
		*  @return    int8
		*************************************************************************/
		virtual int8 GetInt8() const;

		/*!**********************************************************************
		*  @brief     Int16�̒l���擾���܂�
		*  @param[in] void
		*  @return    int16
		*************************************************************************/
		virtual int16 GetInt16() const;

		/*!**********************************************************************
		*  @brief     Int32�̒l���擾���܂�
		*  @param[in] void
		*  @return    int32
		*************************************************************************/
		virtual int32 GetInt32() const;

		/*!**********************************************************************
		*  @brief     Int64�̒l���擾���܂�
		*  @param[in] void
		*  @return    int64
		*************************************************************************/
		virtual int64 GetInt64() const;

		/*!**********************************************************************
		*  @brief     UInt8�̒l���擾���܂�
		*  @param[in] void
		*  @return    uint8
		*************************************************************************/
		virtual uint8 GetUInt8() const;

		/*!**********************************************************************
		*  @brief     UInt16�̒l���擾���܂�
		*  @param[in] void
		*  @return    uint16
		*************************************************************************/
		virtual uint16 GetUInt16() const;

		/*!**********************************************************************
		*  @brief     UInt32�̒l���擾���܂�
		*  @param[in] void
		*  @return    uint32
		*************************************************************************/
		virtual uint32 GetUInt32() const;

		/*!**********************************************************************
		*  @brief     UInt64�̒l���擾���܂�
		*  @param[in] void
		*  @return    uint64
		*************************************************************************/
		virtual uint64 GetUInt64() const;

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

		#pragma endregion TryGetValue

		#pragma region Type Check
		/*!**********************************************************************
		*  @brief     Null���ǂ�����Ԃ��܂�.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsNull() const { return _type == JsonValueType::Null; }

		/*!**********************************************************************
		*  @brief     False���ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsFalse() const { return _type == JsonValueType::False; }

		/*!**********************************************************************
		*  @brief     True���ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsTrue() const { return _type == JsonValueType::True; }

		/*!**********************************************************************
		*  @brief     Bool���ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsBool() const { return _type == JsonValueType::True || _type ==  JsonValueType::False; }

		/*!**********************************************************************
		*  @brief     Number���ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsNumber() const { return _type == JsonValueType::Number; }

		/*!**********************************************************************
		*  @brief     String���ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsString() const { return HasAnyFlags(_type, JsonValueType::String); }

		/*!**********************************************************************
		*  @brief     Array���ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsArray() const { return HasAnyFlags(_type, JsonValueType::Array); }

		/*!**********************************************************************
		*  @brief     Object���ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsObject() const { return HasAnyFlags(_type, JsonValueType::Object); }

		#pragma endregion Type Check

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		JsonValue() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~JsonValue() = default;

		/*! @brief Type���w�肷��*/
		JsonValue(const JsonValueType type) : _type(type) {};
		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property
		/*! @brief Json�̎��*/
		JsonValueType _type = JsonValueType::Null;

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