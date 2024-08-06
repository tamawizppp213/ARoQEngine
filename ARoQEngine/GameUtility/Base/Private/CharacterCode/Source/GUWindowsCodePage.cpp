//////////////////////////////////////////////////////////////////////////////////
///  @file   GUWindowsCodePage.cpp
///  @brief  Windows�̃R�[�h�y�[�W�������N���X
///  @author toide
///  @date   2024/05/27 23:22:44
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/CharacterCode/Include/GUWindowsCodePage.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUUnicodeConverter.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#if defined(_WIN32) || defined(_WIN64)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;
using namespace gu::details::string;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
WindowsCodePage::WindowsCodePage(const uint32 codePage)
{
	/*-------------------------------------------------------------------
	-            �R�[�h�y�[�W���擾���܂�
	---------------------------------------------------------------------*/
	if (!::GetCPInfoEx(codePage, 0, &_codePageInfo))
	{
		return;
	}
}


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
bool WindowsCodePage::FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-          �����ݒ�
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	result->ConsumedElementCount = 0;
	result->OutputByteSize       = 0;
	result->OutputCharacterCount = 0;

	if (outputByteSize > 0)
	{
		output[0] = '\0';
	}

	// ���͂�0�����̏ꍇ�͕ϊ����s��Ȃ�
	// WideCharToMultiByte�̖߂�l���G���[�Ȃ̂������Ȃ̂���������Ȃ��Ȃ�
	if (inputElementSize == 0)
	{
		return true;
	}

	/*-------------------------------------------------------------------
	-          CodePage���̕ϊ�
	---------------------------------------------------------------------*/
	const auto convertedByteCount = ::WideCharToMultiByte
	(
		_codePageInfo.CodePage,                // �R�[�h�y�[�W
		0,                                     // �t���O
		reinterpret_cast<const wchar*>(input), // �ϊ����̕�����
		static_cast<int32>(inputElementSize),  // �ϊ����̕�����̗v�f��
		reinterpret_cast<char*>(output), 	   // �ϊ���̕�����
		static_cast<int32>(outputByteSize),    // �ϊ���̕�����̃o�C�g��
		nullptr,                               // �f�t�H���g�����̐ݒ� 
		FALSE 						           // �f�t�H���g�����̐ݒ�
	);

	// �ϊ����s
	if(convertedByteCount <= 0)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-         UnicodeConverter���g���ĕ������J�E���g
	---------------------------------------------------------------------*/
	uint64 characterCount = 0;
	const auto utf16CharacterLengthResult = UnicodeConverter::GetUTF16CharacterLength(input, inputElementSize, characterCount, true);

	if (utf16CharacterLengthResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	// �ϊ����ʂ�ݒ�
	result->ConsumedElementCount = inputElementSize;
	result->OutputByteSize       = convertedByteCount;
	result->OutputCharacterCount = characterCount;

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
bool WindowsCodePage::FromUTF32(const uint32* input, const uint64 inputElementSize, uint8* output,const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-          �����ݒ�
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	result->ConsumedElementCount = 0;
	result->OutputByteSize       = 0;
	result->OutputCharacterCount = 0;

	if (outputByteSize > 0)
	{
		output[0] = '\0';
	}

	// ���͂�0�����̏ꍇ�͕ϊ����s��Ȃ�
	// WideCharToMultiByte�̖߂�l���G���[�Ȃ̂������Ȃ̂���������Ȃ��Ȃ�
	if (inputElementSize == 0)
	{
		return true;
	}

	/*-------------------------------------------------------------------
	-          Iterator���g���ĕ������ϊ�
	---------------------------------------------------------------------*/
	const uint32* inputPosition = input;
	const uint32* inputEnd      = input + inputElementSize;
	char* outputPosition        = reinterpret_cast<char*>(output);
	char* outputEnd             = reinterpret_cast<char*>(output + outputByteSize);

	const uint64 wordCount = 256;
	uint16 words[wordCount + 1] = {};

	UnicodeConverter::Options options = {};

	while (inputPosition < inputEnd)
	{
		/*-------------------------------------------------------------------
		-         UTF32����UTF16�ɕϊ� (WideCharToMultiByte�����s���邽��)
		---------------------------------------------------------------------*/
		const uint64 convertSize = (inputEnd - inputPosition) > wordCount / 2 ? wordCount / 2 : (inputEnd - inputPosition);
		if (UnicodeConverter::ConvertUTF32ToUTF16(inputPosition, convertSize, words, wordCount, options) != UnicodeConvertResult::Success)
		{
			return false;
		}

		/*-------------------------------------------------------------------
		-         MultiByte�ɕϊ�
		---------------------------------------------------------------------*/
		const auto convertedByteCount = ::WideCharToMultiByte
		(
			_codePageInfo.CodePage,                            // �R�[�h�y�[�W
			0,                                                 // �t���O
			reinterpret_cast<const wchar*>(words),             // �ϊ����̕�����
			static_cast<int32>(options.ConvertedTargetLength), // �ϊ����̕�����̗v�f��
			outputPosition,                                    // �ϊ���̕�����
			static_cast<int32>(outputEnd - outputPosition),    // �ϊ���̕�����̃o�C�g��
			nullptr,                                           // �f�t�H���g�����̐ݒ� 
			FALSE 						                       // �f�t�H���g�����̐ݒ�
		);

		if(convertedByteCount <= 0)
		{
			return false;
		}

		inputPosition                += options.ConvertedSourceLength;
		outputPosition               += convertedByteCount;
		result->OutputCharacterCount += options.CharacterCount;
	}

	// �ϊ����ʂ�ݒ�
	result->ConsumedElementCount = inputElementSize;
	result->OutputByteSize       = outputPosition - reinterpret_cast<char*>(output);

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
bool WindowsCodePage::ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-          �����ݒ�
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	result->ConsumedByteSize     = 0;
	result->OutputByteSize       = 0;
	result->OutputCharacterCount = 0;

	if (outputElementSize > 0)
	{
		output[0] = '\0';
	}

	// ���͂�0�����̏ꍇ�͕ϊ����s��Ȃ�
	// WideCharToMultiByte�̖߂�l���G���[�Ȃ̂������Ȃ̂���������Ȃ��Ȃ�
	if (inputByteSize == 0)
	{
		return true;
	}

	/*-------------------------------------------------------------------
	-          �ϊ�
	---------------------------------------------------------------------*/
	const auto convertedWideCount = ::MultiByteToWideChar
	(
		_codePageInfo.CodePage,                // �R�[�h�y�[�W
		MB_ERR_INVALID_CHARS,                  // �t���O
		reinterpret_cast<const char*>(input),  // �ϊ����̕�����
		static_cast<int32>(inputByteSize),     // �ϊ����̕�����̃o�C�g��
		reinterpret_cast<wchar*>(output),      // �ϊ���̕�����
		static_cast<int32>(outputElementSize)  // �ϊ���̕�����̗v�f��
	);

	// �ϊ����s
	if(convertedWideCount <= 0)
	{
		return false;
	}

	// MultiByteToWideChar�ł͕������̃J�E���g�͂ł��Ȃ�����, UnicodeConverter���g���ĕ������J�E���g
	uint64 characterCount = 0;

	const auto utf16CharacterLengthResult = UnicodeConverter::GetUTF16CharacterLength(output, convertedWideCount, characterCount, true);
	if(utf16CharacterLengthResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	// �ϊ����ʂ�ݒ�
	result->ConsumedByteSize     = inputByteSize;
	result->OutputByteSize       = convertedWideCount * sizeof(wchar);
	result->OutputCharacterCount = characterCount;

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
bool WindowsCodePage::ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-          �����ݒ�
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	result->ConsumedByteSize     = 0;
	result->OutputByteSize       = 0;
	result->OutputCharacterCount = 0;

	if (outputElementSize > 0)
	{
		output[0] = '\0';
	}

	// ���͂�0�����̏ꍇ�͕ϊ����s��Ȃ�
	if (inputByteSize == 0)
	{
		return true;
	}

	/*-------------------------------------------------------------------
	-          UTF16�ɕϊ�
	---------------------------------------------------------------------*/
	gu::DynamicArray<wchar> utf16Buffer(outputElementSize);
	const auto convertedWideCount = ::MultiByteToWideChar
	(
		_codePageInfo.CodePage,                // �R�[�h�y�[�W
		0,                                     // �t���O
		reinterpret_cast<const char*>(input),  // �ϊ����̕�����
		static_cast<int32>(inputByteSize),     // �ϊ����̕�����̃o�C�g��
		utf16Buffer.Data(),                    // �ϊ���̕�����
		static_cast<int32>(outputElementSize)  // �ϊ���̕�����̗v�f��
	);

	// �ϊ����s
	if(convertedWideCount <= 0)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-          UTF32�ɕϊ�
	---------------------------------------------------------------------*/
	UnicodeConverter::Options options = {};
	const auto utf32ConvertResult = UnicodeConverter::ConvertUTF16ToUTF32(reinterpret_cast<uint16*>(utf16Buffer.Data()), convertedWideCount, reinterpret_cast<uint32*>(output), outputElementSize, options);

	if(utf32ConvertResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	// �ϊ����ʂ�ݒ�
	result->ConsumedByteSize     = inputByteSize;
	result->OutputByteSize       = options.ConvertedTargetLength * sizeof(uint32);
	result->OutputCharacterCount = options.ConvertedTargetLength;

	return true;
}

/*!**********************************************************************
*  @brief     �����񒷂��擾���܂�
*  @param[in] const void* buffer ������
*  @param[in] const uint64 bufferSize ������̃o�C�g��
*  @return    const uint64 ������
*************************************************************************/
uint64 WindowsCodePage::GetCharacterLength(const void* buffer, const uint64 bufferSize) const
{
	const uint8* position = static_cast<const uint8*>(buffer);
	const uint8* end      = position + bufferSize;
	uint64 characterCount = 0;

	while(position < end)
	{
		if (::IsDBCSLeadByteEx(_codePageInfo.CodePage, *position))
		{
			++position;
		}

		++position;
		++characterCount;
	}

	return characterCount;
}

/*!**********************************************************************
*  @brief     �w�肳�ꂽ�o�b�t�@�̐擪���}���`�o�C�g�R�[�h��T���Q�[�g�y�A�̐�s�����ł���ꍇ�A�ǉ��œǂނׂ������񒷂���Ԃ��܂��B
*  @param[in] const void* buffer ������
*  @param[in] const uint64 bufferSize ������̃o�C�g��
*  @return    const tchar* �����R�[�h��
*************************************************************************/
uint64 WindowsCodePage::GetLeadExtraLength(const void* buffer,[[maybe_unused]] const uint64 bufferSize) const
{
	return (::IsDBCSLeadByteEx(_codePageInfo.CodePage, *((const uint8*)buffer))) ? 1 : 0;
}

#pragma endregion Public Function

#endif 