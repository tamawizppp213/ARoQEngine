//////////////////////////////////////////////////////////////////////////////////
///  @file   GUStringConverterBase.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/18 2:52:38
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/Base/Include/GUStringConverterBase.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 26819) // 注釈のついてないフォールスルーがあります

using namespace gu;
using namespace gu::details::string;
namespace
{
	/*! @brief 後続バイト数です. 0: ASCII文字, 1: ヨーロッパ特有文字(ウムラウトなど), 2: ひらがななど, 3: 絵文字のようなもの, 4, 5 : 基本許可されていない*/
	constexpr uint8 UTF8_BYTE_MAP[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
	};

	/*! @brief UTF-8 → UTF-32 の変換時に使用し、UTF-8 のバイトマークを打ち消す */
	constexpr uint32 OFFSET_FROM_UTF8[6] =
	{
		0x00000000UL,
		0x00003080UL,
		0x000E2080UL,
		0x03C82080UL,
		0xFA082080UL,
		0x82082080UL,
	};

	/*! @brief UTF-8 の先行バイトに付くバイトマーク 00000000, 00000000, 11000000, 11100000, 11110000, 11111000, 11111100*/
	constexpr uint8 UTF8_FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     UTF-8文字列をUTF-16文字列に変換します.
*  @param[in] const char8*     変換元の文字列
*  @param[in] const gu::uint64 変換元の文字列のサイズ
*  @param[in] gu::char16*      変換先の文字列
*  @param[in] gu::uint64       変換先の文字列の長さ
*  @param[inout] Options&      オプション
*  @return    StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::ConvertUTF8ToUTF16(const UTF8* sourceStart, gu::uint64 sourceLength, UTF16* targetStart, gu::uint64 targetLength, Options& options)
{
	const UTF8*  sourceIt  = sourceStart;
	      UTF16* targetIt  = targetStart;
	const UTF8*  sourceEnd = sourceStart + sourceLength;
	const UTF16* targetEnd = targetStart + targetLength;
	StringConvertResult result = StringConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		/*-------------------------------------------------------------------
		-       UTF8が可変長エンコードであるため, 固定長のUTF32に変換します. 
		---------------------------------------------------------------------*/
		UTF32 codePoint = 0;
		result = CodePointUTF8ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32をUTF16に変換します.
		---------------------------------------------------------------------*/
		result = CodePointUTF32ToUTF16(codePoint, &targetIt, targetEnd, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		options.CharacterCount++;
	}

	options.ConvertedSourceLength = static_cast<int32>(sourceIt - sourceStart);
	options.ConvertedTargetLength = static_cast<int32>(targetIt - targetStart);

	return result;
}

/*!**********************************************************************
*  @brief     UTF-8文字列をUTF-32文字列に変換します.
*  @param[in] const UTF8*      変換元の文字列
*  @param[in] const gu::uint64 変換元の文字列のサイズ
*  @param[in] UTF32*           変換先の文字列
*  @param[in] gu::uint64       変換先の文字列の長さ
*  @param[inout] Options&      オプション
*  @return    StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::ConvertUTF8ToUTF32(const UTF8* sourceStart, uint64 sourceLength, UTF32* targetStart, gu::uint64 targetLength, Options& options)
{
	const UTF8*  sourceIt  = sourceStart;
	      UTF32* targetIt  = targetStart;
	const UTF8*  sourceEnd = sourceStart + sourceLength;
	const UTF32* targetEnd = targetStart + targetLength;
	StringConvertResult result = StringConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		const UTF8* oldSourceIt = sourceIt;

		/*-------------------------------------------------------------------
		-       UTF8が可変長エンコードであるため, 固定長のUTF32に変換します.
		---------------------------------------------------------------------*/
		UTF32 codePoint = 0;
		result = CodePointUTF8ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32用のバッファの範囲チェック
		---------------------------------------------------------------------*/
		if (targetIt >= targetEnd)
		{
			sourceIt = oldSourceIt;
			result   = StringConvertResult::TargetExhausted;
			break;
		}

		/*-------------------------------------------------------------------
		-       不正文字チェック
		---------------------------------------------------------------------*/
		bool isInvalid = codePoint > MAX_LEGAL_UTF32 || (SURROGATE_HIGH_START <= codePoint && codePoint <= SURROGATE_LOW_END);

