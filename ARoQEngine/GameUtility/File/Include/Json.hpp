//////////////////////////////////////////////////////////////////////////////////
///  @file   Json.hpp
///  @brief  Json�̃V���A���C�Y��f�V���A���C�Y���s���N���X
///  @author toide
///  @date   2024/06/29 14:23:49
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_HPP
#define JSON_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonCommon.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	class JsonReader;
	class JsonWriter;
	struct JsonValue;
	struct JsonObject;
	struct JsonArray;

	/****************************************************************************
	*				  			   Json
	****************************************************************************/
	/*  @brief   Json�̃V���A���C�Y��f�V���A���C�Y���s���N���X
	*****************************************************************************/
	class JsonSerializer
	{
	private:
		/****************************************************************************
		*				  			  State
		****************************************************************************/
		/*  @brief   Json��͂��邽�߂̃f�[�^�\��
		*****************************************************************************/
		struct State
		{
			/*! @brief Json�̒l�̎��*/
			JsonValueType Type = JsonValueType::Null;

			/*! @brief �L�[��*/
			gu::tstring Key = SP("");

			/*! @brief Json�z��*/
			gu::DynamicArray<gu::SharedPointer<JsonValue>> Array = {};

			/*! @brief Json�I�u�W�F�N�g*/
			gu::SharedPointer<JsonObject> Object = nullptr;
		};

		/****************************************************************************
		*				  			 Element
		****************************************************************************/
		/*  @brief   Json�̗v�f
		*****************************************************************************/
		struct Element
		{
			/*! @brief �L�[��*/
			gu::tstring Key = SP("");

			/*! @brief �l*/
			gu::SharedPointer<JsonValue> Value = nullptr;

			/*! @brief �v�f�����s���ꂽ��*/
			bool IsExecuted = false;

			/*! @brief �v�f���L�[/�l�̃y�A�ł��邩�ǂ���*/
			bool IsKeyValue = false;

			Element(const gu::SharedPointer<JsonValue>& value) : Value(value) {}
			Element(const gu::SharedPointer<JsonObject>& object);
			Element(const gu::DynamicArray<gu::SharedPointer<JsonValue>>& array);
			Element(const gu::tstring& key, const gu::SharedPointer<JsonValue>& value) : Key(key), Value(value), IsKeyValue(true) {}
		};

	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     �V���A���C�Y���s���܂�.�@�V���A���C�Y�Ƃ̓I�u�W�F�N�g��Json�`���̕�����ɕϊ����邱�Ƃł�.
		*  @param[in] const gu::SharedPointer<JsonWriter>& json���������ރN���X
		*  @param[in] const gu::SharedPointer<JsonValue>& �V���A���C�Y�������I�u�W�F�N�g
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::SharedPointer<JsonValue>& value)
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     �V���A���C�Y���s���܂�.�@�V���A���C�Y�Ƃ̓I�u�W�F�N�g��Json�`���̕�����ɕϊ����邱�Ƃł�.
		*  @param[in] const gu::SharedPointer<JsonWriter>& json���������ރN���X
		*  @param[in] const gu::SharedPointer<JsonObject>& �V���A���C�Y�������I�u�W�F�N�g
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::SharedPointer<JsonObject>& object)
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     �V���A���C�Y���s���܂�.�@�V���A���C�Y�Ƃ̓I�u�W�F�N�g��Json�`���̕�����ɕϊ����邱�Ƃł�.
		*  @param[in] const gu::SharedPointer<JsonWriter>& json���������ރN���X
		*  @param[in] const gu::SharedPointer<JsonObject>& �V���A���C�Y�������I�u�W�F�N�g
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::DynamicArray<gu::SharedPointer<JsonValue>>& object)
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     �f�V���A���C�Y���s���܂�.�@�f�V���A���C�Y�Ƃ�Json�`���̕�������I�u�W�F�N�g�ɕϊ����邱�Ƃł�.
		*  @param[in] const gu::SharedPointer<JsonReader>& json��ǂݍ��ރN���X
		*  @param[in] const gu::SharedPointer<JsonValue>& �f�V���A���C�Y�������I�u�W�F�N�g
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool Deserialize(const gu::SharedPointer<JsonReader>& reader, const gu::SharedPointer<JsonValue>& value)
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     �f�V���A���C�Y���s���܂�.�@�f�V���A���C�Y�Ƃ�Json�`���̕�������I�u�W�F�N�g�ɕϊ����邱�Ƃł�.
		*  @param[in] const gu::SharedPointer<JsonReader>& json��ǂݍ��ރN���X
		*  @param[in] const gu::SharedPointer<JsonObject>& �f�V���A���C�Y�������I�u�W�F�N�g
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool Deserialize(const gu::SharedPointer<JsonReader>& reader, const gu::SharedPointer<JsonObject>& object)
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     �f�V���A���C�Y���s���܂�.�@�f�V���A���C�Y�Ƃ�Json�`���̕�������I�u�W�F�N�g�ɕϊ����邱�Ƃł�.
		*  @param[in] const gu::SharedPointer<JsonReader>& json��ǂݍ��ރN���X
		*  @param[in] const gu::DynamicArray<gu::SharedPointer<JsonValue>>& �f�V���A���C�Y�������I�u�W�F�N�g
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool Deserialize(const gu::SharedPointer<JsonReader>& reader, const gu::DynamicArray<gu::SharedPointer<JsonValue>>& object)
		{
			return false;
		}


		#pragma endregion 

		#pragma region Public Property

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

		/*!**********************************************************************
		*  @brief     �V���A���C�Y���s���܂�.�@�V���A���C�Y�Ƃ̓I�u�W�F�N�g��Json�`���̕�����ɕϊ����邱�Ƃł�.
		*  @param[in] const gu::SharedPointer<JsonWriter>& json���������ރN���X
		*  @param[in] const gu::SharedPointer<JsonObject>& �V���A���C�Y�������I�u�W�F�N�g
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::SharedPointer<Element>& startElement);


		/*!**********************************************************************
		*  @brief     �f�V���A���C�Y���s���܂�.�@�f�V���A���C�Y�Ƃ�Json�`���̕�������I�u�W�F�N�g�ɕϊ����邱�Ƃł�.
		*  @param[in] const gu::SharedPointer<JsonReader>& json��ǂݍ��ރN���X
		*  @param[in] const gu::DynamicArray<gu::SharedPointer<JsonValue>>& �f�V���A���C�Y�������I�u�W�F�N�g
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool Deserialize(const gu::SharedPointer<JsonReader>& reader, State& state);

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};

}
#endif