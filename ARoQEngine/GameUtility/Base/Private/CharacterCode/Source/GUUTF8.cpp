//////////////////////////////////////////////////////////////////////////////////
///  @file   GUUTF8.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/25 21:29:02
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/CharacterCode/Include/GUUTF8.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUUnicodeConverter.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::details::string;
using namespace gu;

#pragma warning(disable: 26819) // ���߂̂��ĂȂ��t�H�[���X���[������܂�

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     UTF16����w�肵�������R�[�h�ɕϊ����܂�
*  @param[in] const uint16* input �ϊ����̕�����
*  @param[in] const uint64 inputElementSize �ϊ����̕�����̗v�f��
*  @param[in] uint8* output �ϊ���̕�����
*  @param[in] const uint64 outputByteSize �ϊ���̕�����̃o�C�g��
*  @param[in] CharacterDecodeResult* result �ϊ�����
*  @return    const tchar* �����R�[�h��
*************************************************************************/
bool UTF8::FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	if (_isBOM)
	{
		// BOM����������
		output[0] = 0xEF;
		output[1] = 0xBB;
		output[2] = 0xBF;
	}

	/*-------------------------------------------------------------------
	-              UTF8�ɕϊ����܂�
	---------------------------------------------------------------------*/
	UnicodeConverter::Options option = {};
	option.ReplacementChar = '?';

	const auto convertResult = UnicodeConverter::ConvertUTF16ToUTF8(input, inputElementSize, output + (_isBOM ? 3 : 0), outputByteSize, option);
	if (convertResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	result->ConsumedElementCount = inputElementSize;
	result->OutputByteSize       = option.ConvertedTargetLength;
	result->OutputCharacterCount = option.ConvertedTargetLength;
	return true;
}

/*!**********************************************************************
*  @brief     UTF32����w�肵�������R�[�h�ɕϊ����܂�
*  @param[in] const uint32* input �ϊ����̕�����
*  @param[in] const uint64 inputElementSize �ϊ����̕�����̗v�f��
*  @param[in] uint8* output �ϊ���̕�����
*  @param[in] const uint64 outputByteSize �ϊ���̕�����̃o�C�g��
*  @param[in] CharacterDecodeResult* result �ϊ�����
*  @return    const tchar* �����R�[�h��
*************************************************************************/
bool UTF8::FromUTF32(const uint32* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	if (_isBOM)
	{
		// BOM����������
		output[0] = 0xEF;
		output[1] = 0xBB;
		output[2] = 0xBF;
	}

	/*-------------------------------------------------------------------
	-              UTF8�ɕϊ����܂�
	---------------------------------------------------------------------*/
	UnicodeConverter::Options option = {};
	option.ReplacementChar = '?';

	const auto convertResult = UnicodeConverter::ConvertUTF32ToUTF8(input, inputElementSize, output + (_isBOM ? 3 : 0), outputByteSize, option);
	if (convertResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	result->ConsumedElementCount = inputElementSize;
	result->OutputByteSize       = option.ConvertedTargetLength;
	result->OutputCharacterCount = option.ConvertedTargetLength;
	return true;
}

/*!**********************************************************************
*  @brief     �w�肵�������R�[�h����UTF16�ɕϊ����܂�
*  @param[in] const uint8* input �ϊ����̕�����
*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
*  @param[in] uint16* output �ϊ���̕�����
*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
*  @param[in] CharacterDecodeResult* result �ϊ�����
*  @return    const tchar* �����R�[�h��
*************************************************************************/
bool UTF8::ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	uint64 remainByteSize = inputByteSize;

	// BOM�̗L�����m�F
	if (_isBOM)
	{
		if (!IsValidBOM(input)) 
		{
			return false; 
		}

		input          += 3;
		remainByteSize -= 3;
	}

	/*-------------------------------------------------------------------
	-              UTF32�ɕϊ����܂�
	---------------------------------------------------------------------*/
	UnicodeConverter::Options option = {};
	option.ReplacementChar = '?';

	const auto convertResult = UnicodeConverter::ConvertUTF8ToUTF16(input, remainByteSize, output, outputElementSize, option);
	if (convertResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	result->ConsumedByteSize     = inputByteSize;
	result->OutputByteSize       = option.ConvertedTargetLength * sizeof(uint32);
	result->OutputCharacterCount = option.ConvertedTargetLength;
	return true;
}

/*!**********************************************************************
*  @brief     �w�肵�������R�[�h����UTF32�ɕϊ����܂�
*  @param[in] const uint8* input �ϊ����̕�����
*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
*  @param[in] uint32* output �ϊ���̕�����
*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
*  @param[in] CharacterDecodeResult* result �ϊ�����
*  @return    const tchar* �����R�[�h��
*************************************************************************/
bool UTF8::ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	uint64 remainByteSize = inputByteSize;

	// BOM�̗L�����m�F
	if (_isBOM)
	{
		if (!IsValidBOM(input)) 
		{
			return false; 
		}

		input          += 3;
		remainByteSize -= 3;
	}

	/*-------------------------------------------------------------------
	-              UTF32�ɕϊ����܂�
	---------------------------------------------------------------------*/
	UnicodeConverter::Options option = {};
	option.ReplacementChar = '?';

	const auto convertResult = UnicodeConverter::ConvertUTF8ToUTF32(input, remainByteSize, output, outputElementSize, option);
	if (convertResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	result->ConsumedByteSize     = inputByteSize;
	result->OutputByteSize       = option.ConvertedTargetLength * sizeof(uint32);
	result->OutputCharacterCount = option.ConvertedTargetLength;
	return true;
}

/*!**********************************************************************
*  @brief     �����񒷂��擾���܂�
*  @param[in] void
*  @return    const uint64 ������
*************************************************************************/
uint64 UTF8::GetCharacterLength(const void* buffer, const uint64 bufferSize) const
{
	uint64 length = 0;
	const auto result = UnicodeConverter::GetUTF8CharacterLength((const uint8*)buffer, bufferSize, length, true);

	return result == UnicodeConvertResult::Success ? length : 0;
}

/*!**********************************************************************
*  @brief     �w�肳�ꂽ�o�b�t�@�̐擪���}���`�o�C�g�R�[�h��T���Q�[�g�y�A�̐�s�����ł���ꍇ�A�ǉ��œǂނׂ������񒷂���Ԃ��܂��B
*  @param[in] const void* buffer ������
*  @param[in] const uint64 bufferSize ������̃o�C�g��
*  @return    const tchar* �����R�[�h��
*************************************************************************/
uint64 UTF8::GetLeadExtraLength(const void* buffer, const uint64 bufferSize) const
{
	uint64 length = 0;
	const auto result = UnicodeConverter::GetUTF8TrailingBytes((const uint8*)buffer, (const uint8*)buffer + bufferSize, length, true);

	return result == UnicodeConvertResult::Success ? length : 0;

}
#pragma endregion Public Function

#pragma region Private Function
/*!**********************************************************************
*  @brief     �L����BOM�̔���
*  @param[in] const uint8* ���͕�����
*  @return    bool
*************************************************************************/
bool UTF8::IsValidBOM(const uint8* input) const
{
	if (!_isBOM) { return false; }

	return input[0] == 0xEF && input[1] == 0xBB && input[2] == 0xBF;
}
#pragma endregion Private Function