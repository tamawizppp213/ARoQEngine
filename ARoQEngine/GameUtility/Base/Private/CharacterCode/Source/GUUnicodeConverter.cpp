//////////////////////////////////////////////////////////////////////////////////
///  @file   GUStringConverterBase.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/18 2:52:38
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/CharacterCode/Include/GUUnicodeConverter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 26819) // ���߂̂��ĂȂ��t�H�[���X���[������܂�

using namespace gu;
using namespace gu::details::string;
namespace
{
	/*! @brief �㑱�o�C�g���ł�. 0: ASCII����, 1: ���[���b�p���L����(�E�����E�g�Ȃ�), 2: �Ђ炪�ȂȂ�, 3: �G�����̂悤�Ȃ���, 4, 5 : ��{������Ă��Ȃ�*/
	constexpr uint8 UTF8_BYTE_MAP[256] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //64 �` 127

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //128 �` 191

		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //192�`213
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 214�`235
		2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5,
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
*  @return    UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::ConvertUTF8ToUTF16(const uint8* sourceStart, gu::uint64 sourceLength, uint16* targetStart, gu::uint64 targetLength, Options& options)
{
	const uint8*  sourceIt  = sourceStart;
	      uint16* targetIt  = targetStart;
	const uint8*  sourceEnd = sourceStart + sourceLength;
	const uint16* targetEnd = targetStart + targetLength;
	UnicodeConvertResult result = UnicodeConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		/*-------------------------------------------------------------------
		-       UTF8���ϒ��G���R�[�h�ł��邽��, �Œ蒷��UTF32�ɕϊ����܂�. 
		---------------------------------------------------------------------*/
		uint32 codePoint = 0;
		result = CodePointUTF8ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != UnicodeConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32��UTF16�ɕϊ����܂�.
		---------------------------------------------------------------------*/
		result = CodePointUTF32ToUTF16(codePoint, &targetIt, targetEnd, options);

		if (result != UnicodeConvertResult::Success)
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
*  @param[in] const uint8*      �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] uint32*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::ConvertUTF8ToUTF32(const uint8* sourceStart, uint64 sourceLength, uint32* targetStart, gu::uint64 targetLength, Options& options)
{
	const uint8*  sourceIt  = sourceStart;
	      uint32* targetIt  = targetStart;
	const uint8*  sourceEnd = sourceStart + sourceLength;
	const uint32* targetEnd = targetStart + targetLength;
	UnicodeConvertResult result = UnicodeConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		const uint8* oldSourceIt = sourceIt;

		/*-------------------------------------------------------------------
		-       UTF8���ϒ��G���R�[�h�ł��邽��, �Œ蒷��UTF32�ɕϊ����܂�.
		---------------------------------------------------------------------*/
		uint32 codePoint = 0;
		result = CodePointUTF8ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != UnicodeConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32�p�̃o�b�t�@�͈̔̓`�F�b�N
		---------------------------------------------------------------------*/
		if (targetIt >= targetEnd)
		{
			sourceIt = oldSourceIt;
			result   = UnicodeConvertResult::TargetExhausted;
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
				result   = UnicodeConvertResult::SourceIllegal;
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
*  @param[in] const uint16*      �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] uint8*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::ConvertUTF16ToUTF8(const uint16* sourceStart, uint64 sourceLength, uint8* targetStart, gu::uint64 targetLength, Options& options)
{
	const uint16* sourceIt = sourceStart;
	      uint8*  targetIt = targetStart;
	const uint16* sourceEnd = sourceStart + sourceLength;
	      uint8*  targetEnd = targetStart + targetLength;

	UnicodeConvertResult result = UnicodeConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		const uint16* oldSourceIt = sourceIt;

		/*-------------------------------------------------------------------
		-       UTF16���ϒ��G���R�[�h�ł��邽��, �Œ蒷��UTF32�ɕϊ����܂�.
		---------------------------------------------------------------------*/
		uint32 codePoint = 0;
		result = CodePointUTF16ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != UnicodeConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32��UTF16�ɕϊ����܂�.
		---------------------------------------------------------------------*/
		result = CodePointUTF32ToUTF8(codePoint, &targetIt, targetEnd, options);

		if (result != UnicodeConvertResult::Success)
		{
			sourceIt = oldSourceIt;
			break;
		}

		options.CharacterCount++;
	}

