//////////////////////////////////////////////////////////////////////////////////
///             @file   GUStringUtilityhpp.hpp
///             @brief  temp
///             @author �Q�l���� : https://github.com/LuminoEngine/Lumino/blob/main/lumino/LuminoCore/include/LuminoCore/Base/StringHelper.ipp
///             @date   2024/01/21 2:42:12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_STRING_UTILITY_HPP
#define GU_STRING_UTILITY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::details::string
{
	enum class NumberConversionResult
	{
		Success        = 0,
		ArgumentsError = 1,
		FormatError    = 2,
		Overflow       = 3,
	};

	/****************************************************************************
	*				  			   GUStringUtility
	*************************************************************************//**
	*  @class     GUStringUtility
	*  @brief     String base function
	*****************************************************************************/
	class StringUtility : public NonCopyAndMove
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  ������̒������擾���܂�. 
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 Length(const Char* string);

		/*----------------------------------------------------------------------
		*  @brief :  ������̃R�s�[���s���܂�
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static void Copy(Char* destination, const uint64 destLength, const Char* source);

		/*----------------------------------------------------------------------
		*  @brief :  ��������r���܂�
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 Compare(const Char* left, uint64 leftLength, const Char* right, uint64 rightLength, 
			uint64 count, const bool useCaseSensitivity) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  ������Ɉ�v����ŏ��̃C���f�b�N�X��Ԃ��܂�. 
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 Find(
			const Char* left, uint64 leftLength,
			const Char* right, uint64 rightLength,
			uint64 startIndex, const bool useCaseSensitivity) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  ������Ɉ�v����ŏ��̃C���f�b�N�X��Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 ReverseFind(
			const Char* left, uint64 leftLength,
			const Char* right, uint64 rightLength,
			uint64 startIndex, uint64 sortCount, const bool useCaseSensitivity) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  ������̕���������𒊏o���܂�
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static void SubString(const Char* string, const uint64 stringLength, uint64 startIndex, uint64 count, const Char** outBegin, const Char** outEnd);

		/*----------------------------------------------------------------------
		*  @brief : ������̐擪����w�肵���������𒊏o���܂�. 
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static void Left(const Char* string, const uint64 count, const Char** outBegin, const Char** outEnd);

		/*----------------------------------------------------------------------
		*  @brief : ������̖�������w�肵���������𒊏o���܂�.
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static void Right(const Char* string, const uint64 count, const Char** outBegin, const Char** outEnd);

		/*----------------------------------------------------------------------
		*  @brief :  ������Ɉ�v����ŏ��̃C���f�b�N�X��Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static bool IsFirstMatch(const Char* left, uint64 leftLength, const Char* right, uint64 rightLength, const bool useCaseSensitivity)
		{
			return Compare(left, leftLength, right, rightLength, cs) == 0;
		}

		/*----------------------------------------------------------------------
		*  @brief :  ������̖������w�肵��������ƈ�v���邩��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static bool IsLastMatch(const Char* left, uint64 leftLength, const Char* right, uint64 rightLength, const bool useCaseSensitivity);

		/*----------------------------------------------------------------------
		*  @brief :  ������啶���ɐ؂�ւ��܂�
		/*----------------------------------------------------------------------*/
		template<class Char>
		static Char ToUpper(Char ch) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  �������������ɐ؂�ւ��܂�
		/*----------------------------------------------------------------------*/
		template<class Char>
		static Char ToLower(Char ch) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  �󔒍s��
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static bool IsSpace(const Char ch) noexcept 
		{
			return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f' || ch == '\v';
		}
		
		template<typename Char>
		static bool IsDigit(const Char ch) noexcept
		{
			return '0' <= ch && ch <= '9';
		}

		/*----------------------------------------------------------------------
		*  @brief :  �󔒍s��
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static void Trim(const Char* begin, uint64 length, const Char** outBegin, uint64* outLength)
		{
			if (begin     == nullptr) { return; }
			if (outBegin  == nullptr) { return; }
			if (outLength == nullptr) { return; }

			if (length == 0)
			{
				*outBegin  = begin;
				*outLength = 0;
				return;
			}

			const Char* end = begin + length;

			// left����
			while (*begin)
			{
				if (!IsSpace(*begin)) { break; }
				++begin;
			}

			// right����
			while (begin < end)
			{
				if (!IsSpace(*(end - 1))) { break; }
				--end;
			}

			*outBegin = begin;
			*outLength = (uint64)(end - begin);
		}
#pragma region Convert number
		template<typename Char>
		static int8 ToInt8(const Char* string, uint64 stringLength, uint64 radix, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			uint8 number = 0;
			NumberConversionResult result = StringToNumberInternal<Char, int8, uint8, MIN_INT8, MAX_INT8, MAX_UINT8>(string, stringLength, radix, false, outEndPointer, &number);
			if (outResult != nullptr)
			{
				*outResult = result;
			}
			return (int8)number;
		}

		template<typename Char>
		static int16 ToInt16(const Char* string, uint64 stringLength, uint64 radix, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			uint16 number = 0;
			NumberConversionResult result = StringToNumberInternal<Char, int16, uint16, MIN_INT16, MAX_INT16, MAX_UINT16>(string, stringLength, radix, false, outEndPointer, &number);
			if (outResult != nullptr)
			{
				*outResult = result;
			}
			return (int16)number;
		}

		template<typename Char>
		static int32 ToInt32(const Char* string, uint64 stringLength, uint64 radix, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			uint32 number = 0;
			NumberConversionResult result = StringToNumberInternal<Char, int32, uint32, MIN_INT32, MAX_INT32, MAX_UINT32>(string, stringLength, radix, false, outEndPointer, &number);
			if (outResult != nullptr)
			{
				*outResult = result;
			}
			return (int32)number;
		}

		template<typename Char>
		static int64 ToInt64(const Char* string, uint64 stringLength, uint64 radix, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			uint64 number = 0;
			NumberConversionResult result = StringToNumberInternal<Char, int64, uint64, MIN_INT64, MAX_INT64, MAX_UINT64>(string, stringLength, radix, false, outEndPointer,&number);
			if (outResult != nullptr)
			{
				*outResult = result;
			}
			return (int64)number;
		}

		template<typename Char>
		static uint8 ToUInt8(const Char* string, uint64 stringLength, uint64 radix, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			uint8 number = 0;
			NumberConversionResult result = StringToNumberInternal<Char, int8, uint8, MIN_INT8, MAX_INT8, MAX_UINT8>(string, stringLength, radix, true, outEndPointer, &number);
			if (outResult != nullptr)
			{
				*outResult = result;
			}
			return number;
		}

		template<typename Char>
		static uint16 ToUInt16(const Char* string, uint64 stringLength, uint64 radix, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			uint16 number = 0;
			NumberConversionResult result = StringToNumberInternal<Char, int16, uint16, MIN_INT16, MAX_INT16, MAX_UINT16>(string, stringLength, radix, true, outEndPointer, &number);
			if (outResult != nullptr)
			{
				*outResult = result;
			}
			return number;
		}

		template<typename Char>
		static uint32 ToUInt32(const Char* string, uint64 stringLength, uint64 radix, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			uint32 number = 0;
			NumberConversionResult result = StringToNumberInternal<Char, int32, uint32, MIN_INT32, MAX_INT32, MAX_UINT32>(string, stringLength, radix, true, outEndPointer,&number);
			if (outResult != nullptr)
			{
				*outResult = result;
			}
			return number;
		}

		template<typename Char>
		static uint64 ToUInt64(const Char* string, uint64 stringLength, uint64 radix, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			uint64 number = 0;
			NumberConversionResult result = StringToNumberInternal<Char, int64, uint64, MIN_INT64, MAX_INT64, MAX_UINT64>(string, stringLength, radix, true, outEndPointer, &number);
			if (outResult != nullptr)
			{
				*outResult = result;
			}
			return number;
		}

		template<typename Char>
		static double ToDouble(const Char* string, uint64 stringLength, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			if (outResult != nullptr) { *outResult = NumberConversionResult::Success; }
			if (string == nullptr)
			{
				if (outResult != nullptr) { *outResult = NumberConversionResult::ArgumentsError; }
				return 0.0;
			}

			stringLength = stringLength == NPOS ? Length(string) : stringLength;

			if (stringLength >= 512)
			{
				if (outResult != nullptr)
				{
					*outResult != NumberConversionResult::ArgumentsError;
				}
				return 0.0;
			}

			// �W���֐��� strtod �͒�����n���Ȃ��̂ňꎞ�o�b�t�@�ɃR�s�[���ďI�[\0�ɂ���B
			// �ő咷���͂Ƃ肠���� 512�B
			// IEEE �`���ł͉������̌����� 2^53=9007199254740992 ��16���ŁA�w������ 308�B
			// IBM �`���ł͉������̌����� 2^24=16777216 ��8���ŁA�w������ 16^63�ŁA7.237005577332262213973186563043e+75�B
			// 0 �� 308 ���ׂ��邱�ƂɂȂ邪�A512 �������̃T�C�Y������Ώ\���B
			char temp[512] = {};
			CopySimpleAsciiString(temp, 512, string, stringLength);
			temp[stringLength] = '0';

			char* end    = nullptr;
			double value = strtod(temp, &end);

			if (outEndPointer != nullptr)
			{
				*outEndPointer = string + (end - temp);
			}

			if (value == HUGE_VAL || value == -HUGE_VAL)
			{
				if (outResult != nullptr)
				{
					*outResult = NumberConversionResult::Overflow;
				}

				return value;
			}

			return value;
		}

		template<typename Char>
		static double ToFloat(const Char* string, uint64 stringLength, const Char** outEndPointer, NumberConversionResult* outResult)
		{
			double value = ToDouble(string, stringLength, outEndPointer, outResult);

			if (outResult != nullptr &&
				*outResult == NumberConversionResult::Success &&
				-FLT_MAX <= value && value <= FLT_MAX)
			{
				*outResult = NumberConversionResult::Overflow;
			}

			return (float)value;
		}

#pragma endregion Convert number
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		static constexpr uint64 NPOS = static_cast<uint64>(-1);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		template<typename Char>
		static int32 StringCompare(const Char* left, const Char* right, uint64 count) noexcept
		{
			for (; 0 < count; --count, ++left, ++right)
			{
				if (*left != *right)
				{
					return (*left < *right) ? -1 : 1;
				}
			}
			return 0;
		}

		template<typename Char>
		static int32 StringICompare(const Char* left, const Char* right, uint64 count) noexcept
		{
			for (; 0 < count; --count, ++left, ++right)
			{
				const auto c1 = ToUpper(*left);
				const auto c2 = ToLower(*right);
				if (c1 != c2)
				{
					return (c1 < c2) ? -1 : 1;
				}
			}
			return 0;
		}

		/*----------------------------------------------------------------------
		*  @brief :�@������ -> ���l�ɕϊ�����
		/*----------------------------------------------------------------------*/
		template<typename Char, typename SignedType, typename UnsignedType, SignedType SignedMin = 0, SignedType SignedMax = 0, UnsignedType UnsignedMax=0>
		static NumberConversionResult StringToNumberInternal
		(
			const Char*  string,
			uint64 stringLength,
			uint64 radix,
			bool   useUnsigned,
			const Char** outEndPointer,
			UnsignedType* outNumber
		);

		template<class DestinationType, class SourceType>
		static void CopySimpleAsciiString(DestinationType* destination, uint64 destinationLength, const SourceType* source, uint64 sourceLength)
		{
			while (destinationLength && sourceLength && *source)
			{
				destinationLength--;
				sourceLength--;
				*destination++ = static_cast<DestinationType*>(*source++);
			}
		}

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

#pragma region Implement
	/*----------------------------------------------------------------------
	*  @brief :  ��������r���܂�
	/*----------------------------------------------------------------------*/
	template<typename Char>
	uint64 StringUtility::Compare(const Char* left, uint64 leftLength, const Char* right, uint64 rightLength, uint64 count, const bool useCaseSensitivity) noexcept
	{
		if (left == nullptr || right == nullptr)
		{
			if      (left == nullptr && right == nullptr) { return 0; }
			else if (left == nullptr)                     { return StringUtility::NPOS; }
			else                                          { return 1; }
		}

		/*-------------------------------------------------------------------
		-       �K�v������Ε������J�E���g
		---------------------------------------------------------------------*/
		leftLength   = (leftLength == NPOS) ? Length(left) : leftLength;
		rightLength  = (rightLength == NPOS) ? Length(right) : rightLength;
		auto minCount = leftLength < rightLength ? leftLength : rightLength;
		auto maxCount = leftLength < rightLength ? rightLength : leftLength ;
		
		if (count == NPOS)
		{
			count = maxCount;
		}
		else
		{
			maxCount = count;
			count    = minCount < count ? minCount : count;
		}
		
		if (useCaseSensitivity)
		{
			while (count > 0)
			{
				if (*left != *right)
				{
					return (*left) - (*right);
				}
				++left;
				++right;
				--count;
			}
		}
		else
		{
			while (count > 0)
			{
				if (ToUpper(*left) != ToUpper(*right))
				{
					return (ToUpper(*left) - ToUpper(*right));
				}

				++left;
				++right;
				--count;
			}
		}

		if (minCount < maxCount)
		{
			return leftLength - rightLength;
		}

		return 0;
	}
	/*----------------------------------------------------------------------
	*  @brief :  ������̒������擾���܂�.
	/*----------------------------------------------------------------------*/
	template<typename Char>
	static uint64 StringUtility::Length(const Char* string)
	{
		Check(string != nullptr);

		const Char* s;
		for (s = string; *s; ++s);
		return (s - string);
	}

	/*----------------------------------------------------------------------
	*  @brief :  ������̃R�s�[���s���܂�
	/*----------------------------------------------------------------------*/
	template<typename Char>
	static void StringUtility::Copy(Char* destination, const uint64 destLength, const Char* source)
	{
		Check(destination != nullptr);
		Check(source      != nullptr);

		if (destLength == 0) { return; }
		destLength--;

		while (destLength && *source)
		{
			destLength--;
			*destination++ = *source++;
		}
		*destination = '\0';
	}

	/*----------------------------------------------------------------------
	*  @brief :  ������Ɉ�v����ŏ��̃C���f�b�N�X��Ԃ��܂�.
	/*----------------------------------------------------------------------*/
	template<class Char>
	uint64 StringUtility::Find(
		const Char* left, uint64 leftLength,
		const Char* right, uint64 rightLength,
		uint64 startIndex, const bool useCaseSensitivity)  noexcept
	{
		/*-------------------------------------------------------------------
		-        �����񂪖�����Ζ����l�����
		---------------------------------------------------------------------*/
		if (left == nullptr || right == nullptr) { return NPOS; }
		if (*left == 0)                          { return NPOS; }

		/*-------------------------------------------------------------------
		-       �C���f�b�N�X���L����
		---------------------------------------------------------------------*/
		leftLength  = (leftLength  == NPOS) ? Length(left)  : leftLength;
		rightLength = (rightLength == NPOS) ? Length(right) : rightLength;
		
		if (leftLength  <= startIndex) { return NPOS; }
		if (rightLength <= 0)          { return NPOS; }

		/*-------------------------------------------------------------------
		-       ������̌���
		---------------------------------------------------------------------*/
		auto pos = left + startIndex;
		const auto end = left + leftLength;

		// �啶��, ����������ʂ���ꍇ
		if (useCaseSensitivity)
		{
			for (; pos < end && *pos; ++pos)
			{
				if (*pos != *right) { continue; }

				if (StringCompare(pos, right, rightLength) == 0)
				{
					return (uint64)(pos - left);
				}
			}
		}
		else
		{
			for (; pos < end && *pos; ++pos)
			{
				if (*pos != *right) { continue; }

				if (StringICompare(pos, right, rightLength) == 0)
				{
					return (uint64)(pos - left);
				}
			}
		}

		return StringUtility::NPOS;
	}

	/*----------------------------------------------------------------------
	*  @brief :  ������Ɉ�v����ŏ��̃C���f�b�N�X��Ԃ��܂�.
	/*----------------------------------------------------------------------*/
	template<class Char>
	uint64 StringUtility::ReverseFind(
		const Char* left, uint64 leftLength,
		const Char* right, uint64 rightLength,
		uint64 startIndex, uint64 sortCount, const bool useCaseSensitivity) noexcept 
	{
		const Char nullString[] = { '\0' };

		/*-------------------------------------------------------------------
		-       ���O����
		---------------------------------------------------------------------*/
		left  = (left  == nullptr) ? nullString : left;
		right = (right == nullptr) ? nullString : right;
		
		leftLength  = static_cast<uint64>((leftLength  == NPOS) ? Length(left) : leftLength);
		rightLength = static_cast<uint64>((rightLength == NPOS) ? Length(right) : rightLength);

		startIndex = (startIndex == NPOS) ? (leftLength - 1) : startIndex;

		/*-------------------------------------------------------------------
		-       �����񂪖�����Ζ����u�𓾂�
		---------------------------------------------------------------------*/
		if (leftLength == 0 && (startIndex == NPOS || startIndex == 0))
		{
			return (rightLength == 0) ? 0 : NPOS;
		}

		/*-------------------------------------------------------------------
		-       �C���f�b�N�X���L����
		---------------------------------------------------------------------*/
		if (startIndex < 0)          { return StringUtility::NPOS; }
		if (startIndex >= leftLength) { return StringUtility::NPOS; }

		/*-------------------------------------------------------------------
		-      �����͈͂����킩�ǂ���
		---------------------------------------------------------------------*/
		// ������������0�̏ꍇ�͕K�������J�n�ʒu�Ńq�b�g����
		if (rightLength == 0 && sortCount != NPOS && startIndex - sortCount + 1 >= 0)
		{
			return startIndex;
		}

		      auto pos = left + startIndex;                                   // �����͈̖͂����̕���
		const auto end = (sortCount == NPOS) ? left : pos - (sortCount - 1); // �����͈͂̐擪�̕���
		if (end > pos) { return StringUtility::NPOS; }                       // �����Ɛ擪���t�]���Ă��Ȃ�

		// �����͈͂������������������Ȃ��ꍇ�͌�����͂����Ȃ�
		if (pos - end < (int64)(rightLength - 1)) { return StringUtility::NPOS; }

		pos -= (rightLength - 1);

		/*-------------------------------------------------------------------
		-       ������̌���
		---------------------------------------------------------------------*/
		// �啶��, ����������ʂ���ꍇ
		if (useCaseSensitivity)
		{
			while (pos >= end)
			{
				if (StringCompare(pos, right, rightLength) == 0)
				{
					return (uint64)(pos - left);
				}
				--pos;
			}
		}
		else
		{
			while (pos >= end)
			{
				if (StringICompare(pos, right, rightLength) == 0)
				{
					return (uint64)(pos - left);
				}
				--pos;
			}
		}

		return NPOS;
	}

	/*----------------------------------------------------------------------
	*  @brief :  ������̕���������𒊏o���܂�
	/*----------------------------------------------------------------------*/
	template<typename Char>
	void StringUtility::SubString(const Char* string, const uint64 stringLength, uint64 startIndex, 
		uint64 count, const Char** outBegin, const Char** outEnd)
	{
		if (startIndex == NPOS) { startIndex = 0; }
		if (count      == NPOS) { count = stringLength - startIndex;}
		if (startIndex + count > stringLength)
		{
			count = stringLength - startIndex;
		}
		if (startIndex > stringLength) { count = 0; }

		if (startIndex == 0 && count == stringLength)
		{
			*outBegin = string;
			*outEnd   = string + stringLength;
		}
		else
		{
			*outBegin = string + startIndex;
			*outEnd   = string + startIndex + count;
		}
	}

	/*----------------------------------------------------------------------
	*  @brief : ������̐擪����w�肵���������𒊏o���܂�.
	/*----------------------------------------------------------------------*/
	template<typename Char>
	void StringUtility::Left(const Char* string, uint64 count, const Char** outBegin, const Char** outEnd)
	{
		if (stringLength == NPOS) { count = 0; }

		auto length = Length(string);
		length      = min(length, count);
		*outBegin   = string;
		*outEnd     = string + length;
	}

	/*----------------------------------------------------------------------
	*  @brief : ������̖�������w�肵���������𒊏o���܂�.
	/*----------------------------------------------------------------------*/
	template<typename Char>
	void StringUtility::Right(const Char* string, uint64 count, const Char** outBegin, const Char** outEnd)
	{
		if (stringLength == NPOS) { count = 0; }

		auto length = Length(string);
		length      = min(length, count);
		*outBegin   = string + length - count;
		*outEnd     = string + length;
	}

	/*----------------------------------------------------------------------
	*  @brief :  ������̖������w�肵��������ƈ�v���邩��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	template<typename Char>
	bool StringUtility::IsLastMatch(const Char* left, uint64 leftLength, const Char* right, uint64 rightLength, const bool useCaseSensitivity)
	{
		/*-------------------------------------------------------------------
		-       ������NPOS�̏ꍇ��\0�܂ŃJ�E���g����
		---------------------------------------------------------------------*/
		leftLength  = leftLength  == NPOS ? Length(left)  : leftLength;
		rightLength = rightLength == NPOS ? Length(right) : rightLength;

		if (rightLength <= 0)         { return true; }
		if (leftLength < rightLength) { return false; }

		/*-------------------------------------------------------------------
		-       ��v���Ă��邩�̔���
		---------------------------------------------------------------------*/
		const Char* positionFirst  = left  + leftLength  - 1;
		const Char* positionSecond = right + rightLength - 1;

		/*-------------------------------------------------------------------
		-       �啶���E����������ʂ���
		---------------------------------------------------------------------*/
		if (useCaseSensitivity)
		{
			while (right <= positionSecond)
			{
				if (positionFirst < left || *positionFirst != *positionSecond)
				{
					return false;
				}

				positionFirst--;
				positionSecond--;
			}
			return true;
		}
		/*-------------------------------------------------------------------
		-       �啶���E����������ʂ��Ȃ�
		---------------------------------------------------------------------*/
		else
		{
			while (right <= positionSecond)
			{
				if (positionFirst < left || ToUpper(*positionFirst) != ToUpper(*positionSecond))
				{
					return false;
				}

				positionFirst--;
				positionSecond--;
			}
			return true;
		}
	}

	/*----------------------------------------------------------------------
	*  @brief :  ������啶���ɐ؂�ւ��܂�
	/*----------------------------------------------------------------------*/
	template<class Char>
	Char StringUtility::ToUpper(Char ch) noexcept 
	{
		if ('a' <= ch && ch <= 'z')
		{
			return 'A' + (ch - 'a');
		}

		return ch;
	}

	/*----------------------------------------------------------------------
	*  @brief :  �������������ɐ؂�ւ��܂�
	/*----------------------------------------------------------------------*/
	template<class Char>
	Char StringUtility::ToLower(Char ch)  noexcept
	{
		if ('A' <= ch && ch <= 'Z')
		{
			return 'a' + (ch - 'A');
		}
		return ch;
	}

	template<typename Char, typename SignedType, typename UnsignedType, SignedType SignedMin, SignedType SignedMax, UnsignedType UnsignedMax>
	NumberConversionResult StringUtility::StringToNumberInternal
	(
		const Char*  string,
		uint64 stringLength,
		uint64 radix,
		bool   useUnsigned,
		const Char** outEndPointer,
		UnsignedType* outNumber
	)
	{
		/*-------------------------------------------------------------------
		-       OutNumber�̏�����
		---------------------------------------------------------------------*/
		if (outNumber != nullptr) 
		{
			*outNumber = 0; 
		}

		/*-------------------------------------------------------------------
		-       ��`�F�b�N
		---------------------------------------------------------------------*/
		if (!(radix == 0 || radix == 2 || radix == 8 || radix == 10 || radix == 16))
		{
			return NumberConversionResult::ArgumentsError;
		}

		/*-------------------------------------------------------------------
		-       �󔒂��X�L�b�v����
		---------------------------------------------------------------------*/
		const Char* pos = string;
		const Char* end = string + (stringLength == NPOS ? Length(string) : stringLength);

		while (IsSpace(*pos)) { ++pos; }

		/*-------------------------------------------------------------------
		-       �����̐������m�F����
		---------------------------------------------------------------------*/
		bool isNegative = false;
		if (*pos == '-')
		{
			isNegative = true;
			++pos;
		}
		else if (*pos == '+')
		{
			++pos;
		}

		// ���������Ȃ������ꍇ
		if (pos >= end)
		{
			return NumberConversionResult::FormatError;
		}

		/*-------------------------------------------------------------------
		-       ���0�̏ꍇ�͎������ʂ��s��
		---------------------------------------------------------------------*/
		if (radix == 0)
		{
			if (pos[0] != '0')
			{
				radix = 10;
			}
			else if (pos[1] == 'x' || pos[1] == 'X')
			{
				radix = 16;
			}
			else
			{
				radix = 8;
			}
		}

		/*-------------------------------------------------------------------
		-       ��ɂ���ăv���t�B�b�N�X���X�L�b�v����
		---------------------------------------------------------------------*/
		if (radix == 8)
		{
			if (pos[0] == '0')
			{
				++pos;

				if (pos >= end)
				{
					if (outEndPointer != nullptr)
					{
						*outEndPointer = pos;
					}
					*outEndPointer = 0;
					return NumberConversionResult::Success;
				}
			}
		}
		else if (radix == 16)
		{
			if (pos[0] == '0' && (pos[1] == 'x' || pos[1] == 'X'))
			{
				++pos;
				++pos;
				if (pos >= end)
				{
					return NumberConversionResult::FormatError;
				}
			}
		}

		/*-------------------------------------------------------------------
		-       ��ɂ���ăv���t�B�b�N�X���X�L�b�v����
		---------------------------------------------------------------------*/
		UnsignedType value = 0;
		UnsignedType overflowMax = UnsignedMax / (UnsignedType)radix; // ��Z���悤�Ƃ���Ƃ�, ���̒l�ȏ�ł���Ύ��̏�Z�ŃI�[�o�[�t���[����
		UnsignedType radixMax    = UnsignedMax % radix;
		uint64       count = 0;
		bool         isOverflow = false;

		for (;;)
		{
			UnsignedType digit = 0;

			if ('0' <= (*pos) && (*pos) <= '9')
			{
				digit = static_cast<UnsignedType>(*pos - '0');
			}
			else if ('A' <= *pos && *pos <= 'F')
			{
				digit = static_cast<UnsignedType>(*pos - 'A' + 10);
			}
			else if('a' <= *pos && *pos <= 'f')
			{
				digit = static_cast<UnsignedType>(*pos - 'a' + 10);
			}
			else
			{
				break;
			}

			// ����傫�������������� (10�i���Ȃ̂�)
			if (digit >= (UnsignedType)radix)
			{
				return NumberConversionResult::FormatError;
			}

			++count;

			// �v�Z����O�ɃI�[�o�[�t���[���m�F����
			if (value < overflowMax ||
				(value == overflowMax && digit <= radixMax))
			{
				value = value * (UnsignedType)radix + digit;
			}
			else
			{
				isOverflow = true;
			}

			// ���̕�����
			++pos;
			if (pos >= end) { break; }
		}

		if (outEndPointer != nullptr)
		{
			*outEndPointer = pos;
		}

		/*-------------------------------------------------------------------
		-       �I�[�o�[�t���[���Ă�����ő�l�Ɋۂ߂Ă���
		---------------------------------------------------------------------*/
		NumberConversionResult result = NumberConversionResult::Success;
		if (isOverflow)
		{
			value = UnsignedMax;
			result = NumberConversionResult::Overflow;
		}
		else if (!useUnsigned)
		{
			if (isNegative && value > (UnsignedType)(-SignedMin))
			{
				value = (UnsignedType)(-SignedMin);
				result = NumberConversionResult::Overflow;
			}
			else if (!isNegative && value > (UnsignedType)SignedMax)
			{
				value = SignedMax;
				result = NumberConversionResult::Overflow;
			}
		}

		/*-------------------------------------------------------------------
		-       �}�C�i�X�����ł���ꍇ��, Singed�l��unsigned�Ƃ��ĕԂ�, �Ăяo������signed�ŃL���X�g���ĕԂ�
		---------------------------------------------------------------------*/
		if (isNegative)
		{
			value = (UnsignedType)(-(SignedType)value);
		}

		if (outNumber != nullptr)
		{
			*outNumber = value;
		}
		return result;

	}
#pragma endregion Implement
}

#endif