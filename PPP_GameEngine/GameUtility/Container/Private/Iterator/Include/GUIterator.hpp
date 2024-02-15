//////////////////////////////////////////////////////////////////////////////////
///             @file   Iterator.hpp
///             @brief  vector
///             @author toide
///             @date   2024/02/12 3:19:34
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_ITERATOR_HPP
#define GU_ITERATOR_HPP

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
	*				  			   Iterator
	*************************************************************************//**
	*  @class     Iterator
	*  @brief     temp
	*****************************************************************************/
	template<class ElementType>
	class Iterator
	{
	public:
		using DifferenceType = int;

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		__forceinline Iterator& operator++() { _pointer++; return *this; }

		__forceinline Iterator operator++(DifferenceType)
		{
			Iterator result = *this;
			_pointer++;
			return result;
		}

		__forceinline Iterator operator+(const DifferenceType& x) const { return Iterator(*this) += x; }

		__forceinline Iterator& operator+=(const DifferenceType& x) { _pointer += x; return *this; }

		__forceinline Iterator& operator--() { _pointer--; return *this; }

		__forceinline Iterator operator--(DifferenceType)
		{
			Iterator result = *this;
			_pointer--;
			return result;
		}

		__forceinline Iterator operator-(const DifferenceType& x) const { return Iterator(*this) -= x; }

		__forceinline DifferenceType operator-(const Iterator& iterator) const { _pointer - iterator._pointer; }

		__forceinline Iterator& operator-=(const DifferenceType& x) { _pointer -= x; return *this; }

		ElementType& operator*() const { return *_pointer; }

		ElementType& operator[] (const DifferenceType& x) const
		{
			return *(*this + x);
		}

		bool operator<(const Iterator& iterator) const { return _pointer < iterator._pointer; }

		bool operator<= (const Iterator& iterator) const { return _pointer <= iterator._pointer; }

		bool operator==(const Iterator& iterator) const { return _pointer == iterator._pointer; }

		bool operator>(const Iterator& iterator) const { return _pointer > iterator._pointer; }

		bool operator>=(const Iterator& iterator) const { return _pointer >= iterator._pointer; }

		bool operator!=(const Iterator& iterator) const { return _pointer != iterator._pointer; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Iterator() noexcept : _pointer(nullptr) {};

		Iterator(ElementType* begin, const DifferenceType index) noexcept : 
			_pointer(begin + index) {};

		Iterator(const Iterator& iterator) : _pointer(iterator._pointer) {};

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