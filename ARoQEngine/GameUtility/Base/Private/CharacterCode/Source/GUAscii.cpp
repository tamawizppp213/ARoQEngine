//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAscii.cpp
///  @brief  Asciiクラスの実装ファイル
///  @author toide
///  @date   2024/05/24 23:32:54
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/CharacterCode/Include/GUAscii.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::details::string;

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
bool Ascii::FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, [[maybe_unused]] const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              引数チェック
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	/*-------------------------------------------------------------------
	-              引数チェック
	---------------------------------------------------------------------*/
	for (uint64 i = 0; i < inputElementSize; ++i)
	{
		// 普通のASCII文字
		if (input[i] <= 0x7F)
		{
			output[i] = static_cast<uint8>(input[i]);
		}
		else
		{
			return false;
		}

		result->ConsumedElementCount++;
		result->OutputByteSize += sizeof(uint16);
		result->OutputCharacterCount++;
	}

	return true;
}

/*!**********************************************************************
*  @brief     UTF32から指定した文字コードに変換します
*  @param[in] const uint32* input 変換元の文字列
*  @param[in] const uint64 inputElementSize 変換元の文字列のバイト数
*  @param[in] uint8* output 変換後の文字列
*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
*  @param[in] CharacterDecodeResult* result 変換結果
*  @return    const tchar* 文字コード名
*************************************************************************/
bool Ascii::FromUTF32(const uint32* input, const uint64 inputElementSize, uint8* output, [[maybe_unused]] const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              引数チェック
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	/*-------------------------------------------------------------------
	-              引数チェック
	---------------------------------------------------------------------*/
	for (uint64 i = 0; i < inputElementSize; ++i)
	{
		// 普通のASCII文字
		if (input[i] <= 0x7F)
		{
			output[i] = static_cast<uint8>(input[i]);
		}
		else
		{
			return false;
		}

		result->ConsumedElementCount++;
		result->OutputByteSize += sizeof(uint16);
		result->OutputCharacterCount++;
	}

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
bool Ascii::ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, [[maybe_unused]] const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              引数チェック
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	/*-------------------------------------------------------------------
	-              引数チェック
	---------------------------------------------------------------------*/
	for (uint64 i = 0; i < inputByteSize; ++i)
	{
		// 普通のASCII文字
		if(input[i] <= 0x7F)
		{
			output[i] = static_cast<uint16>(input[i]);
		}
		else
		{
			return false;
		}

		result->ConsumedByteSize++;
		result->OutputByteSize += sizeof(uint16);
		result->OutputCharacterCount++;
	}

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
bool Ascii::ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, [[maybe_unused]] const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              引数チェック
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	/*-------------------------------------------------------------------
	-              引数チェック
	---------------------------------------------------------------------*/
	for (uint64 i = 0; i < inputByteSize; ++i)
	{
		// 普通のASCII文字
		if (input[i] <= 0x7F)
		{
			output[i] = static_cast<uint32>(input[i]);
		}
		else
		{
			return false;
		}

		result->ConsumedByteSize++;
		result->OutputByteSize += sizeof(uint32);
		result->OutputCharacterCount++;
	}

	return true;
}
#pragma endregion Public Function