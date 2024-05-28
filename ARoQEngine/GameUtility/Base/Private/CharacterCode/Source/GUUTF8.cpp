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

#pragma warning(disable: 26819) // 注釈のついてないフォールスルーがあります

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

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
bool UTF8::FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	if (_isBOM)
	{
		// BOMを書き込む
		output[0] = 0xEF;
		output[1] = 0xBB;
		output[2] = 0xBF;
	}

	/*-------------------------------------------------------------------
	-              UTF8に変換します
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
*  @brief     UTF32から指定した文字コードに変換します
*  @param[in] const uint32* input 変換元の文字列
*  @param[in] const uint64 inputElementSize 変換元の文字列の要素数
*  @param[in] uint8* output 変換後の文字列
*  @param[in] const uint64 outputByteSize 変換後の文字列のバイト数
*  @param[in] CharacterDecodeResult* result 変換結果
*  @return    const tchar* 文字コード名
*************************************************************************/
bool UTF8::FromUTF32(const uint32* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	if (_isBOM)
	{
		// BOMを書き込む
		output[0] = 0xEF;
		output[1] = 0xBB;
		output[2] = 0xBF;
	}

	/*-------------------------------------------------------------------
	-              UTF8に変換します
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
*  @brief     指定した文字コードからUTF16に変換します
*  @param[in] const uint8* input 変換元の文字列
*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
*  @param[in] uint16* output 変換後の文字列
*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
*  @param[in] CharacterDecodeResult* result 変換結果
*  @return    const tchar* 文字コード名
*************************************************************************/
bool UTF8::ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	uint64 remainByteSize = inputByteSize;

	// BOMの有無を確認
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
	-              UTF32に変換します
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
*  @brief     指定した文字コードからUTF32に変換します
*  @param[in] const uint8* input 変換元の文字列
*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
*  @param[in] uint32* output 変換後の文字列
*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
*  @param[in] CharacterDecodeResult* result 変換結果
*  @return    const tchar* 文字コード名
*************************************************************************/
bool UTF8::ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	uint64 remainByteSize = inputByteSize;

	// BOMの有無を確認
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
	-              UTF32に変換します
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
*  @brief     文字列長を取得します
*  @param[in] void
*  @return    const uint64 文字数
*************************************************************************/
uint64 UTF8::GetCharacterLength(const void* buffer, const uint64 bufferSize) const
{
	uint64 length = 0;
	const auto result = UnicodeConverter::GetUTF8CharacterLength((const uint8*)buffer, bufferSize, length, true);

	return result == UnicodeConvertResult::Success ? length : 0;
}

/*!**********************************************************************
*  @brief     指定されたバッファの先頭がマルチバイトコードやサロゲートペアの先行文字である場合、追加で読むべき文字列長さを返します。
*  @param[in] const void* buffer 文字列
*  @param[in] const uint64 bufferSize 文字列のバイト数
*  @return    const tchar* 文字コード名
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
*  @brief     有効なBOMの判定
*  @param[in] const uint8* 入力文字列
*  @return    bool
*************************************************************************/
bool UTF8::IsValidBOM(const uint8* input) const
{
	if (!_isBOM) { return false; }

	return input[0] == 0xEF && input[1] == 0xBB && input[2] == 0xBF;
}
#pragma endregion Private Function