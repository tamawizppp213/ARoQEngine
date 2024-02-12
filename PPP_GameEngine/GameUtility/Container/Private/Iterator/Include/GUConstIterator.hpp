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
	*************************************************************************//**
	*  @class     ConstIterator
	*  @brief     �l�̏��������s�̃C�e���[�^
	*****************************************************************************/
	template<class ElementType>
	class ConstIterator
	{
	public:
		using DifferenceType = int64;

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
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

		ElementType& operator*() const { return *_pointer; }

		ElementType& operator[] (const DifferenceType& x) const
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

		ConstIterator(ElementType* begin, const DifferenceType index) noexcept :
			_pointer(begin + index) {};

		ConstIterator(const ConstIterator& iterator) : _pointer(iterator._pointer) {};

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		const ElementType* _pointer = nullptr;
	};
}

#endif