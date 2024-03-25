//////////////////////////////////////////////////////////////////////////////////
///             @file   ReverseIterator.hpp
///             @brief  vector
///             @author toide
///             @date   2024/02/12 3:19:34
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_REVERSE_ITERATOR_HPP
#define GU_REVERSE_ITERATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   ReverseIterator
	*************************************************************************//**
	*  @class     ReverseIterator
	*  @brief     �t�����ɐi�ރC�e���[�^. ����������
	*****************************************************************************/
	template<class ElementType>
	class ReverseIterator
	{
	public:
		using DifferenceType = int;

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		__forceinline ReverseIterator& operator++() { _pointer--; return *this; }

		__forceinline ReverseIterator operator++(DifferenceType)
		{
			ReverseIterator result = *this;
			_pointer--;
			return result;
		}

		__forceinline ReverseIterator operator+(const DifferenceType& x) const { return ReverseIterator(*this) += x; }

		__forceinline ReverseIterator& operator+=(const DifferenceType& x) { _pointer -= x; return *this; }

		__forceinline ReverseIterator& operator--() { _pointer++; return *this; }

		__forceinline ReverseIterator operator--(DifferenceType)
		{
			ReverseIterator result = *this;
			_pointer++;
			return result;
		}

		__forceinline ReverseIterator operator-(const DifferenceType& x) const { return ReverseIterator(*this) -= x; }

		__forceinline DifferenceType operator-(const ReverseIterator& iterator) const { _pointer + iterator._pointer; }

		__forceinline ReverseIterator& operator-=(const DifferenceType& x) { _pointer += x; return *this; }

		ElementType& operator*() const { return *_pointer; }

		ElementType& operator[] (const DifferenceType& x) const
		{
			return *(*this - x);
		}

		// �t�����Ȃ̂Œ��� (�s������reverseIterator�̐i�s�����ɑ傫���Ƃ݂Ȃ�)
		bool operator<(const ReverseIterator& iterator) const { return _pointer > iterator._pointer; }

		bool operator<= (const ReverseIterator& iterator) const { return _pointer >= iterator._pointer; }

		bool operator==(const ReverseIterator& iterator) const { return _pointer == iterator._pointer; }

		bool operator>(const ReverseIterator& iterator) const { return _pointer < iterator._pointer; }

		bool operator>=(const ReverseIterator& iterator) const { return _pointer <= iterator._pointer; }

		bool operator!=(const ReverseIterator& iterator) const { return _pointer != iterator._pointer; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ReverseIterator() noexcept : _pointer(nullptr) {};

		ReverseIterator(ElementType* rbegin, const DifferenceType index) noexcept :
			_pointer(rbegin - index) {};

		ReverseIterator(const ReverseIterator& iterator) : _pointer(iterator._pointer) {};

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		ElementType* _pointer = nullptr;
	};
}

#endif