	options.ConvertedSourceLength = static_cast<int32>(sourceIt - sourceStart);
	options.ConvertedTargetLength = static_cast<int32>(targetIt - targetStart);
	return result;
}

/*!**********************************************************************
*  @brief     UTF-16�������UTF-32������ɕϊ����܂�.
*  @param[in] const uint16*      �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] uint32*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::ConvertUTF16ToUTF32(const uint16* sourceStart, uint64 sourceLength, uint32* targetStart, gu::uint64 targetLength, Options& options)
{
	const uint16* sourceIt = sourceStart;
	      uint32* targetIt = targetStart;
	const uint16* sourceEnd = sourceStart + sourceLength;
	const uint32* targetEnd = targetStart + targetLength;

	UnicodeConvertResult result = UnicodeConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		const uint16* oldSourceIt = sourceIt;

		/*-------------------------------------------------------------------
		-       UTF16���ϒ��G���R�[�h�ł��邽��, �Œ蒷��UTF32�ɕϊ����܂�.
		---------------------------------------------------------------------*/
		uint32 codePoint = 0;
		result = CodePointUTF16ToUTF32(&sourceIt, sourceEnd, codePoint, options);

		if (result != UnicodeConvertResult::Success)
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       UTF32�p�̃o�b�t�@�͈̔̓`�F�b�N
		---------------------------------------------------------------------*/
		if (targetIt >= targetEnd)
		{
			sourceIt = oldSourceIt;
			result = UnicodeConvertResult::TargetExhausted;
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
*  @param[in] const uint32*      �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] uint8*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::ConvertUTF32ToUTF8(const uint32* sourceStart, uint64 sourceLength, uint8* targetStart, gu::uint64 targetLength, Options& options)
{
	const uint32* sourceIt = sourceStart;
	      uint8*  targetIt = targetStart;
	const uint32* sourceEnd = sourceStart + sourceLength;
	      uint8*  targetEnd = targetStart + targetLength;

	UnicodeConvertResult result = UnicodeConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		const uint32* oldSourceIt = sourceIt;
		uint32 codePoint = *sourceIt;
		sourceIt++;

		/*-------------------------------------------------------------------
		-       UTF16�T���Q�[�gh��UTF32�ł͕s������
		---------------------------------------------------------------------*/
		if(SURROGATE_HIGH_START <= codePoint && codePoint <= SURROGATE_LOW_END)
		{
			if(IsStrictConversion(options))
			{
				sourceIt = oldSourceIt;
				result = UnicodeConvertResult::SourceIllegal;
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

		if (result != UnicodeConvertResult::Success)
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
*  @param[in] const uint32*     �ϊ����̕�����
*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
*  @param[in] uint16*           �ϊ���̕�����
*  @param[in] gu::uint64       �ϊ���̕�����̒���
*  @param[inout] Options&      �I�v�V����
*  @return    UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::ConvertUTF32ToUTF16(const uint32* sourceStart, uint64 sourceLength, uint16* targetStart, gu::uint64 targetLength, Options& options)
{
	const uint32* sourceIt = sourceStart;
	      uint16* targetIt = targetStart;
	const uint32* sourceEnd = sourceStart + sourceLength;
	const uint16* targetEnd = targetStart + targetLength;
	options.CharacterCount = 0;

	UnicodeConvertResult result = UnicodeConvertResult::Success;

	while (sourceIt < sourceEnd)
	{
		/*-------------------------------------------------------------------
		-       �o�̓o�b�t�@�̃T�C�Y�`�F�b�N
		---------------------------------------------------------------------*/
		if(targetIt >= targetEnd)
		{
			result = UnicodeConvertResult::TargetExhausted;
			break;
		}

		uint32 codePoint = *sourceIt;
		sourceIt++;

		/*-------------------------------------------------------------------
		-       UTF32��UTF16�ɕϊ����܂�.
		---------------------------------------------------------------------*/
		result = CodePointUTF32ToUTF16(codePoint, &targetIt, targetEnd, options);

		if (result != UnicodeConvertResult::Success)
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
*  @param[in]    const uint8**    �ϊ����̕�����
*  @param[in]    const uint8*     �ϊ����̏I�[����
*  @param[out]   uint32&          �ϊ���̕���
*  @param[inout] Options&        �I�v�V����
*  @return       UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::CodePointUTF8ToUTF32(const uint8** sourceStart, const uint8* sourceEnd, uint32& target,[[maybe_unused]] Options& options)
{
	auto source = *sourceStart;
	
	/*-------------------------------------------------------------------
	-       �ǉ��œǂރo�C�g�����擾���܂�. (�͈̓`�F�b�N)
	---------------------------------------------------------------------*/
	const unsigned short bytes  = static_cast<unsigned short>(UTF8_BYTE_MAP[*source]);
	
	if(source + bytes >= sourceEnd)
	{
		return UnicodeConvertResult::SourceExhausted;
	}
	
	/*-------------------------------------------------------------------
	-       UTF8�̃o�C�g�V�[�P���X�ł��邩���m�F
	---------------------------------------------------------------------*/
	if (!IsLegalUTF8(source, static_cast<uint64>(bytes) + 1))
	{
		//return UnicodeConvertResult::SourceIllegal;
	}

	/*-------------------------------------------------------------------
	-       UTF32�����̎擾
	---------------------------------------------------------------------*/
	uint32 result = 0;

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

	return UnicodeConvertResult::Success;
}

/*!**********************************************************************
*  @brief        UTF-16������UTF-32�ɕϊ����܂�.
*  @param[in]    const uint16**    �ϊ����̕�����
*  @param[in]    const uint16*     �ϊ����̏I�[����
*  @param[out]   gu::char32*      �ϊ���̕�����
*  @param[inout] Options&         �I�v�V����
*  @return       UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::CodePointUTF16ToUTF32(const uint16** sourceStart, const uint16* sourceEnd, uint32& target, Options& options)
{
	auto source = *sourceStart;
	
	uint32 char1 = *source;
	source++;

	/*-------------------------------------------------------------------
	-        �T���Q�[�g�y�A�������Ă���ꍇ��UTF32�̕ϊ��������s��
	---------------------------------------------------------------------*/
	if(SURROGATE_HIGH_START <= char1 && char1 <= SURROGATE_HIGH_END)
	{
		// �o�b�t�@�̏I�[�𒲂ׂ�
		if(source < sourceEnd)
		{
			uint32 char2 = *source; // 2�����ڂ��擾

			if(SURROGATE_LOW_START <= char2 && char2 <= SURROGATE_LOW_END)
			{
				target = ((char1 - SURROGATE_HIGH_START) << SURROGATE_HIGH_SHIFT) +
					      (char2 - SURROGATE_LOW_START) + SURROGATE_LOW_BASE;
				++source;
			}
			else if(IsStrictConversion(options))
			{
				// ��ʃT���Q�[�g������̂�, ���ʃT���Q�[�g���Ȃ��ꍇ�̓G���[
				return UnicodeConvertResult::SourceIllegal;
			}
		}
		else
		{
			// ���ʃT���Q�[�g��T���Ă���r���Ƀo�b�t�@�I�[�ɓ��B����ꍇ�̓G���[
			return UnicodeConvertResult::SourceExhausted;
		}
		return UnicodeConvertResult::SourceIllegal;
	}
	else
	{
		if (SURROGATE_LOW_START <= char1 && char1 <= SURROGATE_LOW_END)
		{
			return UnicodeConvertResult::SourceIllegal;
		}
	}

	*sourceStart = source;
	target       = char1;

	return UnicodeConvertResult::Success;
}

