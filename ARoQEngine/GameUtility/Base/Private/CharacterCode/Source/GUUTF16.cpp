//////////////////////////////////////////////////////////////////////////////////
///  @file   GUUTF16.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/26 2:39:28
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/CharacterCode/Include/GUUTF16.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUUnicodeConverter.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::details::string;
using namespace gu;

namespace
{
	void SwapEndian(uint32* utf32String, const uint64 length)
	{
		for (uint64 i = 0; i < length; ++i)
		{
			uint32& character = utf32String[i];
			character = (character >> 24) |
				((character << 8) & 0x00FF0000) |
				((character >> 8) & 0x0000FF00) |
				(character << 24);
		}
	}

	void SwapEndian(uint16_t* utf16String, const uint64 length)
	{
		for (uint64 i = 0; i < length; ++i)
		{
			uint16& character = utf16String[i];
			character = (character >> 8) | (character << 8);
		}
	}
}

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
bool UTF16::FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �r�b�O�G���f�B�A�������g���G���f�B�A�����𔻒�
	---------------------------------------------------------------------*/
	bool isBigEndian = true;
	if (inputElementSize >= 1)
	{
		if (input[0] == 0xFEFF)
		{
			isBigEndian = true;
		}
		else if (input[0] == 0xFFFE)
		{
			isBigEndian = false;
		}
	}

	Memory::Copy(output, input, inputElementSize * sizeof(uint16));

	/*-------------------------------------------------------------------
	-              ���g�̃G���f�B�A���ƈقȂ�ꍇ�̓G���f�B�A����ϊ�
	---------------------------------------------------------------------*/
	if (isBigEndian != _isBigEndian)
	{
		uint16* utf16String = reinterpret_cast<uint16*>(output);
		::SwapEndian(utf16String, inputElementSize);
	}

	if (result != nullptr)
	{
		result->ConsumedElementCount = inputElementSize;
		result->OutputByteSize       = inputElementSize * sizeof(uint16);
		result->OutputCharacterCount = inputElementSize;

		Check(result->OutputByteSize == outputByteSize);
	}

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
bool UTF16::FromUTF32(const uint32* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �r�b�O�G���f�B�A�������g���G���f�B�A�����𔻒�
	---------------------------------------------------------------------*/
	uint32* utf32String = const_cast<uint32*>(reinterpret_cast<const uint32*>(input));

	bool isBigEndian = true;
	bool isBOM       = false;
	if (inputElementSize >= 1)
	{
		if (utf32String[0] == 0x0000FEFF)
		{
			isBigEndian = true;
			isBOM       = true;
			utf32String++;
		}
		else if (utf32String[0] == 0xFFFE0000)
		{
			isBigEndian = false;
			isBOM       = true;
			utf32String++;
		}
	}

	/*-------------------------------------------------------------------
	-              UTF32�ɕϊ�
	---------------------------------------------------------------------*/
	UnicodeConverter::Options option = {};
	uint16* utf16String = reinterpret_cast<uint16*>(output);

	// output��BOM��ǉ�
	const uint64 bomOffset = isBOM ? 1 : 0;

	if (isBOM)
	{
		utf16String[0] = _isBigEndian ? 0xFEFF : 0xFFFE;
		utf16String++;
	}

	const auto convertResult = UnicodeConverter::ConvertUTF32ToUTF16
	(
		utf32String, inputElementSize - bomOffset, 
		utf16String, outputByteSize / sizeof(uint16) - bomOffset, option
	);

	if (convertResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-              ���g�̃G���f�B�A���ƈقȂ�ꍇ�̓G���f�B�A����ϊ�
	---------------------------------------------------------------------*/
	if (isBigEndian != _isBigEndian)
	{
		::SwapEndian(utf16String, option.ConvertedTargetLength);
	}

	if (result != nullptr)
	{
		result->ConsumedElementCount = inputElementSize;
		result->OutputByteSize       = option.ConvertedTargetLength * sizeof(uint16);
		result->OutputCharacterCount = option.ConvertedTargetLength;
	}

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
bool UTF16::ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �r�b�O�G���f�B�A�������g���G���f�B�A�����𔻒�
	---------------------------------------------------------------------*/
	bool isBigEndian = true;
	if (inputByteSize >= 2)
	{
		const uint16* utf16String = reinterpret_cast<const uint16*>(input);
		if (utf16String[0] == 0xFEFF)
		{
			isBigEndian = true;
		}
		else if (utf16String[0] == 0xFFFE)
		{
			isBigEndian = false;
		}
	}

	Memory::Copy(output, input, inputByteSize);

	/*-------------------------------------------------------------------
	-              ���g�̃G���f�B�A���ƈقȂ�ꍇ�̓G���f�B�A����ϊ�
	---------------------------------------------------------------------*/
	if (isBigEndian != _isBigEndian)
	{
		::SwapEndian(output, inputByteSize / sizeof(uint16));
	}

	if (result != nullptr)
	{
		result->ConsumedByteSize       = inputByteSize;
		result->OutputByteSize         = inputByteSize;
		result->OutputCharacterCount   = inputByteSize / sizeof(uint16);

		Check(result->OutputByteSize == outputElementSize * sizeof(uint16));
	}

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
bool UTF16::ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �r�b�O�G���f�B�A�������g���G���f�B�A�����𔻒�
	---------------------------------------------------------------------*/
	uint16* utf16String = const_cast<uint16*>(reinterpret_cast<const uint16*>(input));

	bool isBigEndian = true;
	bool isBOM       = false;
	if (inputByteSize >= 2)
	{
		if (utf16String[0] == 0xFEFF)
		{
			isBigEndian = true;
			isBOM       = true;
			utf16String++;
		}
		else if (utf16String[0] == 0xFFFE)
		{
			isBigEndian = false;
			isBOM       = true;
			utf16String++;
		}
	}

	/*-------------------------------------------------------------------
	-              UTF32�ɕϊ�
	---------------------------------------------------------------------*/
	// output��BOM��ǉ�
	const uint64 bomOffset = isBOM ? 1 : 0;
	if (isBOM)
	{
		output[0] = isBigEndian ? 0x0000FEFF : 0xFFFE0000;
		output++;
	}

	UnicodeConverter::Options option = {};
	const auto covertResult = UnicodeConverter::ConvertUTF16ToUTF32
	(
		utf16String, inputByteSize / sizeof(uint16) - bomOffset,
		output, outputElementSize - bomOffset, option
	);

	if (covertResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-              ���g�̃G���f�B�A���ƈقȂ�ꍇ�̓G���f�B�A����ϊ�
	---------------------------------------------------------------------*/
	if (isBigEndian != _isBigEndian)
	{
		::SwapEndian(output, option.ConvertedTargetLength);
	}

	if (result != nullptr)
	{
		result->ConsumedByteSize       = inputByteSize;
		result->OutputByteSize         = (option.ConvertedTargetLength + bomOffset) * sizeof(uint32);
		result->OutputCharacterCount   = (option.ConvertedTargetLength + bomOffset);
	}

	return true;
}

/*!**********************************************************************
*  @brief     �����񒷂��擾���܂�
*  @param[in] const void* buffer ������
*  @param[in] const uint64 bufferSize ������̃o�C�g��
*  @return    const uint64 ������
*************************************************************************/
uint64 UTF16::GetCharacterLength(const void* buffer, const uint64 bufferSize) const
{
	uint64 count = 0;
	const auto result = UnicodeConverter::GetUTF16CharacterLength((const uint16*)buffer, bufferSize / sizeof(uint16), count, true);

	return result == UnicodeConvertResult::Success ? count : 0;
}

/*!**********************************************************************
*  @brief     �w�肳�ꂽ�o�b�t�@�̐擪���}���`�o�C�g�R�[�h��T���Q�[�g�y�A�̐�s�����ł���ꍇ�A�ǉ��œǂނׂ������񒷂���Ԃ��܂��B
*  @param[in] const void* buffer ������
*  @param[in] const uint64 bufferSize ������̃o�C�g��
*  @return    const tchar* �����R�[�h��
*************************************************************************/
uint64 UTF16::GetReadExtraLength(const void* buffer, const uint64 bufferSize) const
{
	bool surrogatePair = false;

	const auto result = UnicodeConverter::CheckUTF16Surrogate((const uint16*)buffer, ((const uint16*)buffer) + bufferSize / sizeof(uint16), true, &surrogatePair);
	if(result != UnicodeConvertResult::Success)
	{
		return 0;
	}

	return surrogatePair ? 1 : 0;
}
#pragma endregion Public Function