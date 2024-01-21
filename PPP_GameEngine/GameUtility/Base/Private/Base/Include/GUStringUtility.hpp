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
		static uint64 FindFirstIndexOf(
			const Char* left, uint64 leftLength,
			const Char* right, uint64 rightLength,
			uint64 startIndex, const bool useCaseSensitivity) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  ������Ɉ�v����ŏ��̃C���f�b�N�X��Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		template<typename Char>
		static uint64 FindLastIndexOf(
			const Char* left, uint64 leftLength,
			const Char* right, uint64 rightLength,
			uint64 startIndex, uint64 sortCount, const bool useCaseSensitivity) noexcept;

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
	uint64 StringUtility::FindFirstIndexOf(
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
	uint64 StringUtility::FindLastIndexOf(
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
		if (pos - end < (rightLength - 1)) { return StringUtility::NPOS; }

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
#pragma endregion Implement
}

#endif