/*!**********************************************************************
*  @brief        UTF-32������UTF-8�ɕϊ����܂�.
*  @param[in]    const uint32     �ϊ����̕���
*  @param[in]    uint8**          �ϊ���̕�����|�C���^ (���ۂɊւ�炸��O�܂ŕ�����������Ă���.)
*  @param[out]   const uint8*     �ϊ���̏I�[����
*  @param[inout] Options&        �I�v�V����
*  @return       UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::CodePointUTF32ToUTF8(uint32 source, uint8** targetStart, const uint8* targetEnd, Options& options)
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
			return UnicodeConvertResult::SourceIllegal;
		}
		else
		{
			source = options.ReplacementChar;
			options.InvalidCharacterCount++;
		}
		return UnicodeConvertResult::SourceIllegal;
	}

	/*-------------------------------------------------------------------
	-       �͈̓`�F�b�N
	---------------------------------------------------------------------*/
	if(target + bytes > targetEnd)
	{
		return UnicodeConvertResult::TargetExhausted;
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
			*(--target) = static_cast<uint8>((source | BYTE_MARK) & BYTE_MASK);
			source >>= 6;
		}
		case 3: 
		{
			*(--target) = static_cast<uint8>((source | BYTE_MARK) & BYTE_MASK);
			source >>= 6;
		}
		case 2: 
		{
			*(--target) = static_cast<uint8>((source | BYTE_MARK) & BYTE_MASK);
			source >>= 6;
		}
		case 1: 
		{
			*(--target) = static_cast<uint8>(source | UTF8_FIRST_BYTE_MARK[bytes]);
		}
	}

	target      += bytes;
	*targetStart = target;
	return UnicodeConvertResult::Success;
}

