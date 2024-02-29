//////////////////////////////////////////////////////////////////////////////////
///             @file   GUStringUtilityhpp.hpp
///             @brief  temp
///             @author 参考資料 : https://github.com/LuminoEngine/Lumino/blob/main/lumino/LuminoCore/include/LuminoCore/Base/StringHelper.ipp
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
		*  @brief :  文字列の長さを取得します. 
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 Length(const Char* string);

		/*----------------------------------------------------------------------
		*  @brief :  文字列のコピーを行います
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static void Copy(Char* destination, const uint64 destLength, const Char* source);

		/*----------------------------------------------------------------------
		*  @brief :  文字列を比較します
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 Compare(const Char* left, uint64 leftLength, const Char* right, uint64 rightLength, 
			uint64 count, const bool useCaseSensitivity) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  文字列に一致する最初のインデックスを返します. 
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 Find(
			const Char* left, uint64 leftLength,
			const Char* right, uint64 rightLength,
			uint64 startIndex, const bool useCaseSensitivity) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  文字列に一致する最初のインデックスを返します.
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 ReverseFind(
			const Char* left, uint64 leftLength,
			const Char* right, uint64 rightLength,
			uint64 startIndex, uint64 sortCount, const bool useCaseSensitivity) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  文字列の部分文字列を抽出します
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static void SubString(const Char* string, const uint64 stringLength, uint64 startIndex, uint64 count, const Char** outBegin, const Char** outEnd);

		/*----------------------------------------------------------------------
		*  @brief : 文字列の先頭から指定した文字数を抽出します. 
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static void Left(const Char* string, const uint64 count, const Char** outBegin, const Char** outEnd);

		/*----------------------------------------------------------------------
		*  @brief : 文字列の末尾から指定した文字数を抽出します.
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static void Right(const Char* string, const uint64 count, const Char** outBegin, const Char** outEnd);

		/*----------------------------------------------------------------------
		*  @brief :  文字列に一致する最初のインデックスを返します.
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static bool IsFirstMatch(const Char* left, uint64 leftLength, const Char* right, uint64 rightLength, const bool useCaseSensitivity)
		{
			return Compare(left, leftLength, right, rightLength, cs) == 0;
		}

		/*----------------------------------------------------------------------
		*  @brief :  文字列の末尾が指定した文字列と一致するかを返します
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static bool IsLastMatch(const Char* left, uint64 leftLength, const Char* right, uint64 rightLength, const bool useCaseSensitivity);

		/*----------------------------------------------------------------------
		*  @brief :  文字を大文字に切り替えます
		/*----------------------------------------------------------------------*/
		template<class Char>
		static Char ToUpper(Char ch) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  文字を小文字に切り替えます
		/*----------------------------------------------------------------------*/
		template<class Char>
		static Char ToLower(Char ch) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  空白行か
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
		*  @brief :  空白行か
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

			// left部分
			while (*begin)
			{
				if (!IsSpace(*begin)) { break; }
				++begin;
			}

			// right部分
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

			// 標準関数の strtod は長さを渡せないので一時バッファにコピーして終端\0にする。
			// 最大長さはとりあえず 512。
			// IEEE 形式では仮数部の桁数は 2^53=9007199254740992 で16桁で、指数部は 308。
			// IBM 形式では仮数部の桁数は 2^24=16777216 で8桁で、指数部は 16^63で、7.237005577332262213973186563043e+75。
			// 0 は 308 個並べられることになるが、512 文字分のサイズがあれば十分。
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
		*  @brief :　文字列 -> 数値に変換する
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
	*  @brief :  文字列を比較します
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
		-       必要があれば文字数カウント
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
	*  @brief :  文字列の長さを取得します.
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
	*  @brief :  文字列のコピーを行います
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
	*  @brief :  文字列に一致する最初のインデックスを返します.
	/*----------------------------------------------------------------------*/
	template<class Char>
	uint64 StringUtility::Find(
		const Char* left, uint64 leftLength,
		const Char* right, uint64 rightLength,
		uint64 startIndex, const bool useCaseSensitivity)  noexcept
	{
		/*-------------------------------------------------------------------
		-        文字列が無ければ無効値を取る
		---------------------------------------------------------------------*/
		if (left == nullptr || right == nullptr) { return NPOS; }
		if (*left == 0)                          { return NPOS; }

		/*-------------------------------------------------------------------
		-       インデックスが有効か
		---------------------------------------------------------------------*/
		leftLength  = (leftLength  == NPOS) ? Length(left)  : leftLength;
		rightLength = (rightLength == NPOS) ? Length(right) : rightLength;
		
		if (leftLength  <= startIndex) { return NPOS; }
		if (rightLength <= 0)          { return NPOS; }

		/*-------------------------------------------------------------------
		-       文字列の検索
		---------------------------------------------------------------------*/
		auto pos = left + startIndex;
		const auto end = left + leftLength;

		// 大文字, 小文字を区別する場合
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
	*  @brief :  文字列に一致する最初のインデックスを返します.
	/*----------------------------------------------------------------------*/
	template<class Char>
	uint64 StringUtility::ReverseFind(
		const Char* left, uint64 leftLength,
		const Char* right, uint64 rightLength,
		uint64 startIndex, uint64 sortCount, const bool useCaseSensitivity) noexcept 
	{
		const Char nullString[] = { '\0' };

		/*-------------------------------------------------------------------
		-       事前準備
		---------------------------------------------------------------------*/
		left  = (left  == nullptr) ? nullString : left;
		right = (right == nullptr) ? nullString : right;
		
		leftLength  = static_cast<uint64>((leftLength  == NPOS) ? Length(left) : leftLength);
		rightLength = static_cast<uint64>((rightLength == NPOS) ? Length(right) : rightLength);

		startIndex = (startIndex == NPOS) ? (leftLength - 1) : startIndex;

		/*-------------------------------------------------------------------
		-       文字列が無ければ無効置を得る
		---------------------------------------------------------------------*/
		if (leftLength == 0 && (startIndex == NPOS || startIndex == 0))
		{
			return (rightLength == 0) ? 0 : NPOS;
		}

		/*-------------------------------------------------------------------
		-       インデックスが有効か
		---------------------------------------------------------------------*/
		if (startIndex < 0)          { return StringUtility::NPOS; }
		if (startIndex >= leftLength) { return StringUtility::NPOS; }

		/*-------------------------------------------------------------------
		-      検索範囲が正常かどうか
		---------------------------------------------------------------------*/
		// 検索文字数が0の場合は必ず検索開始位置でヒットする
		if (rightLength == 0 && sortCount != NPOS && startIndex - sortCount + 1 >= 0)
		{
			return startIndex;
		}

		      auto pos = left + startIndex;                                   // 検索範囲の末尾の文字
		const auto end = (sortCount == NPOS) ? left : pos - (sortCount - 1); // 検索範囲の先頭の文字
		if (end > pos) { return StringUtility::NPOS; }                       // 末尾と先頭が逆転していない

		// 検索範囲が検索文字数よりも少ない場合は見つかるはずがない
		if (pos - end < (int64)(rightLength - 1)) { return StringUtility::NPOS; }

		pos -= (rightLength - 1);

		/*-------------------------------------------------------------------
		-       文字列の検索
		---------------------------------------------------------------------*/
		// 大文字, 小文字を区別する場合
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
	*  @brief :  文字列の部分文字列を抽出します
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
	*  @brief : 文字列の先頭から指定した文字数を抽出します.
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
	*  @brief : 文字列の末尾から指定した文字数を抽出します.
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
	*  @brief :  文字列の末尾が指定した文字列と一致するかを返します
	/*----------------------------------------------------------------------*/
	template<typename Char>
	bool StringUtility::IsLastMatch(const Char* left, uint64 leftLength, const Char* right, uint64 rightLength, const bool useCaseSensitivity)
	{
		/*-------------------------------------------------------------------
		-       長さがNPOSの場合は\0までカウントする
		---------------------------------------------------------------------*/
		leftLength  = leftLength  == NPOS ? Length(left)  : leftLength;
		rightLength = rightLength == NPOS ? Length(right) : rightLength;

		if (rightLength <= 0)         { return true; }
		if (leftLength < rightLength) { return false; }

		/*-------------------------------------------------------------------
		-       一致しているかの判別
		---------------------------------------------------------------------*/
		const Char* positionFirst  = left  + leftLength  - 1;
		const Char* positionSecond = right + rightLength - 1;

		/*-------------------------------------------------------------------
		-       大文字・小文字を区別する
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
		-       大文字・小文字を区別しない
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
	*  @brief :  文字を大文字に切り替えます
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
	*  @brief :  文字を小文字に切り替えます
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
		-       OutNumberの初期化
		---------------------------------------------------------------------*/
		if (outNumber != nullptr) 
		{
			*outNumber = 0; 
		}

		/*-------------------------------------------------------------------
		-       基数チェック
		---------------------------------------------------------------------*/
		if (!(radix == 0 || radix == 2 || radix == 8 || radix == 10 || radix == 16))
		{
			return NumberConversionResult::ArgumentsError;
		}

		/*-------------------------------------------------------------------
		-       空白をスキップする
		---------------------------------------------------------------------*/
		const Char* pos = string;
		const Char* end = string + (stringLength == NPOS ? Length(string) : stringLength);

		while (IsSpace(*pos)) { ++pos; }

		/*-------------------------------------------------------------------
		-       符号の正負を確認する
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

		// 符号しかなかった場合
		if (pos >= end)
		{
			return NumberConversionResult::FormatError;
		}

		/*-------------------------------------------------------------------
		-       基数が0の場合は自動判別を行う
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
		-       基数によってプレフィックスをスキップする
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
		-       基数によってプレフィックスをスキップする
		---------------------------------------------------------------------*/
		UnsignedType value = 0;
		UnsignedType overflowMax = UnsignedMax / (UnsignedType)radix; // 乗算しようとするとき, この値以上であれば次の乗算でオーバーフローする
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

			// 基数より大きい桁が見つかった (10進数なのに)
			if (digit >= (UnsignedType)radix)
			{
				return NumberConversionResult::FormatError;
			}

			++count;

			// 計算する前にオーバーフローを確認する
			if (value < overflowMax ||
				(value == overflowMax && digit <= radixMax))
			{
				value = value * (UnsignedType)radix + digit;
			}
			else
			{
				isOverflow = true;
			}

			// 次の文字へ
			++pos;
			if (pos >= end) { break; }
		}

		if (outEndPointer != nullptr)
		{
			*outEndPointer = pos;
		}

		/*-------------------------------------------------------------------
		-       オーバーフローしていたら最大値に丸めておく
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
		-       マイナス符号である場合は, Singed値をunsignedとして返し, 呼び出し側でsignedでキャストして返す
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