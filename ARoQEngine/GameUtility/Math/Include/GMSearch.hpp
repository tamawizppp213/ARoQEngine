//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSort.hpp
///             @brief  GMSortAlgorithm
///             @author Toide Yutaro
///             @date   2020_12_13
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SEARCH_HPP
#define GM_SEARCH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#define NOT_FOUND -1

//////////////////////////////////////////////////////////////////////////////////
//                         
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	/****************************************************************************
	*				  			Search
	*************************************************************************//**
	*  @class     Search
	*  @brief     Search
	*****************************************************************************/
	template<typename T>
	class Search
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// for array
		static int LinearSearch(const T* array, int arraySize, T key);
		static int BinarySearch(const T* array, int arraySize, T key);

		// for std::vector
		static int LinearSearch(const std::vector<T>& vector, T key);
		static int BinarySearch(const std::vector<T>& vector, T key);
		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/


	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/

	};

#pragma region Public Function
	/****************************************************************************
	*							LinearSearch
	*************************************************************************//**
	*  @fn        int Search<T>::LinearSearch(T* array, int arraySize, T key)
	*  @brief     Linear Search for array (O(N))
	*  @param[in] T*  array
	*  @param[in] int arraySize
	*  @param[in] T   key 
	*  @return 　　int index (NOT FOUND: -1)
	*****************************************************************************/
	template<typename T> int Search<T>::LinearSearch(const T* array, int arraySize, T key)
	{
		for (int index = 0; index < arraySize; ++index)
		{
			if (array[index] == key)
			{
				return index;
			}
		}
		return NOT_FOUND;
	}

	/****************************************************************************
	*							LinearSearch
	*************************************************************************//**
	*  @fn        int Search<T>::LinearSearch(const std::vector<T>& vector, T key)
	*  @brief     Linear Search for vector (O(N))
	*  @param[in] std::vector<T>& vector
	*  @param[in] T   key
	*  @return 　　int index (NOT FOUND: -1)
	*****************************************************************************/
	template<typename T> int Search<T>::LinearSearch(const std::vector<T>& vector, T key)
	{
		for (int index = 0; index < vector.size(); ++index)
		{
			if (vector[index] == key)
			{
				return index;
			}
		}
		return NOT_FOUND;
	}

	/****************************************************************************
	*							BinarySearch
	*************************************************************************//**
	*  @fn        int Search<T>::BinarySearch(T* array, int arraySize, T key)
	*  @brief     Linear Search for array (O(log_2(N))) This algorithm is needed well-ordered ascend data
	*  @param[in] T*  array
	*  @param[in] int arraySize
	*  @param[in] T   key
	*  @return 　　int index (NOT FOUND: -1)
	*****************************************************************************/
	template<typename T> int Search<T>::BinarySearch(const T* array, int arraySize, T key)
	{
		int left  = 0;
		int right = arraySize;
		int mid;

		while (left < right)
		{
			mid = (left + right) / 2;
			if (key == array[mid])
			{
				return mid;
			}

			// Explore the second half
			if (key > array[mid])
			{
				left = mid + 1;
			}
			// Explore the first half
			else if (key < array[mid])
			{
				right = mid;
			}

		}
		return NOT_FOUND;
	}

	/****************************************************************************
	*							BinarySearch
	*************************************************************************//**
	*  @fn        int Search<T>::BinarySearch(const std::vector<T>& vector, T key)
	*  @brief     Binary Search for vector (O(log_2(N))) This algorithm is needed well-ordered ascend data
	*  @param[in] std::vector<T>& vector
	*  @param[in] T   key
	*  @return 　　int index (NOT FOUND: -1)
	*****************************************************************************/
	template<typename T> int Search<T>::BinarySearch(const std::vector<T>& vector, T key)
	{
		int left  = 0;
		int right = static_cast<int>(vector.size());
		int mid;

		while (left < right)
		{
			mid = (left + right) / 2;
			if (key == vector[mid])
			{
				return mid;
			}

			// Explore the second half
			if (key > vector[mid])
			{
				left = mid + 1;
			}
			// Explore the first half
			else if (key < vector[mid])
			{
				right = mid;
			}

		}
		return NOT_FOUND;
	}
#pragma endregion Public Function
#pragma region Private Function

#pragma endregion Private Function
}

#endif