		if (isInvalid)
		{
		    if(IsStrictConversion(options))
			{
				sourceIt = oldSourceIt;
				result   = StringConvertResult::SourceIllegal;
				break;
			}
			else
			{
				*targetIt = options.ReplacementChar;
				targetIt++;
				options.InvalidCharacterCount++;
			}
		}
		else
		{
			*targetIt++ = codePoint;
		}

		options.CharacterCount++;
	}

	options.ConvertedSourceLength = static_cast<int32>(sourceIt - sourceStart);
	options.ConvertedTargetLength = static_cast<int32>(targetIt - targetStart);
	return result;
}

/*!**********************************************************************
*  @brief     UTF-16文字列をUTF-8文字列に変換します.
*  @param[in] const UTF16*      変換元の文字列
*  @param[in] const gu::uint64 変換元の文字列のサイズ
*  @param[in] UTF8*           変換先の文字列
*  @param[in] gu::uint64       変換先の文字列の長さ
*  @param[inout] Options&      オプション
*  @return    StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::ConvertUTF16ToUTF8(const UTF16* sourceStart, uint64 sourceLength, UTF8* targetStart, gu::uint64 targetLength, Options& options)
{
	const UTF16* sourceIt = sourceStart;
	      UTF8*  targetIt = targetStart;
	const UTF16* sourceEnd = sourceStart + sourceLength;
	      UTF8*  targetEnd = targetStart + targetLength;

	StringConvertResult result = StringConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		const UTF16* oldSourceIt = sourceIt;

		/*-------------------------------------------------------------------
		-       UTF16が可変長エンコードであるため, 固定長のUTF32に変換します.
		---------------------------------------------------------------------*/
		UTF32 codePoint = 0;
		result = CodePointUTF16ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32をUTF16に変換します.
		---------------------------------------------------------------------*/
		result = CodePointUTF32ToUTF8(codePoint, &targetIt, targetEnd, options);

		if (result != StringConvertResult::Success)
		{
			sourceIt = oldSourceIt;
			break;
		}

		options.CharacterCount++;
	}

	options.ConvertedSourceLength = static_cast<int32>(sourceIt - sourceStart);
	options.ConvertedTargetLength = static_cast<int32>(targetIt - targetStart);
}

/*!**********************************************************************
*  @brief     UTF-16文字列をUTF-32文字列に変換します.
*  @param[in] const UTF16*      変換元の文字列
*  @param[in] const gu::uint64 変換元の文字列のサイズ
*  @param[in] UTF32*           変換先の文字列
*  @param[in] gu::uint64       変換先の文字列の長さ
*  @param[inout] Options&      オプション
*  @return    StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::ConvertUTF16ToUTF32(const UTF16* sourceStart, uint64 sourceLength, UTF32* targetStart, gu::uint64 targetLength, Options& options)
{
	const UTF16* sourceIt = sourceStart;
	      UTF32* targetIt = targetStart;
	const UTF16* sourceEnd = sourceStart + sourceLength;
	const UTF32* targetEnd = targetStart + targetLength;

	StringConvertResult result = StringConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		const UTF16* oldSourceIt = sourceIt;

		/*-------------------------------------------------------------------
		-       UTF16が可変長エンコードであるため, 固定長のUTF32に変換します.
		---------------------------------------------------------------------*/
		UTF32 codePoint = 0;
		result = CodePointUTF16ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32用のバッファの範囲チェック
		---------------------------------------------------------------------*/
		if (targetIt >= targetEnd)
		{
			sourceIt = oldSourceIt;
			result = StringConvertResult::TargetExhausted;
			break;
		}

		*targetIt = codePoint;
		targetIt++;

		options.CharacterCount++;
	}

	options.ConvertedSourceLength = static_cast<int32>(sourceIt - sourceStart);
	options.ConvertedTargetLength = static_cast<int32>(targetIt - targetStart);
	return result;

}