/*!**********************************************************************
*  @brief        UTF-32������UTF-16�ɕϊ����܂�.
*  @param[in]    const uint32     �ϊ����̕���
*  @param[in]    const uint16**   �ϊ���̕�����|�C���^
*  @param[out]   const uint16*    �ϊ���̏I�[����
*  @param[inout] Options&        �I�v�V����
*  @return       UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::CodePointUTF32ToUTF16(uint32 source, uint16** targetStart, const uint16* targetEnd, Options& options)
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
				return UnicodeConvertResult::SourceExhausted;
			}
			else
			{
				*target = (uint16)options.ReplacementChar;
				target++;
				options.InvalidCharacterCount++;
			}
		}
		else
		{
			*target = (uint16)source;
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
			return UnicodeConvertResult::SourceExhausted;
		}
		else
		{
			*target = (uint16)options.ReplacementChar;
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
			return UnicodeConvertResult::TargetExhausted;
		}

		source -= SURROGATE_LOW_BASE;
		*target = (uint16)((source >> SURROGATE_HIGH_SHIFT) + SURROGATE_HIGH_START);
		target++;
		*target = (uint16)((source & SURROGATE_LOW_MASK) + SURROGATE_LOW_START);
		target++;
	}

	*targetStart = target;
	return UnicodeConvertResult::Success;
}

/*!**********************************************************************
*  @brief        UTF8�ɂ������s�o�C�g���l�����������񒷂��擾���܂�
*  @param[in]    const uint8* �Ώە�����
*  @param[in]    const uint64 �����񒷂�
*  @param[in]    const bool ���s�����Ƃ��ɃG���[���o�����ǂ���
*  @param[out]   uint64& ������
*  @return       bool
*************************************************************************/
UnicodeConvertResult UnicodeConverter::GetUTF8CharacterLength(const uint8* sourceStart, const uint64 sourceLength, uint64& outLength, const bool strict)
{
	/*-------------------------------------------------------------------
	-        �㑱�o�C�g�̎擾
	---------------------------------------------------------------------*/
	const uint8* source    = sourceStart;
	const uint8* sourceEnd = sourceStart + sourceLength;
	UnicodeConvertResult result = UnicodeConvertResult::Success;
	uint64 count = 0;

	/*-------------------------------------------------------------------
	-        ������̃J�E���g
	---------------------------------------------------------------------*/
	while (source < sourceEnd)
	{
		uint64 extraByteCount = 0;

		result = GetUTF8TrailingBytes(source, sourceEnd, extraByteCount, strict);
		if (result != UnicodeConvertResult::Success)
		{
			return result;
		}

		source += extraByteCount + 1;
		count++;
	}

	outLength = count;
	return result;
}

