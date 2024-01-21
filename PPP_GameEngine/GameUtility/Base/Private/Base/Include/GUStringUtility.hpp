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
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::details
{
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
		static uint64 FindFirstIndexOf(
			const Char* left, uint64 leftLength,
			const Char* right, uint64 rightLength,
			uint64 startIndex, const bool useCaseSensitivity) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  文字列に一致する最初のインデックスを返します.
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 FindLastIndexOf(
			const Char* left, uint64 leftLength,
			const Char* right, uint64 rightLength,
			uint64 startIndex, uint64 sortCount, const bool useCaseSensitivity) noexcept;

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
		auto minCount = min(leftLength, rightLength);
		auto maxCount = max(leftLength, rightLength);
		
		if (count == NPOS)
		{
			count = maxCount;
		}
		else
		{
			maxCount = count;
			count    = min(minCount, count);
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
				if (ToUpper(*left) != ToUpper(right))
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
	uint64 StringUtility::FindFirstIndexOf(
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
	uint64 StringUtility::FindLastIndexOf(
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
		if (pos - end < (rightLength - 1)) { return StringUtility::NPOS; }

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
#pragma endregion Implement
}

#endif