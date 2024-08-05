//////////////////////////////////////////////////////////////////////////////////
///             @file   ConstIterator.hpp
///             @brief  vector
///             @author toide
///             @date   2024/02/12 3:19:34
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_CONST_ITERATOR_HPP
#define GU_CONST_ITERATOR_HPP

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
	*				  			   ConstIterator
	****************************************************************************/
	/* @class     ConstIterator
	*  @brief     値の書き換え不可のイテレータ
	*****************************************************************************/
	template<class ElementType>
	class ConstIterator
	{
	public:
		using DifferenceType = int;

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		__forceinline ConstIterator& operator++() { _pointer++; return *this; }

		__forceinline ConstIterator operator++(DifferenceType)
		{
			ConstIterator result = *this;
			_pointer++;
			return result;
		}

		__forceinline ConstIterator operator+(const DifferenceType& x) const { return ConstIterator(*this) += x; }

		__forceinline ConstIterator& operator+=(const DifferenceType& x) { _pointer += x; return *this; }

		__forceinline ConstIterator& operator--() { _pointer--; return *this; }

		__forceinline ConstIterator operator--(DifferenceType)
		{
			ConstIterator result = *this;
			_pointer--;
			return result;
		}

		__forceinline ConstIterator operator-(const DifferenceType& x) const { return ConstIterator(*this) -= x; }

		__forceinline DifferenceType operator-(const ConstIterator& iterator) const { _pointer - iterator._pointer; }

		__forceinline ConstIterator& operator-=(const DifferenceType& x) { _pointer -= x; return *this; }

		const ElementType& operator*() const { return *_pointer; }

		const ElementType& operator[] (const DifferenceType& x) const
		{
			return *(*this + x);
		}

		bool operator<(const ConstIterator& iterator) const { return _pointer < iterator._pointer; }

		bool operator<= (const ConstIterator& iterator) const { return _pointer <= iterator._pointer; }

		bool operator==(const ConstIterator& iterator) const { return _pointer == iterator._pointer; }

		bool operator>(const ConstIterator& iterator) const { return _pointer > iterator._pointer; }

		bool operator>=(const ConstIterator& iterator) const { return _pointer >= iterator._pointer; }

		bool operator!=(const ConstIterator& iterator) const { return _pointer != iterator._pointer; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ConstIterator() noexcept : _pointer(nullptr) {};

		ConstIterator(const ElementType* begin, const DifferenceType index) noexcept :
			_pointer(begin + index) {};

		ConstIterator(const ConstIterator& iterator) : _pointer(iterator._pointer) {};

		ConstIterator(const ElementType* begin) noexcept : _pointer(begin) {};
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