/*!**********************************************************************
*  @brief        UTF16�ɂ������s�o�C�g���l�����������񒷂��擾���܂�
*  @param[in]    const uint16* �Ώە�����
*  @param[in]    const uint64 �����񒷂�
*  @param[in]    const bool ���s�����Ƃ��ɃG���[���o�����ǂ���
*  @param[out]   uint64& ������
*  @return       UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::GetUTF16CharacterLength(const uint16* sourceStart, const uint64 sourceLength, uint64& outLength, const bool strict)
{
	const uint16* source    = sourceStart;
	const uint16* sourceEnd = sourceStart + sourceLength;
	outLength = 0;

	while (source < sourceEnd)
	{
		const uint16 char1 = *source;
		source++;

		// �T���Q�[�g�y�A�̏ꍇ
		if (SURROGATE_HIGH_START <= char1 && char1 <= SURROGATE_HIGH_END)
		{
			// �o�b�t�@�̏I�[�𒲂ׂ�
			if (source < sourceEnd)
			{
				const uint16 char2 = *source; // 2�����ڂ��擾

				if (SURROGATE_LOW_START <= char2 && char2 <= SURROGATE_LOW_END)
				{
					source++;
				}
				else if (strict)
				{
					// ��ʃT���Q�[�g������̂�, ���ʃT���Q�[�g���Ȃ��ꍇ�̓G���[
					return UnicodeConvertResult::SourceIllegal;
				}
			}
			else
			{
				// ���ʃT���Q�[�g��T���Ă���r���Ƀo�b�t�@�I�[�ɓ��B����ꍇ�̓G���[
				return UnicodeConvertResult::SourceExhausted;
			}
		}
		else if (SURROGATE_LOW_START <= char1 && char1 <= SURROGATE_LOW_END)
		{
			return UnicodeConvertResult::SourceIllegal;
		}

		outLength++;
	}

	return UnicodeConvertResult::Success;
}


/*!**********************************************************************
*  @brief        UTF8�����̌㑱�o�C�g�����擾���܂�
*  @param[in]    const uint8* �����̐擪�����̃|�C���^
*  @param[in]    const uint8* �o�b�t�@�̏I�[�����̃|�C���^
*  @param[out]   uint64& �㑱�o�C�g�� (ASCII������, �㑱�������ꍇ��0���i�[�����)
*  @param[in]    const bool true�̏ꍇ, �s�������̃`�F�b�N���s���܂� (�s�����������������ꍇ��SourceIllegal��Ԃ�)
*  @return       UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::GetUTF8TrailingBytes(const uint8* sourceStart, const uint8* sourceEnd, uint64& outExtraByteCount, const bool strict)
{
	/*-------------------------------------------------------------------
	-        �㑱�o�C�g�̎擾
	---------------------------------------------------------------------*/
	outExtraByteCount = 0;

	const uint64 extraBytes = static_cast<uint64>(UTF8_BYTE_MAP[*sourceStart]);
	outExtraByteCount = extraBytes;

	/*-------------------------------------------------------------------
	-        �͈̓`�F�b�N
	---------------------------------------------------------------------*/
	if (sourceStart + extraBytes >= sourceEnd)
	{
		return UnicodeConvertResult::SourceExhausted;
	}

	/*-------------------------------------------------------------------
	-        �����UTF8�����ł��邩
	---------------------------------------------------------------------*/
	if (strict && IsLegalUTF8(sourceStart, extraBytes + 1))
	{
		return UnicodeConvertResult::SourceIllegal;
	}

	return UnicodeConvertResult::Success;
}

