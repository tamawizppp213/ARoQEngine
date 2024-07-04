//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonReader.hpp
///  @brief  temp
///  @author toide
///  @date   2024/06/13 23:19:53
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_READER_HPP
#define JSON_READER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "JsonCommon.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace platform::core::file
{
	class IFileHandle;
}


//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::file::json
{
	struct JsonValue;
	struct JsonObject;

	/****************************************************************************
	*				  			   JsonReader
	****************************************************************************/
	/* @brief Json�t�@�C����ǂݍ��݂܂�.
	*         �w�肵�������R�[�h��UTF-8��z�肵�Ă��܂�.
	*****************************************************************************/
	class JsonReader
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ���̒l��ǂݍ��݂܂�
		*  @param[out]JsonNotation&  
		*  @return    bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool ReadNext(JsonNotation& notation);

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		JsonReader(const gu::tstring& filePath, const bool useAsync = false)
		{
			Initialize(filePath, useAsync);
		}
		
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     Json�t�@�C�����Ō�܂œǂݍ��݂܂�.
		*  @param[in] const gu::tstring& filePath : �ǂݍ��݂���Json�t�@�C���̃p�X
		*  @param[in] const bool �񓯊��ǂݍ��݂��s����
		*  @return    bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool Initialize(const gu::tstring& filePath, const bool useAsync);

		/*!**********************************************************************
		*  @brief      ���̃g�[�N����������܂Ńt�@�C���|�C���^��i�߂܂�. 
		*  @param[in]  const tstring& json�t�@�C���𕶎��񉻂�������
		*  @param[out] JsonToken token : ���̃g�[�N��
		*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool ParseNextToken(const tstring& json, JsonToken& token);

		/*!**********************************************************************
		*  @brief      ���l�^��ǂݎ��܂�. 
		*  @param[in]  const tchar ������
		*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool ParseNumberToken(const tstring& json);

		#pragma endregion 

		#pragma region Protected Property

		/*! @brief �t�@�C���ǂݍ���*/
		gu::SharedPointer<platform::core::file::IFileHandle> _fileSystem = nullptr;

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function
		/*!**********************************************************************
		*  @brief      �󔒕������ǂ�����Ԃ��܂�
		*  @param[out] tchar ����
		*  @return     bool
		*************************************************************************/
		__forceinline bool IsWhiteSpace(const tchar character)
		{
			return character == SP(' ') || character == SP('\t') || character == SP('\n') || character == SP('\r');
		}

		/*!**********************************************************************
		*  @brief      Json�̐��l�^�̔���
		*  @param[out] const tchar character
		*  @return     bool
		*************************************************************************/
		__forceinline bool IsNumber(const tchar character)
		{
			return (SP('0') <= character && character <= SP('9')) || character == SP('-') || character == SP('.')
				|| character == SP('+') || character == SP('e') || character == SP('E');
		}

		#pragma endregion 

		#pragma region Private Property 
		
		/*! @brief Parse���s���Ă���C���f�b�N�X*/
		gu::uint64 _parseIndex = 0;

		/*! @brief �L�[��*/
		gu::tstring _key = SP("");

		/*! @brief ���f�[�^*/
		gu::tstring _rawData = SP("");

		/*! @brief ���݂�Token*/
		JsonToken _token = JsonToken::None;

		/*! @brief */

		#pragma endregion 

	};
}


#endif