//////////////////////////////////////////////////////////////////////////////////
///  @file   GUhashMapIterator.hpp
///  @brief  temp
///  @author toide
///  @date   2024/05/04 11:57:03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_HASH_MAP_ITERATOR_HPP
#define GU_HASH_MAP_ITERATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::details::hashmap
{ 
	/****************************************************************************
	*				  			   GUhashMapIterator
	*************************************************************************//**
	/*  @class     GUhashMapIterator
	*   @brief     temp
	*****************************************************************************/
	template<typename Key, typename Value>
	struct TemplatedIterator
	{
		using Element = HashMapElement<Key, Value>;

	public:
		#pragma region Public Function
		#pragma endregion 

		#pragma region Public Property

		/*! @brief 現在のイテレータ*/
		Element* Current = nullptr;
		
		#pragma endregion 

		#pragma region Public Operator 
		/*! @brief Iteratorが等しいか(同じポインタを指し示しているか)どうか*/
		friend bool operator==(const TemplatedIterator& lhs, const TemplatedIterator& rhs)
		{
			return lhs.Current == rhs.Current;
		}

		/*! @brief Iteratorが異なるか(違うポインタを指し示しているか)どうか*/
		friend bool operator!=(const TemplatedIterator& lhs, const TemplatedIterator& rhs)
		{
			return !(lhs == rhs);
		}

		TemplatedIterator& operator++()
		{
			do
			{
				++Current;
			} while (Current->IsEmpty());
			return *this;
		}

		TemplatedIterator operator++(int)
		{
			TemplatedIterator copy(*this);
			++*this;
			return copy;
		}

		gu::Pair<Key, Value>& operator*() const
		{
			return Current->Data;
		}
		gu::Pair<Key, Value>* operator->() const
		{
			return &Current->Data;
		}

		operator TemplatedIterator<const Key, const Value>() const
		{
			return { Current->Data };
		}
		#pragma endregion 

		#pragma region Public Constructor and Destructor
		TemplatedIterator() = default;

		explicit TemplatedIterator(Element* element) : Current(element) {};
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif