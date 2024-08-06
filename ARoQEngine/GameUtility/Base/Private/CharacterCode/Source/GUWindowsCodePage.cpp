//////////////////////////////////////////////////////////////////////////////////
///  @file   GUWindowsCodePage.cpp
///  @brief  Windowsのコードページを扱うクラス
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
	-            コードページを取得します
	---------------------------------------------------------------------*/
	if (!::GetCPInfoEx(codePage, 0, &_codePageInfo))
	{
		return;
	}
}


#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     UTF16から指定した文字コードに変換します
*  @param[in] const uint16* input 変換元の文字列
*  @param[in] const uint64 inputElementSize 変換元の文字列の要素数
*  @param[in] uint8* output 変換後の文字列
*  @param[in] const uint64 outputByteSize 変換後の文字列のバイト数
*  @param[in] CharacterDecodeResult* result 変換結果
*  @return    const tchar* 文字コード名
*************************************************************************/
bool WindowsCodePage::FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-          初期設定
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

	// 入力が0文字の場合は変換を行わない
	// WideCharToMultiByteの戻り値がエラーなのか成功なのかが分からなくなる
	if (inputElementSize == 0)
	{
		return true;
	}

	/*-------------------------------------------------------------------
	-          CodePage毎の変換
	---------------------------------------------------------------------*/
	const auto convertedByteCount = ::WideCharToMultiByte
	(
		_codePageInfo.CodePage,                // コードページ
		0,                                     // フラグ
		reinterpret_cast<const wchar*>(input), // 変換元の文字列
		static_cast<int32>(inputElementSize),  // 変換元の文字列の要素数
		reinterpret_cast<char*>(output), 	   // 変換後の文字列
		static_cast<int32>(outputByteSize),    // 変換後の文字列のバイト数
		nullptr,                               // デフォルト文字の設定 
		FALSE 						           // デフォルト文字の設定
	);

	// 変換失敗
	if(convertedByteCount <= 0)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-         UnicodeConverterを使って文字数カウント
	---------------------------------------------------------------------*/
	uint64 characterCount = 0;
	const auto utf16CharacterLengthResult = UnicodeConverter::GetUTF16CharacterLength(input, inputElementSize, characterCount, true);

	if (utf16CharacterLengthResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	// 変換結果を設定
	result->ConsumedElementCount = inputElementSize;
	result->OutputByteSize       = convertedByteCount;
	result->OutputCharacterCount = characterCount;

	return true;
}

/*!**********************************************************************
*  @brief     UTF32から指定した文字コードに変換します
*  @param[in] const uint32* input 変換元の文字列
*  @param[in] const uint64 inputElementSize 変換元の文字列の要素数
*  @param[in] uint8* output 変換後の文字列
*  @param[in] const uint64 outputByteSize 変換後の文字列のバイト数
*  @param[in] CharacterDecodeResult* result 変換結果
*  @return    const tchar* 文字コード名
*************************************************************************/
bool WindowsCodePage::FromUTF32(const uint32* input, const uint64 inputElementSize, uint8* output,const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-          初期設定
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

	// 入力が0文字の場合は変換を行わない
	// WideCharToMultiByteの戻り値がエラーなのか成功なのかが分からなくなる
	if (inputElementSize == 0)
	{
		return true;
	}

	/*-------------------------------------------------------------------
	-          Iteratorを使って文字列を変換
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
		-         UTF32からUTF16に変換 (WideCharToMultiByteを実行するため)
		---------------------------------------------------------------------*/
		const uint64 convertSize = (inputEnd - inputPosition) > wordCount / 2 ? wordCount / 2 : (inputEnd - inputPosition);
		if (UnicodeConverter::ConvertUTF32ToUTF16(inputPosition, convertSize, words, wordCount, options) != UnicodeConvertResult::Success)
		{
			return false;
		}

		/*-------------------------------------------------------------------
		-         MultiByteに変換
		---------------------------------------------------------------------*/
		const auto convertedByteCount = ::WideCharToMultiByte
		(
			_codePageInfo.CodePage,                            // コードページ
			0,                                                 // フラグ
			reinterpret_cast<const wchar*>(words),             // 変換元の文字列
			static_cast<int32>(options.ConvertedTargetLength), // 変換元の文字列の要素数
			outputPosition,                                    // 変換後の文字列
			static_cast<int32>(outputEnd - outputPosition),    // 変換後の文字列のバイト数
			nullptr,                                           // デフォルト文字の設定 
			FALSE 						                       // デフォルト文字の設定
		);

		if(convertedByteCount <= 0)
		{
			return false;
		}

		inputPosition                += options.ConvertedSourceLength;
		outputPosition               += convertedByteCount;
		result->OutputCharacterCount += options.CharacterCount;
	}

	// 変換結果を設定
	result->ConsumedElementCount = inputElementSize;
	result->OutputByteSize       = outputPosition - reinterpret_cast<char*>(output);

	return true;
}

