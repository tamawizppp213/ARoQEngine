//////////////////////////////////////////////////////////////////////////////////
///             @file   ReverseConstIterator.hpp
///             @brief  vector
///             @author toide
///             @date   2024/02/12 3:19:34
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_REVERSE_CONST_ITERATOR_HPP
#define GU_REVERSE_CONST_ITERATOR_HPP

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
	*				  			   ReverseConstIterator
	****************************************************************************/
	/* @class     ReverseConstIterator
	*  @brief     逆方向に進むイテレータ. 書き換え可
	*****************************************************************************/
	template<class ElementType>
	class ReverseConstIterator
	{
	public:
		using DifferenceType = int;

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		__forceinline ReverseConstIterator& operator++() { _pointer--; return *this; }

		__forceinline ReverseConstIterator operator++(DifferenceType)
		{
			ReverseConstIterator result = *this;
			_pointer--;
			return result;
		}

		__forceinline ReverseConstIterator operator+(const DifferenceType& x) const { return ReverseConstIterator(*this) += x; }

		__forceinline ReverseConstIterator& operator+=(const DifferenceType& x) { _pointer -= x; return *this; }

		__forceinline ReverseConstIterator& operator--() { _pointer++; return *this; }

		__forceinline ReverseConstIterator operator--(DifferenceType)
		{
			ReverseConstIterator result = *this;
			_pointer++;
			return result;
		}

		__forceinline ReverseConstIterator operator-(const DifferenceType& x) const { return ReverseConstIterator(*this) -= x; }

		__forceinline DifferenceType operator-(const ReverseConstIterator& iterator) const { _pointer + iterator._pointer; }

		__forceinline ReverseConstIterator& operator-=(const DifferenceType& x) { _pointer += x; return *this; }

		ElementType& operator*() const { return *_pointer; }

		ElementType& operator[] (const DifferenceType& x) const
		{
			return *(*this - x);
		}

		// 逆向きなので注意 (不等号はreverseIteratorの進行方向に大きいとみなす)
		bool operator<(const ReverseConstIterator& iterator) const { return _pointer > iterator._pointer; }

		bool operator<= (const ReverseConstIterator& iterator) const { return _pointer >= iterator._pointer; }

		bool operator==(const ReverseConstIterator& iterator) const { return _pointer == iterator._pointer; }

		bool operator>(const ReverseConstIterator& iterator) const { return _pointer < iterator._pointer; }

		bool operator>=(const ReverseConstIterator& iterator) const { return _pointer <= iterator._pointer; }

		bool operator!=(const ReverseConstIterator& iterator) const { return _pointer != iterator._pointer; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ReverseConstIterator() noexcept : _pointer(nullptr) {};

		ReverseConstIterator(const ElementType* rbegin, const DifferenceType index) noexcept :
			_pointer(rbegin - index) {};

		ReverseConstIterator(const ReverseConstIterator& iterator) : _pointer(iterator._pointer) {};

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		const ElementType* _pointer = nullptr;
	};
}

#endif