/*!**********************************************************************
*  @brief     UTF-32文字列をUTF-8文字列に変換します.
*  @param[in] const UTF32*      変換元の文字列
*  @param[in] const gu::uint64 変換元の文字列のサイズ
*  @param[in] UTF8*           変換先の文字列
*  @param[in] gu::uint64       変換先の文字列の長さ
*  @param[inout] Options&      オプション
*  @return    StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::ConvertUTF32ToUTF8(const UTF32* sourceStart, uint64 sourceLength, UTF8* targetStart, gu::uint64 targetLength, Options& options)
{
	const UTF32* sourceIt = sourceStart;
	      UTF8*  targetIt = targetStart;
	const UTF32* sourceEnd = sourceStart + sourceLength;
	      UTF8*  targetEnd = targetStart + targetLength;

	StringConvertResult result = StringConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		const UTF32* oldSourceIt = sourceIt;
		UTF32 codePoint = *sourceIt;
		sourceIt++;

		/*-------------------------------------------------------------------
		-       UTF16サロゲートhがUTF32では不正文字
		---------------------------------------------------------------------*/
		if(SURROGATE_HIGH_START <= codePoint && codePoint <= SURROGATE_LOW_END)
		{
			if(IsStrictConversion(options))
			{
				sourceIt = oldSourceIt;
				result = StringConvertResult::SourceIllegal;
				break;
			}
			else
			{
				codePoint = options.ReplacementChar;
				options.InvalidCharacterCount++;
			}
		}

		/*-------------------------------------------------------------------
		-       UTF32をUTF8に変換します.
		---------------------------------------------------------------------*/
		result = CodePointUTF32ToUTF8(codePoint, &targetIt, targetEnd, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		options.CharacterCount++;
	}

	options.ConvertedSourceLength = static_cast<int32>(sourceIt - sourceStart);
	options.ConvertedTargetLength = static_cast<int32>(targetIt - targetStart);
	return result;
}
/*!**********************************************************************
*  @brief     UTF-32文字列をUTF-8文字列に変換します.
*  @param[in] const UTF32*     変換元の文字列
*  @param[in] const gu::uint64 変換元の文字列のサイズ
*  @param[in] UTF16*           変換先の文字列
*  @param[in] gu::uint64       変換先の文字列の長さ
*  @param[inout] Options&      オプション
*  @return    StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::ConvertUTF32ToUTF16(const UTF32* sourceStart, uint64 sourceLength, UTF16* targetStart, gu::uint64 targetLength, Options& options)
{
	const UTF32* sourceIt = sourceStart;
	      UTF16* targetIt = targetStart;
	const UTF32* sourceEnd = sourceStart + sourceLength;
	const UTF16* targetEnd = targetStart + targetLength;
	options.CharacterCount = 0;

	StringConvertResult result = StringConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		/*-------------------------------------------------------------------
		-       出力バッファのサイズチェック
		---------------------------------------------------------------------*/
		if(targetIt >= targetEnd)
		{
			result = StringConvertResult::TargetExhausted;
			break;
		}

		UTF32 codePoint = *sourceIt;
		sourceIt++;

		/*-------------------------------------------------------------------
		-       UTF32をUTF16に変換します.
		---------------------------------------------------------------------*/
		result = CodePointUTF32ToUTF16(codePoint, &targetIt, targetEnd, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		options.CharacterCount++;
	}

	options.ConvertedSourceLength = static_cast<int32>(sourceIt - sourceStart);
	options.ConvertedTargetLength = static_cast<int32>(targetIt - targetStart);
	return result;

}

/*!**********************************************************************
*  @brief        UTF-8文字をUTF-32に変換します. (1文字分). CodePointは, U+0000～U+10FFFFまでの範囲の値です.
*  @param[in]    const UTF8**    変換元の文字列
*  @param[in]    const UTF8*     変換元の終端文字
*  @param[out]   UTF32&          変換先の文字
*  @param[inout] Options&        オプション
*  @return       StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::CodePointUTF8ToUTF32(const UTF8** sourceStart, const UTF8* sourceEnd, UTF32& target, Options& options)
{
	auto source = *sourceStart;
	
	/*-------------------------------------------------------------------
	-       追加で読むバイト数を取得します. (範囲チェック)
	---------------------------------------------------------------------*/
	const auto bytes  = UTF8_BYTE_MAP[*source];
	
	if(source + bytes >= sourceEnd)
	{
		return StringConvertResult::SourceExhausted;
	}

	/*-------------------------------------------------------------------
	-       UTF8のバイトシーケンスであるかを確認
	---------------------------------------------------------------------*/
	if (!IsLegalUTF8(source, bytes + 1))
	{
		return StringConvertResult::SourceIllegal;
	}

	/*-------------------------------------------------------------------
	-       UTF32文字の取得
	---------------------------------------------------------------------*/
	UTF32 result = 0;

	switch (bytes)
	{
		case 5: 
		{
			result += *source++;
			result <<= 6;
		}
		case 4: 
		{
			result += *source++;
			result <<= 6;
		}
		case 3: 
		{
			result += *source++;
			result <<= 6;
		}
		case 2: 
		{
			result += *source++;
			result <<= 6;
		}
		case 1: 
		{
			result += *source++;
			result <<= 6;
		}
		case 0: 
		{
			result += *source++;
		}
	}

	result -= OFFSET_FROM_UTF8[bytes];

	*sourceStart = source;
	target       = result;

	return StringConvertResult::Success;
}

