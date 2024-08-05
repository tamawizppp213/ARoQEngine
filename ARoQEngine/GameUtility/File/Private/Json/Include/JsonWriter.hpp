//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonWriter.hpp
///  @brief  Json�t�@�C���ɏ������݂��s���N���X.
///  @author toide
///  @date   2024/06/29 17:59:25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_WRITER_HPP
#define JSON_WRITER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "JsonCommon.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUStack.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::file::json
{

	/****************************************************************************
	*				  			   JsonWriter
	****************************************************************************/
	/*  @brief   Json�t�@�C���ɏ������݂��s���N���X.
	*****************************************************************************/
	class JsonWriter : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ���߂�������f�[�^���t�@�C���ɗ������݂܂�.
		*  @param[in] const gu::tstring filePath
		*  @param[in] const bool useAsync
		*  @return    bool
		*************************************************************************/
		bool Flush(const gu::tstring& filePath, const bool useAsync);

		/*!**********************************************************************
		*  @brief     JsonObject�̏������݂��J�n���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteObjectStart();

		/*!**********************************************************************
		*  @brief     JsonObject�̏������݂��J�n���܂� 
		*  @param[in] const gu::tstring& key : �L�[��
		*  @return    void
		*************************************************************************/
		void WriteObjectStart(const gu::tstring& key);

		/*!**********************************************************************
		*  @brief     JsonObject�̏������݂��I�����܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteObjectEnd();

		/*!**********************************************************************
		*  @brief     JsonArray�̏������݂��J�n���܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteArrayStart();

		/*!**********************************************************************
		*  @brief     JsonArray�̏������݂��J�n���܂�.
		*  @param[in] const gu::tstring& �L�[��
		*  @return    void
		*************************************************************************/
		void WriteArrayStart(const gu::tstring& key);

		/*!**********************************************************************
		*  @brief     JsonArray�̏������݂��I�����܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteArrayEnd();

		/*!**********************************************************************
		*  @brief     ������f�[�^���������݂܂�
		*  @param[in] const gu::tstring& key : �L�[��
		*  @param[in] const gu::tstring& value : �o�����[��
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const gu::tstring& value);

		/*!**********************************************************************
		*  @brief     ������f�[�^���������݂܂�
		*  @param[in] const gu::tstring& key : �L�[��
		*  @param[in] const bool value : �o�����[��
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const bool value);

		/*!**********************************************************************
		*  @brief     ������f�[�^���������݂܂�
		*  @param[in] const gu::tstring& key : �L�[��
		*  @param[in] const float value : �o�����[��
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const float value);

		/*!**********************************************************************
		*  @brief     ������f�[�^���������݂܂�
		*  @param[in] const gu::tstring& key : �L�[��
		*  @param[in] const double value : �o�����[��
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const double value);

		/*!**********************************************************************
		*  @brief     ������f�[�^���������݂܂�
		*  @param[in] const gu::tstring& key : �L�[��
		*  @param[in] const gu::int32 value : �o�����[��
		*  @return    void
		*************************************************************************/
		void WriteValue(const gu::tstring& key, const gu::int32 value);

		/*!**********************************************************************
		*  @brief     ������f�[�^���������݂܂�
		*  @param[in] const gu::tstring& key : �L�[��
		*  @param[in] const gu::tstring& value : �l
		*  @return    void
		*************************************************************************/
		void WriteNumberInternal(const gu::tstring& key, const gu::tstring& number);
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     ���ݏ������ݒ���JsonValueType��Ԃ��܂�. 
		*  @param[in] void
		*  @return    JsonValueType
		*************************************************************************/
		JsonValueType GetCurrentValueType() const { return !_stackStates.IsEmpty() ? _stackStates.Top() : JsonValueType::Null; }

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		JsonWriter() = default;



		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		/*!**********************************************************************
		*  @brief     �L�[���̏������݂��s���܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteKey(const gu::tstring& key);

		/*!**********************************************************************
		*  @brief     �G�X�P�[�v�������������ꍇ�̕����񏑂�����
		*  @param[in] const gu::tstring& ������
		*  @return    void
		*************************************************************************/
		void WriteEscapedString(const gu::tstring& value);

		/*!**********************************************************************
		*  @brief     �J���}���K�v�Ȃ�L�q
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void WriteCommaIfNeeded();

		/*!**********************************************************************
		*  @brief     �L�[���K�v�Ȃ��ꍇ�ɂł��I�u�W�F�N�g����������ł��ǂ���
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool EnableWriteWithoutKey() const
		{
			return _stackStates.IsEmpty() || _stackStates.Top() == JsonValueType::Array || _previousToken == JsonToken::Key || _previousToken == JsonToken::Colon;
		}
		#pragma endregion 

		#pragma region Protected Property
		/*! @brief Json�̏�Ԃ�ێ����Ă����ϐ�. (Json������K�w�\�����l���������̂ł�.*/
		gu::Stack<JsonValueType> _stackStates = {};

		/*! @brief ������*/
		gu::tstring _data = SP("");

		/*! @brief �K�w���x��*/
		gu::int32 _indentLevel = 0;

		/*! @brief ��O�ɋL�q����Token*/
		JsonToken _previousToken = JsonToken::None;
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