/*!**********************************************************************
*  @brief     指定した文字コードからUTF16に変換します
*  @param[in] const uint8* input 変換元の文字列
*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
*  @param[in] uint16* output 変換後の文字列
*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
*  @param[in] CharacterDecodeResult* result 変換結果
*  @return    const tchar* 文字コード名
*************************************************************************/
bool WindowsCodePage::ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-          初期設定
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

	// 入力が0文字の場合は変換を行わない
	// WideCharToMultiByteの戻り値がエラーなのか成功なのかが分からなくなる
	if (inputByteSize == 0)
	{
		return true;
	}

	/*-------------------------------------------------------------------
	-          変換
	---------------------------------------------------------------------*/
	const auto convertedWideCount = ::MultiByteToWideChar
	(
		_codePageInfo.CodePage,                // コードページ
		MB_ERR_INVALID_CHARS,                  // フラグ
		reinterpret_cast<const char*>(input),  // 変換元の文字列
		static_cast<int32>(inputByteSize),     // 変換元の文字列のバイト数
		reinterpret_cast<wchar*>(output),      // 変換後の文字列
		static_cast<int32>(outputElementSize)  // 変換後の文字列の要素数
	);

	// 変換失敗
	if(convertedWideCount <= 0)
	{
		return false;
	}

	// MultiByteToWideCharでは文字数のカウントはできないため, UnicodeConverterを使って文字数カウント
	uint64 characterCount = 0;

	const auto utf16CharacterLengthResult = UnicodeConverter::GetUTF16CharacterLength(output, convertedWideCount, characterCount, true);
	if(utf16CharacterLengthResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	// 変換結果を設定
	result->ConsumedByteSize     = inputByteSize;
	result->OutputByteSize       = convertedWideCount * sizeof(wchar);
	result->OutputCharacterCount = characterCount;

	return true;
}

/*!**********************************************************************
*  @brief     指定した文字コードからUTF32に変換します
*  @param[in] const uint8* input 変換元の文字列
*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
*  @param[in] uint32* output 変換後の文字列
*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
*  @param[in] CharacterDecodeResult* result 変換結果
*  @return    const tchar* 文字コード名
*************************************************************************/
bool WindowsCodePage::ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-          初期設定
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

	// 入力が0文字の場合は変換を行わない
	if (inputByteSize == 0)
	{
		return true;
	}

	/*-------------------------------------------------------------------
	-          UTF16に変換
	---------------------------------------------------------------------*/
	gu::DynamicArray<wchar> utf16Buffer(outputElementSize);
	const auto convertedWideCount = ::MultiByteToWideChar
	(
		_codePageInfo.CodePage,                // コードページ
		0,                                     // フラグ
		reinterpret_cast<const char*>(input),  // 変換元の文字列
		static_cast<int32>(inputByteSize),     // 変換元の文字列のバイト数
		utf16Buffer.Data(),                    // 変換後の文字列
		static_cast<int32>(outputElementSize)  // 変換後の文字列の要素数
	);

	// 変換失敗
	if(convertedWideCount <= 0)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-          UTF32に変換
	---------------------------------------------------------------------*/
	UnicodeConverter::Options options = {};
	const auto utf32ConvertResult = UnicodeConverter::ConvertUTF16ToUTF32(reinterpret_cast<uint16*>(utf16Buffer.Data()), convertedWideCount, reinterpret_cast<uint32*>(output), outputElementSize, options);

	if(utf32ConvertResult != UnicodeConvertResult::Success)
	{
		return false;
	}

	// 変換結果を設定
	result->ConsumedByteSize     = inputByteSize;
	result->OutputByteSize       = options.ConvertedTargetLength * sizeof(uint32);
	result->OutputCharacterCount = options.ConvertedTargetLength;

	return true;
}

/*!**********************************************************************
*  @brief     文字列長を取得します
*  @param[in] const void* buffer 文字列
*  @param[in] const uint64 bufferSize 文字列のバイト数
*  @return    const uint64 文字数
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
*  @brief     指定されたバッファの先頭がマルチバイトコードやサロゲートペアの先行文字である場合、追加で読むべき文字列長さを返します。
*  @param[in] const void* buffer 文字列
*  @param[in] const uint64 bufferSize 文字列のバイト数
*  @return    const tchar* 文字コード名
*************************************************************************/
uint64 WindowsCodePage::GetLeadExtraLength(const void* buffer,[[maybe_unused]] const uint64 bufferSize) const
{
	return (::IsDBCSLeadByteEx(_codePageInfo.CodePage, *((const uint8*)buffer))) ? 1 : 0;
}

#pragma endregion Public Function

#endif 