/*!**********************************************************************
*  @brief        UTF-16文字をUTF-32に変換します.
*  @param[in]    const UTF16**    変換元の文字列
*  @param[in]    const UTF16*     変換元の終端文字
*  @param[out]   gu::char32*      変換先の文字列
*  @param[inout] Options&         オプション
*  @return       StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::CodePointUTF16ToUTF32(const UTF16** sourceStart, const UTF16* sourceEnd, UTF32& target, Options& options)
{
	auto source = *sourceStart;
	
	UTF32 char1 = *source;
	source++;

	/*-------------------------------------------------------------------
	-        サロゲートペアを持っている場合はUTF32の変換処理を行う
	---------------------------------------------------------------------*/
	if(SURROGATE_HIGH_START <= char1 && char1 <= SURROGATE_HIGH_END)
	{
		// バッファの終端を調べる
		if(source < sourceEnd)
		{
			UTF32 char2 = *source; // 2文字目を取得

			if(SURROGATE_LOW_START <= char2 && char2 <= SURROGATE_LOW_END)
			{
				target = ((char1 - SURROGATE_HIGH_START) << SURROGATE_HIGH_SHIFT) +
					      (char2 - SURROGATE_LOW_START) + SURROGATE_LOW_BASE;
				++source;
			}
			else if(IsStrictConversion(options))
			{
				// 上位サロゲートがあるのに, 下位サロゲートがない場合はエラー
				return StringConvertResult::SourceIllegal;
			}
		}
		else
		{
			// 下位サロゲートを探している途中にバッファ終端に到達する場合はエラー
			return StringConvertResult::SourceExhausted;
		}
		return StringConvertResult::SourceIllegal;
	}
	else
	{
		if (SURROGATE_LOW_START <= char1 && char1 <= SURROGATE_LOW_END)
		{
			return StringConvertResult::SourceIllegal;
		}
	}

	*sourceStart = source;
	target       = char1;

	return StringConvertResult::Success;
}

/*!**********************************************************************
*  @brief        UTF-32文字をUTF-8に変換します.
*  @param[in]    const UTF32     変換元の文字
*  @param[in]    UTF8**          変換先の文字列ポインタ (成否に関わらず一つ前まで文字が書かれている.)
*  @param[out]   const UTF8*     変換先の終端文字
*  @param[inout] Options&        オプション
*  @return       StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::CodePointUTF32ToUTF8(UTF32 source, UTF8** targetStart, const UTF8* targetEnd, Options& options)
{
	auto target = *targetStart;

	/*-------------------------------------------------------------------
	-       変換後のバイト数が何バイトかを確認します
	---------------------------------------------------------------------*/
	unsigned short bytes = 0;
	if (source < 0x80) // 1000 0000
	{
		bytes = 1;
	}
	else if (source < 0x800) // 1000 0000 0000
	{
		bytes = 2;
	}
	else if (source < 0x10000) // 0001 0000 0000 0000 0000
	{
		bytes = 3;
	}
	else if (source < MAX_LEGAL_UTF32)
	{
		bytes = 4;
	}
	else
	{
		bytes = 3;
		if (IsStrictConversion(options))
		{
			return StringConvertResult::SourceIllegal;
		}
		else
		{
			source = options.ReplacementChar;
			options.InvalidCharacterCount++;
		}
		return StringConvertResult::SourceIllegal;
	}

	/*-------------------------------------------------------------------
	-       範囲チェック
	---------------------------------------------------------------------*/
	if(target + bytes > targetEnd)
	{
		return StringConvertResult::TargetExhausted;
	}

	/*-------------------------------------------------------------------
	-       範囲チェック
	---------------------------------------------------------------------*/
	constexpr uint32 BYTE_MARK = 0x80;
	constexpr uint32 BYTE_MASK = 0xBF;

	target += bytes;
	
	switch (bytes)
	{
		case 4: 
		{
			*(--target) = static_cast<UTF8>((source | BYTE_MARK) & BYTE_MASK);
			source >>= 6;
		}
		case 3: 
		{
			*(--target) = static_cast<UTF8>((source | BYTE_MARK) & BYTE_MASK);
			source >>= 6;
		}
		case 2: 
		{
			*(--target) = static_cast<UTF8>((source | BYTE_MARK) & BYTE_MASK);
			source >>= 6;
		}
		case 1: 
		{
			*(--target) = static_cast<UTF8>(source | UTF8_FIRST_BYTE_MARK[bytes]);
		}
	}

	target      += bytes;
	*targetStart = target;
	return StringConvertResult::Success;
}

