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
#pragma warning(disable: 26819) // ���߂̂��ĂȂ��t�H�[���X���[������܂�

using namespace gu;
using namespace gu::details::string;
namespace
{
	/*! @brief �㑱�o�C�g���ł�. 0: ASCII����, 1: ���[���b�p���L����(�E�����E�g�Ȃ�), 2: �Ђ炪�ȂȂ�, 3: �G�����̂悤�Ȃ���, 4, 5 : ��{������Ă��Ȃ�*/
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

	/*! @brief UTF-8 �� UTF-32 �̕ϊ����Ɏg�p���AUTF-8 �̃o�C�g�}�[�N��ł����� */
	constexpr uint32 OFFSET_FROM_UTF8[6] =
	{
		0x00000000UL,
		0x00003080UL,
		0x000E2080UL,
		0x03C82080UL,
		0xFA082080UL,
		0x82082080UL,
	};

	/*! @brief UTF-8 �̐�s�o�C�g�ɕt���o�C�g�}�[�N 00000000, 00000000, 11000000, 11100000, 11110000, 11111000, 11111100*/
	constexpr uint8 UTF8_FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     UTF-8�������UTF-16������ɕϊ����܂�.
*  @param[in] const char8*     �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] gu::char16*      �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    StringConvertResult �ϊ�����
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
		-       UTF8���ϒ��G���R�[�h�ł��邽��, �Œ蒷��UTF32�ɕϊ����܂�. 
		---------------------------------------------------------------------*/
		UTF32 codePoint = 0;
		result = CodePointUTF8ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32��UTF16�ɕϊ����܂�.
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
*  @brief     UTF-8�������UTF-32������ɕϊ����܂�.
*  @param[in] const UTF8*      �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] UTF32*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    StringConvertResult �ϊ�����
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
		-       UTF8���ϒ��G���R�[�h�ł��邽��, �Œ蒷��UTF32�ɕϊ����܂�.
		---------------------------------------------------------------------*/
		UTF32 codePoint = 0;
		result = CodePointUTF8ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32�p�̃o�b�t�@�͈̔̓`�F�b�N
		---------------------------------------------------------------------*/
		if (targetIt >= targetEnd)
		{
			sourceIt = oldSourceIt;
			result   = StringConvertResult::TargetExhausted;
			break;
		}

		/*-------------------------------------------------------------------
		-       �s�������`�F�b�N
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
*  @brief     UTF-16�������UTF-8������ɕϊ����܂�.
*  @param[in] const UTF16*      �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] UTF8*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    StringConvertResult �ϊ�����
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
		-       UTF16���ϒ��G���R�[�h�ł��邽��, �Œ蒷��UTF32�ɕϊ����܂�.
		---------------------------------------------------------------------*/
		UTF32 codePoint = 0;
		result = CodePointUTF16ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32��UTF16�ɕϊ����܂�.
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
*  @brief     UTF-16�������UTF-32������ɕϊ����܂�.
*  @param[in] const UTF16*      �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] UTF32*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    StringConvertResult �ϊ�����
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
		-       UTF16���ϒ��G���R�[�h�ł��邽��, �Œ蒷��UTF32�ɕϊ����܂�.
		---------------------------------------------------------------------*/
		UTF32 codePoint = 0;
		result = CodePointUTF16ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != StringConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32�p�̃o�b�t�@�͈̔̓`�F�b�N
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
*  @brief     UTF-32�������UTF-8������ɕϊ����܂�.
*  @param[in] const UTF32*      �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] UTF8*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    StringConvertResult �ϊ�����
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
		-       UTF16�T���Q�[�gh��UTF32�ł͕s������
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
		-       UTF32��UTF8�ɕϊ����܂�.
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
*  @brief     UTF-32�������UTF-8������ɕϊ����܂�.
*  @param[in] const UTF32*     �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] UTF16*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    StringConvertResult �ϊ�����
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
		-       �o�̓o�b�t�@�̃T�C�Y�`�F�b�N
		---------------------------------------------------------------------*/
		if(targetIt >= targetEnd)
		{
			result = StringConvertResult::TargetExhausted;
			break;
		}

		UTF32 codePoint = *sourceIt;
		sourceIt++;

		/*-------------------------------------------------------------------
		-       UTF32��UTF16�ɕϊ����܂�.
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
*  @brief        UTF-8������UTF-32�ɕϊ����܂�. (1������). CodePoint��, U+0000�`U+10FFFF�܂ł͈̔͂̒l�ł�.
*  @param[in]    const UTF8**    �ϊ����̕�����
*  @param[in]    const UTF8*     �ϊ����̏I�[����
*  @param[out]   UTF32&          �ϊ���̕���
*  @param[inout] Options&        �I�v�V����
*  @return       StringConvertResult �ϊ�����
*************************************************************************/
StringConvertResult StringConverterBase::CodePointUTF8ToUTF32(const UTF8** sourceStart, const UTF8* sourceEnd, UTF32& target, Options& options)
{
	auto source = *sourceStart;
	
	/*-------------------------------------------------------------------
	-       �ǉ��œǂރo�C�g�����擾���܂�. (�͈̓`�F�b�N)
	---------------------------------------------------------------------*/
	const auto bytes  = UTF8_BYTE_MAP[*source];
	
	if(source + bytes >= sourceEnd)
	{
		return StringConvertResult::SourceExhausted;
	}

	/*-------------------------------------------------------------------
	-       UTF8�̃o�C�g�V�[�P���X�ł��邩���m�F
	---------------------------------------------------------------------*/
	if (!IsLegalUTF8(source, bytes + 1))
	{
		return StringConvertResult::SourceIllegal;
	}

	/*-------------------------------------------------------------------
	-       UTF32�����̎擾
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
*  @brief        UTF-16������UTF-32�ɕϊ����܂�.
*  @param[in]    const UTF16**    �ϊ����̕�����
*  @param[in]    const UTF16*     �ϊ����̏I�[����
*  @param[out]   gu::char32*      �ϊ���̕�����
*  @param[inout] Options&         �I�v�V����
*  @return       StringConvertResult �ϊ�����
*************************************************************************/
StringConvertResult StringConverterBase::CodePointUTF16ToUTF32(const UTF16** sourceStart, const UTF16* sourceEnd, UTF32& target, Options& options)
{
	auto source = *sourceStart;
	
	UTF32 char1 = *source;
	source++;

	/*-------------------------------------------------------------------
	-        �T���Q�[�g�y�A�������Ă���ꍇ��UTF32�̕ϊ��������s��
	---------------------------------------------------------------------*/
	if(SURROGATE_HIGH_START <= char1 && char1 <= SURROGATE_HIGH_END)
	{
		// �o�b�t�@�̏I�[�𒲂ׂ�
		if(source < sourceEnd)
		{
			UTF32 char2 = *source; // 2�����ڂ��擾

			if(SURROGATE_LOW_START <= char2 && char2 <= SURROGATE_LOW_END)
			{
				target = ((char1 - SURROGATE_HIGH_START) << SURROGATE_HIGH_SHIFT) +
					      (char2 - SURROGATE_LOW_START) + SURROGATE_LOW_BASE;
				++source;
			}
			else if(IsStrictConversion(options))
			{
				// ��ʃT���Q�[�g������̂�, ���ʃT���Q�[�g���Ȃ��ꍇ�̓G���[
				return StringConvertResult::SourceIllegal;
			}
		}
		else
		{
			// ���ʃT���Q�[�g��T���Ă���r���Ƀo�b�t�@�I�[�ɓ��B����ꍇ�̓G���[
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
*  @brief        UTF-32������UTF-8�ɕϊ����܂�.
*  @param[in]    const UTF32     �ϊ����̕���
*  @param[in]    UTF8**          �ϊ���̕�����|�C���^ (���ۂɊւ�炸��O�܂ŕ�����������Ă���.)
*  @param[out]   const UTF8*     �ϊ���̏I�[����
*  @param[inout] Options&        �I�v�V����
*  @return       StringConvertResult �ϊ�����
*************************************************************************/
StringConvertResult StringConverterBase::CodePointUTF32ToUTF8(UTF32 source, UTF8** targetStart, const UTF8* targetEnd, Options& options)
{
	auto target = *targetStart;

	/*-------------------------------------------------------------------
	-       �ϊ���̃o�C�g�������o�C�g�����m�F���܂�
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
	-       �͈̓`�F�b�N
	---------------------------------------------------------------------*/
	if(target + bytes > targetEnd)
	{
		return StringConvertResult::TargetExhausted;
	}

	/*-------------------------------------------------------------------
	-       �͈̓`�F�b�N
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
*  @brief        UTF-32������UTF-16�ɕϊ����܂�.
*  @param[in]    const UTF32     �ϊ����̕���
*  @param[in]    const UTF16**   �ϊ���̕�����|�C���^
*  @param[out]   const UTF16*    �ϊ���̏I�[����
*  @param[inout] Options&        �I�v�V����
*  @return       StringConvertResult �ϊ�����
*************************************************************************/
StringConvertResult StringConverterBase::CodePointUTF32ToUTF16(UTF32 source, UTF16** targetStart, const UTF16* targetEnd, Options& options)
{
	auto target = *targetStart;

	/*-------------------------------------------------------------------
	-         0xFFFF�ȉ��ł͕��ʂ�UTF16�����Ƃ��Ĉ�����
	---------------------------------------------------------------------*/
	if (source <= MAX_BMP)
	{
		// UTF-16�̃T���Q�[�g��UTF32�ł͖����ȕ���
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
	-         �K�i�O�̕���
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
	-        0xFFFF �` 0x10FFFF �̊� (�T���Q�[�g�g�p)
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
*  @brief        �K�؂�UTF8�R�[�f�b�N���ǂ���
*  @param[in]    const UTF8* �ϊ���
*  @param[in]    const uint64 �����񒷂�
*  @return       bool
*************************************************************************/
bool StringConverterBase::IsLegalUTF8(const UTF8* sourceStart, const uint64 length)
{
	UTF8 result = 0;

	// �ŏ��Ɉ�Ԍ����w���A�t�H�[���X���[�łЂƂ��O�֌��Ă���
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