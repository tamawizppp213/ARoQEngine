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
#include "GameUtility/Container/Include/GUStack.hpp"

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
		*  @return    bool : �ǂݍ��ݐ����������ł͂Ȃ�, �܂��㑱�̓ǂݍ��݂����݂��邩
		*************************************************************************/
		bool ReadNext(JsonNotation& notation);

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     Json�̃L�[�����擾���܂�. 
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline const tstring& GetKey() const { return _key; }

		/*!**********************************************************************
		*  @brief     Json��Value�Ƃ���, String���擾���܂�.
		*  @param[in] void
		*  @return    const tstring
		*************************************************************************/
		__forceinline const tstring& GetValueAsString() const
		{
			Check(_token == JsonToken::String);
			return _stringValue;
		}

		/*!**********************************************************************
		*  @brief     Json��Value�Ƃ���, Number���擾���܂�.
		*  @param[in] void
		*  @return    double
		*************************************************************************/
		__forceinline double GetValueAsNumber() const
		{
			Check(_token == JsonToken::Number);
			return _numberValue;
		}

		/*!**********************************************************************
		*  @brief     Json��Value�Ƃ���, Number��������擾���܂�.
		*  @param[in] void
		*  @return    coknst tstring
		*************************************************************************/
		__forceinline const tstring& GetValueAsNumberString() const
		{
			Check(_token == JsonToken::String);
			return _stringValue;
		}

		/*!**********************************************************************
		*  @brief     Json��Value�Ƃ���, Number��������擾���܂�.
		*  @param[in] void
		*  @return    coknst tstring
		*************************************************************************/
		__forceinline bool GetValueAsBoolean() const
		{
			Check(_token == JsonToken::True || _token == JsonToken::False);
			return _boolValue;
		}


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
		*  @brief     JsonToken���擾���܂�
		*  @param[out]JsonToken&
		*  @return    bool
		*************************************************************************/
		bool ReadStart(JsonToken& token);

		/*!**********************************************************************
		*  @brief     JsonObject���擾���܂�
		*  @param[out]JsonToken&
		*  @return    bool
		*************************************************************************/
		bool ReadNextObjectValue(JsonToken& token);

		/*!**********************************************************************
		*  @brief     JsonArray���擾���܂�
		*  @param[out]JsonToken&
		*  @return    bool
		*************************************************************************/
		bool ReadNextArrayValue(JsonToken& token);

		/*!**********************************************************************
		*  @brief      ���̃g�[�N����������܂Ńt�@�C���|�C���^��i�߂܂�. 
		*  @param[out] JsonToken token : ���̃g�[�N��
		*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool ParseNextToken(JsonToken& token);

		/*!**********************************************************************
		*  @brief      ���l�^��ǂݎ��܂�. 
		*  @param[in]  void
		*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool ParseNumberToken();

		/*!**********************************************************************
		*  @brief      ������^��ǂݎ��܂�.
		*  @param[in]  void
		*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool ParseStringToken();

		/*!**********************************************************************
		*  @brief      �󔒕����񂪑����܂Ői�݂܂�
		*  @param[in]  const tchar ������
		*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		bool ParseWhiteSpace();

		/*!**********************************************************************
		*  @brief      �Ō�܂ŕ������ǂݍ��񂾂�
		*  @param[in]  void
		*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
		*************************************************************************/
		__forceinline bool ReadAtEnd() const
		{
			return (_parseIndex >= _rawData.Size() && _parseIndex != static_cast<gu::uint64>(-1));
		}

		/*!**********************************************************************
		*  @brief      ���̕����ɐi�߂邩���m�F��, ���Ȃ����Index�����ɐi�߂�.
		*  @param[in]  void
		*  @return     bool
		*************************************************************************/
		__forceinline bool TryToNextIndex()
		{
			const auto index = _parseIndex + 1;
			const auto enable = index < _rawData.Size();
			if (enable)
			{
				_parseIndex = index;
			}
			return enable;
		}

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
		gu::uint64 _parseIndex = static_cast<gu::uint64>(-1);

		/*! @brief �L�[��*/
		gu::tstring _key = SP("");

		/*! @brief ���f�[�^*/
		gu::tstring _rawData = SP("");

		/*! @brief Json�̏�Ԃ�ێ����Ă����ϐ�. (Json������K�w�\�����l���������̂ł�.)*/
		gu::Stack<JsonValueType> _parseStates = {};

		/*! @brief ���݂�Token*/
		JsonToken _token = JsonToken::None;

		/*! @brief RootObject���ǂݍ��ݏI���������ǂ���*/
		bool _hasReadRootObject = false;

		/*! @brief �u�[���l*/
		bool _boolValue = false;

		/*! @brief ���l*/
		double _numberValue = 0.0;

		/*! @brief ������*/
		gu::tstring _stringValue = SP("");
		#pragma endregion 

	};
}


#endif