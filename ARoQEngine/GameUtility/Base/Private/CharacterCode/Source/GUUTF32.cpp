//////////////////////////////////////////////////////////////////////////////////
///  @file   GUUTF32.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/26 2:39:38
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/CharacterCode/Include/GUUTF32.hpp"
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
bool UTF32::FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �r�b�O�G���f�B�A�������g���G���f�B�A�����𔻒�
	---------------------------------------------------------------------*/
	bool isBigEndian = true;
	bool isBOM       = false;
	if (inputElementSize >= 1)
	{
		if (input[0] == 0xFEFF)
		{
			isBigEndian = true;
			isBOM       = true;
			input++;
		}
		else if (input[0] == 0xFFFE)
		{
			isBigEndian = false;
			isBOM       = true;
			input++;
		}
	}

	/*-------------------------------------------------------------------
	-              UTF16�ɕϊ�
	---------------------------------------------------------------------*/
	UnicodeConverter::Options option = {};
	uint32* utf32String = reinterpret_cast<uint32*>(output);

	// output��BOM��ǉ�
	const uint64 bomOffset = isBOM ? 1 : 0;

	if (isBOM)
	{
		utf32String[0] = _isBigEndian ? 0x0000FEFF : 0xFFFE0000;
		utf32String++;
	}

	const auto convertResult = UnicodeConverter::ConvertUTF16ToUTF32(
		                                         input, inputElementSize - bomOffset, 
		                                         utf32String, outputByteSize / sizeof(uint32) - bomOffset, option);

	if (convertResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-              ���g�̃G���f�B�A���ƈقȂ�ꍇ�̓G���f�B�A����ϊ�
	---------------------------------------------------------------------*/
	if (isBigEndian != _isBigEndian)
	{
		::SwapEndian(utf32String, option.ConvertedTargetLength);
	}

	if (result != nullptr)
	{
		result->ConsumedElementCount = inputElementSize;
		result->OutputByteSize       = (option.ConvertedTargetLength + bomOffset) * sizeof(uint32);
		result->OutputCharacterCount = option.ConvertedTargetLength + bomOffset;
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
bool UTF32::FromUTF32(const uint32* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �r�b�O�G���f�B�A�������g���G���f�B�A�����𔻒�
	---------------------------------------------------------------------*/
	bool isBigEndian = true;
	if (inputElementSize >= 1)
	{
		if (input[0] == 0x0000FEFF)
		{
			isBigEndian = true;
		}
		else if (input[0] == 0xFFFE0000)
		{
			isBigEndian = false;
		}
	}

	Memory::Copy(output, input, inputElementSize * sizeof(uint32));

	/*-------------------------------------------------------------------
	-              ���g�̃G���f�B�A���ƈقȂ�ꍇ�̓G���f�B�A����ϊ�
	---------------------------------------------------------------------*/
	if (isBigEndian != _isBigEndian)
	{
		uint32* utf32String = reinterpret_cast<uint32*>(output);
		::SwapEndian(utf32String, inputElementSize);
	}

	if (result != nullptr)
	{
		result->ConsumedElementCount = inputElementSize;
		result->OutputByteSize       = inputElementSize * sizeof(uint32);
		result->OutputCharacterCount = inputElementSize;

		Check(result->OutputByteSize == outputByteSize);
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
bool UTF32::ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �r�b�O�G���f�B�A�������g���G���f�B�A�����𔻒�
	---------------------------------------------------------------------*/
	uint32* utf32String = const_cast<uint32*>(reinterpret_cast<const uint32*>(input));

	bool isBigEndian = true;
	bool isBOM       = false;
	if (inputByteSize >= 4)
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
	-              UTF16�ɕϊ�
	---------------------------------------------------------------------*/
	// output��BOM��ǉ�
	const uint64 bomOffset = isBOM ? 1 : 0;
	if (isBOM)
	{
		output[0] = isBigEndian ? 0xFEFF : 0xFFFE;
		output++;
	}

	UnicodeConverter::Options option = {};
	const auto convertResult = UnicodeConverter::ConvertUTF32ToUTF16
	(
		utf32String, (inputByteSize / sizeof(uint32)) - bomOffset , 
		output, outputElementSize - bomOffset, option
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
		::SwapEndian(output, option.ConvertedTargetLength);
	}

	if (result != nullptr)
	{
		result->ConsumedByteSize     = inputByteSize;
		result->OutputByteSize       = (option.ConvertedTargetLength + bomOffset) * sizeof(uint16);
		result->OutputCharacterCount = option.ConvertedTargetLength + bomOffset;
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
bool UTF32::ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �r�b�O�G���f�B�A�������g���G���f�B�A�����𔻒�
	---------------------------------------------------------------------*/
	bool isBigEndian = true;
	if (inputByteSize >= 4)
	{
		const uint32* utf32String = reinterpret_cast<const uint32*>(input);
		if (utf32String[0] == 0x0000FEFF)
		{
			isBigEndian = true;
		}
		else if (utf32String[0] == 0xFFFE0000)
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
		::SwapEndian(output, inputByteSize / sizeof(uint32));
	}

	if (result != nullptr)
	{
		result->ConsumedByteSize       = inputByteSize;
		result->OutputByteSize         = inputByteSize;
		result->OutputCharacterCount   = inputByteSize / sizeof(uint32);

		Check(result->OutputByteSize == outputElementSize * sizeof(uint32));
	}

	return true;
}
#pragma endregion Public Function