/*!**********************************************************************
*  @brief     UTF16 �̃T���Q�[�g�y�A�ł��邩���m�F����
*  @param[in] const void* �`�F�b�N�Ώۂ̐擪�����̃|�C���^
*  @param[in] const void* �`�F�b�N�Ώۂ̏I�[�����̃|�C���^
*  @param[in] const bool true�̏ꍇ, �s�������̃`�F�b�N���s���܂� (�s�����������������ꍇ��SourceIllegal��Ԃ�)
*  @param[out] bool* �T���Q�[�g�y�A�ł��邩�ǂ���
*  @return    UnicodeConvertResult �ϊ�����
*************************************************************************/
UnicodeConvertResult UnicodeConverter::CheckUTF16Surrogate(const uint16* sourceStart, const uint16* sourceEnd, const bool strict, bool* outSurrogate)
{
	*outSurrogate = false;

	const uint16* source = sourceStart;
	while (source < sourceEnd)
	{
		const uint16 char1 = *source;
		source++;

		// �T���Q�[�g�y�A�̏ꍇ
		if (SURROGATE_HIGH_START <= char1 && char1 <= SURROGATE_HIGH_END)
		{
			// �o�b�t�@�̏I�[�𒲂ׂ�
			if (source < sourceEnd)
			{
				const uint16 char2 = *source; // 2�����ڂ��擾

				if (SURROGATE_LOW_START <= char2 && char2 <= SURROGATE_LOW_END)
				{
					*outSurrogate = true;
					source++;
				}
				else if (strict)
				{
					// ��ʃT���Q�[�g������̂�, ���ʃT���Q�[�g���Ȃ��ꍇ�̓G���[
					return UnicodeConvertResult::SourceIllegal;
				}
			}
			else
			{
				// ���ʃT���Q�[�g��T���Ă���r���Ƀo�b�t�@�I�[�ɓ��B����ꍇ�̓G���[
				return UnicodeConvertResult::SourceExhausted;
			}
		}
		else if (strict && SURROGATE_LOW_START <= char1 && char1 <= SURROGATE_LOW_END)
		{
			return UnicodeConvertResult::SourceIllegal;
		}
	}

	return UnicodeConvertResult::Success;

}

#pragma endregion Public Function

#pragma region Private Function
/*!**********************************************************************
*  @brief        �K�؂�UTF8�R�[�f�b�N���ǂ���
*  @param[in]    const uint8* �ϊ���
*  @param[in]    const uint64 �����񒷂�
*  @return       bool
*************************************************************************/
bool UnicodeConverter::IsLegalUTF8(const uint8* sourceStart, const uint64 length)
{
	const uint8* source = sourceStart;

	switch(length)
	{
		default:
		{
			return false;
		}
		case 4:
		{
			if ((0xF8 & *source) == 0xF0 && *source <= 0xF4)
			{
				// Each of the following bytes is a value
				 // between 0x80 and 0xBF
				if (((0xC0 & *(source + 1)) != 0x80) || ((0xC0 & *(source + 2)) != 0x80)
					|| ((0xC0 & *(source + 3)) != 0x80))
				{
					return false;
				}

				// If the first byte of the sequence is 0xF0
				// then the first continuation byte must be between 0x90 and 0xBF
				// otherwise, if the byte is 0xF4
				// then the first continuation byte must be between 0x80 and 0x8F
				if (*source == 0xF0)
				{
					if (*(source + 1) < 0x90 || *(source + 1) > 0xBF)
					{
						return false;
					}
				}
				else if (*source == 0xF4)
				{
					if (*(source + 1) < 0x80 || *(source + 1) > 0x8F)
					{
						return false;
					}
				}
			}
		}
		case 3:
		{
			// Each of the following bytes starts with
		    // 0b10xxxxxx in a valid string
			if (((0xC0 & *(source + 1)) != 0x80) || ((0xC0 & *(source + 2)) != 0x80))
			{
				return false;
			}

			// If the first byte of the sequence is 0xE0
			// then the first continuation byte must be between 0xA0 and 0xBF
			// otherwise, if the byte is 0xF4
			// then the first continuation byte must be between 0x80 and 0x9F
			if (*source == 0xE0)
			{
				if (*(source + 1) < 0xA0 || *(source + 1) > 0xBF)
				{
					return false;
				}
			}
			else if (*source == 0xED)
			{
				if (*(source + 1) > 0x9F)
				{
					return false;
				}
			}
		}
		case 2:
		{
			// The following byte starts with 0b10xxxxxx in a valid string
			if ((0xC0 & *(source + 1)) != 0x80)
			{
				return false;
			}
		}
		case 1:
		{
			break;
		}
	}

	return true;
}
#pragma endregion Private Function