/*!**********************************************************************
*  @brief        UTF-32文字をUTF-16に変換します.
*  @param[in]    const UTF32     変換元の文字
*  @param[in]    const UTF16**   変換先の文字列ポインタ
*  @param[out]   const UTF16*    変換先の終端文字
*  @param[inout] Options&        オプション
*  @return       StringConvertResult 変換結果
*************************************************************************/
StringConvertResult StringConverterBase::CodePointUTF32ToUTF16(UTF32 source, UTF16** targetStart, const UTF16* targetEnd, Options& options)
{
	auto target = *targetStart;

	/*-------------------------------------------------------------------
	-         0xFFFF以下では普通のUTF16文字として扱える
	---------------------------------------------------------------------*/
	if (source <= MAX_BMP)
	{
		// UTF-16のサロゲートはUTF32では無効な文字
		if (SURROGATE_HIGH_START <= source && source <= SURROGATE_LOW_END)
		{
			if (IsStrictConversion(options))
			{
				return StringConvertResult::SourceExhausted;
			}
			else
			{
				*target = (UTF16)options.ReplacementChar;
				target++;
				options.InvalidCharacterCount++;
			}
		}
		else
		{
			*target = (UTF16)source;
			target++;
		}
	}
	/*-------------------------------------------------------------------
	-         規格外の文字
	---------------------------------------------------------------------*/
	else if (source > MAX_LEGAL_UTF32)
	{
		if (IsStrictConversion(options))
		{
			return StringConvertResult::SourceExhausted;
		}
		else
		{
			*target = (UTF16)options.ReplacementChar;
			target++;
			options.InvalidCharacterCount++;
		}
	}
	/*-------------------------------------------------------------------
	-        0xFFFF ～ 0x10FFFF の間 (サロゲート使用)
	---------------------------------------------------------------------*/
	else
	{
		if(target + 1 >= targetEnd)
		{
			return StringConvertResult::TargetExhausted;
		}

		source -= SURROGATE_LOW_BASE;
		*target = (UTF16)((source >> SURROGATE_HIGH_SHIFT) + SURROGATE_HIGH_START);
		target++;
		*target = (UTF16)((source & SURROGATE_LOW_MASK) + SURROGATE_LOW_START);
		target++;
	}

	*targetStart = target;
	return StringConvertResult::Success;
}
#pragma endregion Public Function

#pragma region Private Function
/*!**********************************************************************
*  @brief        適切なUTF8コーデックかどうか
*  @param[in]    const UTF8* 変換元
*  @param[in]    const uint64 文字列長さ
*  @return       bool
*************************************************************************/
bool StringConverterBase::IsLegalUTF8(const UTF8* sourceStart, const uint64 length)
{
	UTF8 result = 0;

	// 最初に一番後ろを指し、フォールスルーでひとつずつ前へ見ていく
	const UTF8* source = sourceStart + length;

	switch(length)
	{
		case 4:
		{
			if ((result = (*--source)) < 0x80 || result > 0xBF)
			{
				return false;
			}
		}
		case 3:
		{
			if ((result = (*--source)) < 0x80 || result > 0xBF)
			{
				return false;
			}
		}
		case 2:
		{
			if ((result = (*--source)) < 0x80 || result > 0xBF)
				return false;

			switch (*source)
			{
				case 0xE0:
				{
					if (result < 0xA0)
					{
						return false;
					}
					break;
				}
				case 0xED:
				{
					if (result > 0x9F)
					{
						return false;
					}
					break;
				}
				case 0xF0:
				{
					if (result < 0x90)
					{
						return false;
					}
					break;
				}
				case 0xF4:
				{
					if (result > 0x8F)
					{
						return false;
					}
					break;
				}
				default:
				{
					if (result < 0x80)
					{
						return false;
					}
				}
			}
		}
		case 1:
		{
			if (*source >= 0x80 && *source < 0xC2)
			{
				return false;
			}
		}
		default:
		{
			return false;
		}
	}

	if (*source > 0xF4) { return false; }

	return true;
}
#pragma endregion Private Function