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
		*  @brief     Json�t�@�C�����Ō�܂œǂݍ��݂܂�. 
		*  @param[in] const gu::tstring& filePath : �ǂݍ��݂���Json�t�@�C���̃p�X
		*  @param[in] const bool �񓯊��ǂݍ��݂��s����
		*  @return    bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool Read(const gu::tstring& filePath, const bool useAsync);

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
		/*!**********************************************************************
		*  @brief      ���̃g�[�N����������܂Ńt�@�C���|�C���^��i�߂܂�. 
		*  @param[in]  const tstring& json�t�@�C���𕶎��񉻂�������
		*  @param[out] JsonToken token : ���̃g�[�N��
		*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool ParseNextToken(const tstring& json, JsonToken& token);

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
		#pragma endregion 

		#pragma region Private Property 
		
		/*! @brief Parse���s���Ă���C���f�b�N�X*/
		gu::uint64 _parseIndex = 0;

		/*! @brief json�̍s��*/
		gu::uint32  _lineCount = 0;

		/*! @brief �e�s�̕�����*/
		gu::uint32 _characterCount = 0;

		#pragma endregion 

	};
}


#endif