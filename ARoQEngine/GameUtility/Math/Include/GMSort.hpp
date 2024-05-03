//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSort.hpp
///             @brief  GMSortAlgorithm
///             @author Toide Yutaro
///             @date   2020_12_17
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SORT_HPP
#define GM_SORT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <utility>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#define QUICK_THREASHOLD 64

//////////////////////////////////////////////////////////////////////////////////
//                         
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	/****************************************************************************
	*				  			Sort
	*************************************************************************//**
	*  @class     Sort
	*  @brief     Sort
	*****************************************************************************/
	template<typename T>
	class Sort
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// for array
		static void InsertionSortAscend (T* array, int arraySize);
		static void InsertionSortDescend(T* array, int arraySize);
		static void BubbleSortAscend    (T* array, int arraySize);
		static void BubbleSortDescend   (T* array, int arraySize);
		static void SelectionSortAscend (T* array, int arraySize);
		static void SelectionSortDescend(T* array, int arraySize);
		static void ShellSortAscend     (T* array, int arraySize);
		static void ShellSortDescend    (T* array, int arraySize);
		static void MergeSortAscend     (T* array, int left, int right);
		static void MergeSortDescend    (T* array, int left, int right);
		static void QuickSortAscend     (T* array, int left, int right);
		static void QuickSortDescend    (T* array, int left, int right);

		// for std::vector
		static void InsertionSortAscend (std::vector<T>& vector);
		static void InsertionSortDescend(std::vector<T>& vector);
		static void BubbleSortAscend    (std::vector<T>& vector);
		static void BubbleSortDescend   (std::vector<T>& vector);
		static void SelectionSortAscend (std::vector<T>& vector);
		static void SelectionSortDescend(std::vector<T>& vector);
		static void ShellSortAscend     (std::vector<T>& vector);
		static void ShellSortDescend    (std::vector<T>& vector);
		static void MergeSortAscend     (std::vector<T>& vector, int left, int right);
		static void MergeSortDescend    (std::vector<T>& vector, int left, int right);
		static void QuickSortAscend     (std::vector<T>& vector, int left, int right);
		static void QuickSortDescend    (std::vector<T>& vector, int left, int right);
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
		static void MergeAscend (T* array, int left, int mid, int right);
		static void MergeDescend(T* array, int left, int mid, int right);
		static void MergeAscend (std::vector<T>& vector, int left, int mid, int right);
		static void MergeDescend(std::vector<T>& vector, int left, int mid, int right);
		static T Median(T& a, T& b, T& c);
		

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		
	};

#pragma region Public Function
	/****************************************************************************
	*							InsertionSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::InsertionSortAscend(T* array, int arraySize)
	*  @brief     Insertion Sort (O(N ~ N^2))
	*  @param[out]T*  array
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::InsertionSortAscend(T* array, int arraySize)
	{
		int j;
		
		for (int i = 1; i < arraySize; ++i)
		{
			T temp = array[i];
			for (j = i - 1; j >= 0 && array[j] >  temp; j--)
			{
				array[j + 1] = array[j];
			}

			array[j + 1] = temp;
		}
	}

	/****************************************************************************
	*							InsertionSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::InsertionSortDescend(T* array, int arraySize)
	*  @brief     Insertion Sort (O(N ~ N^2))
	*  @param[out]T* array
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::InsertionSortDescend(T* array, int arraySize)
	{
		int j;

		for (int i = 1; i < arraySize; ++i)
		{
			T temp = array[i];
			for (j = i - 1; j >= 0 && array[j] < temp; j--)
			{
				array[j + 1] = array[j];
			}

			array[j + 1] = temp;
		}
	}
	/****************************************************************************
	*							BubbleSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::BubbleSortDescend(T* array, int arraySize)
	*  @brief     Bubble Sort (O(N^2))
	*  @param[out]T* array
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::BubbleSortAscend(T* array, int arraySize)
	{
		bool isSwap = true;
		for (int i = 0; isSwap; ++i)
		{
			isSwap = false;

			for (int j = arraySize - 1; j >= i + 1; --j)
			{
				if (array[j] < array[j - 1])
				{
					std::swap(array[j], array[j - 1]);
					isSwap = true;
				}
			}
		}
	}
	/****************************************************************************
	*							BubbleSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::InsertionSortDescend(T* array, int arraySize)
	*  @brief     Insertion Sort (O(N^2))
	*  @param[out]T* array
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::BubbleSortDescend(T* array, int arraySize)
	{
		bool isSwap = true;
		for (int i = 0; isSwap; ++i)
		{
			isSwap = false;

			for (int j = arraySize - 1; j >= i + 1; --j)
			{
				if (array[j] > array[j - 1])
				{
					std::swap(array[j], array[j - 1]);
					isSwap = true;
				}
			}
		}
	}
	/****************************************************************************
	*							SelectionSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::SelectionSortAscend(T* array, int arraySize)
	*  @brief     Selection Sort (O(N^2)) !note: This sort is instable and has a bug. 
	*  @param[out]T* array
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::SelectionSortAscend(T* array, int arraySize)
	{
		int i, j, temp, min_jIndex;
		for (i = 0; i < arraySize - 1; i++)
		{
			min_jIndex = i;
			for (j = i + 1; j < arraySize; j++)
			{
				// get min_jindex
				if (array[j] < array[min_jIndex])
				{
					min_jIndex = j;
				}
				
				// swap
				temp              = array[i];
				array[i]          = array[min_jIndex];
				array[min_jIndex] = temp;
			}
		}
	}
	/****************************************************************************
	*							SelectionSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::SelectionSortDescend(T* array, int arraySize)
	*  @brief     Selection Sort (O(N^2))!note: This sort is instable and has a bug.
	*  @param[out]T* array
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::SelectionSortDescend(T* array, int arraySize)
	{
		int i, j, temp, max_jIndex;
		for (i = 0; i < arraySize - 1; i++)
		{
			max_jIndex = i;
			for (j = i + 1; j < arraySize; j++)
			{
				// get min_jindex
				if (array[j] > array[max_jIndex])
				{
					max_jIndex = j;
				}

				// swap
				temp              = array[i];
				array[i]          = array[max_jIndex];
				array[max_jIndex] = temp;
			}
		}
	}

	/****************************************************************************
	*							ShellSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::ShellSortAscend(T* array, int arraySize)
	*  @brief     Shell Sort (O(N^1.25))(step: g_{n+1} = 3g_n +1).
	*  @param[out]T* array
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::ShellSortAscend(T* array, int arraySize)
	{
		int j, temp, step = 1;

		while (3 * step + 1 <= arraySize / 9)
		{
			step = 3 * step + 1;
		}

		while (step >= 1)
		{
			for (int i = step; i < arraySize; ++i)
			{
				j = i;
				while ((j >= step) && (array[j - step] > array[j]))
				{
					temp            = array[j - step];
					array[j - step] = array[j];
					array[j]        = temp;

					j -= step;
				}
			}
			step /= 3;
		}
	}
	/****************************************************************************
	*							ShellSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::ShellSortDescend(T* array, int arraySize)
	*  @brief     Shell Sort (O(N^1.25))(step: g_{n+1} = 3g_n +1).
	*  @param[out]T* array
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::ShellSortDescend(T* array, int arraySize)
	{
		int j, temp, step = 1;

		while (3 * step + 1 <= arraySize / 9)
		{
			step = 3 * step + 1;
		}

		while (step >= 1)
		{
			for (int i = step; i < arraySize; ++i)
			{
				j = i;
				while ((j >= step) && (array[j - step] < array[j]))
				{
					temp            = array[j - step];
					array[j - step] = array[j];
					array[j]        = temp;

					j -= step;
				}
			}
			step /= 3;
		}
	}

	/****************************************************************************
	*							MergeSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::MergeSortAscend(T* array, int left, int right)
	*  @brief     Merge Sort (O(nlogn))(stable).
	*  @param[out]T* array
	*  @param[in] int left index
	*  @param[in] int right index
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::MergeSortAscend(T* array, int left, int right)
	{
		if (left < right)
		{
			int mid = left + (right - left) / 2;

			gm::Sort<T>::MergeSortAscend(array, left, mid);
			gm::Sort<T>::MergeSortAscend(array, mid + 1, right);
			gm::Sort<T>::MergeAscend(array, left, mid, right);
		}
	}

	/****************************************************************************
	*							MergeSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::MergeSortDescend(T* array, int left, int right)
	*  @brief     Merge Sort (O(nlogn))(stable).
	*  @param[out]T* array
	*  @param[in] int left Index
	*  @param[in] int right Index
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::MergeSortDescend(T* array, int left, int right)
	{
		if (left < right)
		{
			int mid = left + (right - left) / 2;

			gm::Sort<T>::MergeSortDescend(array, left, mid);
			gm::Sort<T>::MergeSortDescend(array, mid + 1, right);
			gm::Sort<T>::MergeDescend(array, left, mid, right);
		}
	}

	/****************************************************************************
	*							QuickSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::QuickSortAscend(T* array, int left, int right)
	*  @brief     Quick Sort (If the number of index is small (<= 64), switch to insertion sort for performance)
	*  @param[out]T*  array
	*  @param[in] int left index
	*  [param[in] int right index
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::QuickSortAscend(T* array, int left, int right)
	{
		
		if (right - left <= QUICK_THREASHOLD)
		{
			InsertionSortAscend(array, right - left + 1);
			return;
		}
		
		// Define pivot
		T pivot = Median(array[left], array[(left + right) / 2], array[right]);

		// Division
		int l = left;
		int r = right;

		while (l <= r)
		{
			while (l < right && (array[l] - pivot) < 0 )l++;
			while (r > left  && (array[r] - pivot) >= 0)r--;
			if (l > r)break;
			std::swap(array[l], array[r]);
			l++; r--;
		}

		QuickSortAscend(array, left, l - 1);
		QuickSortAscend(array, l, right);
	}

	/****************************************************************************
	*							QuickSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::QuickSortDescend(T* array, int left, int right)
	*  @brief     Quick Sort (If the number of index is small (<= 64), switch to insertion sort for performance)
	*  @param[out]T*  array
	*  @param[in] int left index
	*  [param[in] int right index
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::QuickSortDescend(T* array, int left, int right)
	{

		if (right - left <= QUICK_THREASHOLD)
		{
			InsertionSortDescend(array, right - left + 1);
			return;
		}

		// Define pivot
		T pivot = Median(array[left], array[(left + right) / 2], array[right]);

		// Division
		int l = left;
		int r = right;

		while (l <= r)
		{
			while (l < right && (array[l] - pivot) > 0)l++;
			while (r > left  && (array[r] - pivot) <= 0)r--;
			if (l > r)break;
			std::swap(array[l], array[r]);
			l++; r--;
		}

		QuickSortDescend(array, left, l - 1);
		QuickSortDescend(array, l, right);
	}


	/****************************************************************************
	*							InsertionSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::InsertionSortAscend(std::vector<T>& vector)
	*  @brief     Insertion Sort (O(N ~ N^2))
	*  @param[out]std::vector<T>& vector
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::InsertionSortAscend(std::vector<T>& vector)
	{
		int j;

		for (int i = 1; i < vector.size(); ++i)
		{
			T temp = vector[i];
			for (j = i - 1; j >= 0 && vector[j] > temp; j--)
			{
				vector[j + 1] = vector[j];
			}

			vector[j + 1] = temp;
		}
	}

	/****************************************************************************
	*							InsertionSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::InsertionSortDescend(std::vector<T>& vector)
	*  @brief     Insertion Sort (O(N ~ N^2))
	*  @param[out]std::vector<T>& vector
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::InsertionSortDescend(std::vector<T>& vector)
	{
		int j;

		for (int i = 1; i < vector.size(); ++i)
		{
			T temp = vector[i];
			for (j = i - 1; j >= 0 && vector[j] < temp; j--)
			{
				vector[j + 1] = vector[j];
			}

			vector[j + 1] = temp;
		}
	}
	/****************************************************************************
	*							BubbleSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::BubbleSortDescend(std::vector<T>& vector)
	*  @brief     Bubble Sort (O(N^2))
	*  @param[out]T* array
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::BubbleSortAscend(std::vector<T>& vector)
	{
		bool isSwap = true;
		for (int i = 0; isSwap; ++i)
		{
			isSwap = false;

			for (int j = vector.size() - 1; j >= i + 1; --j)
			{
				if (vector[j] < vector[j - 1])
				{
					std::swap(vector[j], vector[j - 1]);
					isSwap = true;
				}
			}
		}
	}
	/****************************************************************************
	*							BubbleSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::InsertionSortDescend(std::vector<T>& vector)
	*  @brief     Insertion Sort (O(N^2))
	*  @param[out]std::vector<T>& vector
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::BubbleSortDescend(std::vector<T>& vector)
	{
		bool isSwap = true;
		for (int i = 0; isSwap; ++i)
		{
			isSwap = false;

			for (int j = vector.size() - 1; j >= i + 1; --j)
			{
				if (vector[j] > vector[j - 1])
				{
					std::swap(vector[j], vector[j - 1]);
					isSwap = true;
				}
			}
		}
	}
	/****************************************************************************
	*							SelectionSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::SelectionSortAscend(std::vector<T>& vector)
	*  @brief     Selection Sort (O(N^2)) !note: This sort is instable and has a bug.
	*  @param[out]std::vector<T>& vector
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::SelectionSortAscend(std::vector<T>& vector)
	{
		int i, j, temp, min_jIndex;
		for (i = 0; i < vector.size() - 1; i++)
		{
			min_jIndex = i;
			for (j = i + 1; j < vector.size(); j++)
			{
				// get min_jindex
				if (vector[j] < vector[min_jIndex])
				{
					min_jIndex = j;
				}

				// swap
				temp = vector[i];
				vector[i] = vector[min_jIndex];
				vector[min_jIndex] = temp;
			}
		}
	}
	/****************************************************************************
	*							SelectionSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::SelectionSortDescendstd::vector<T>& vector)
	*  @brief     Selection Sort (O(N^2))!note: This sort is instable and has a bug.
	*  @param[out]std::vector<T>& vector
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::SelectionSortDescend(std::vector<T>& vector)
	{
		int i, j, temp, max_jIndex;
		for (i = 0; i < vector.size() - 1; i++)
		{
			max_jIndex = i;
			for (j = i + 1; j < vector.size(); j++)
			{
				// get min_jindex
				if (vector[j] > vector[max_jIndex])
				{
					max_jIndex = j;
				}

				// swap
				temp = vector[i];
				vector[i] = vector[max_jIndex];
				vector[max_jIndex] = temp;
			}
		}
	}

	/****************************************************************************
	*							ShellSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::ShellSortAscend(std::vector<T>& vector)
	*  @brief     Shell Sort (O(N^1.25))(step: g_{n+1} = 3g_n +1).
	*  @param[out]std::vector<T>& vector
	*  @param[in] int arraySize
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::ShellSortAscend(std::vector<T>& vector)
	{
		int j, temp, step = 1;

		while (3 * step + 1 <= vector.size() / 9)
		{
			step = 3 * step + 1;
		}

		while (step >= 1)
		{
			for (int i = step; i < vector.size(); ++i)
			{
				j = i;
				while ((j >= step) && (vector[j - step] > vector[j]))
				{
					temp = vector[j - step];
					vector[j - step] = vector[j];
					vector[j] = temp;

					j -= step;
				}
			}
			step /= 3;
		}
	}
	/****************************************************************************
	*							ShellSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::ShellSortDescend(std::vector<T>& vector)
	*  @brief     Shell Sort (O(N^1.25))(step: g_{n+1} = 3g_n +1).
	*  @param[out]std::vector<T>& vector
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::ShellSortDescend(std::vector<T>& vector)
	{
		int j, temp, step = 1;

		while (3 * step + 1 <= vector.size() / 9)
		{
			step = 3 * step + 1;
		}

		while (step >= 1)
		{
			for (int i = step; i < vector.size(); ++i)
			{
				j = i;
				while ((j >= step) && (vector[j - step] < vector[j]))
				{
					temp = vector[j - step];
					vector[j - step] = vector[j];
					vector[j] = temp;

					j -= step;
				}
			}
			step /= 3;
		}
	}

	/****************************************************************************
	*							MergeSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::MergeSortAscend(std::vector<T>& vector)
	*  @brief     Merge Sort (O(nlogn))(stable).
	*  @param[out]std::vector<T>& vector
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::MergeSortAscend(std::vector<T>& vector, int left, int right)
	{
		if (left < right)
		{
			int mid = left + (right - left) / 2;

			gm::Sort<T>::MergeSortAscend(vector, left, mid);
			gm::Sort<T>::MergeSortAscend(vector, mid + 1, right);
			gm::Sort<T>::MergeAscend(vector, left, mid, right);
		}
	}

	/****************************************************************************
	*							MergeSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::MergeSortDescend(std::vector<T>& vector)
	*  @brief     Merge Sort (O(nlogn))(stable).
	*  @param[out]std::vector<T>& vector
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::MergeSortDescend(std::vector<T>& vector, int left, int right)
	{
		if (left < right)
		{
			int mid = left + (right - left) / 2;

			gm::Sort<T>::MergeSortDescend(vector, left, mid);
			gm::Sort<T>::MergeSortDescend(vector, mid + 1, right);
			gm::Sort<T>::MergeDescend  (vector, left, mid, right);
		}
	}
#pragma endregion Public Function
#pragma region Private Function
	/****************************************************************************
	*							  Merge
	*************************************************************************//**
	*  @fn        void Sort<T>::MergeSort(T* array, int arraySize, int left, int mid, int right)
	*  @brief     For Merge Sort 
	*  @param[out]T* array
	*  @param[in] int left
	*  @param[in] int mid,
	*  @param[in] int right
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::MergeAscend(T* array, int left, int mid, int right)
	{
		int leftSize  = mid - left + 1;
		int rightSize = right - mid;

		// create temporary subArray
		T* leftArray  = new T[leftSize];
		T* rightArray = new T[rightSize];

		for (int i = 0; i < leftSize; i++)
		{
			leftArray[i] = array[left + i];
		}
		for (int i = 0; i < rightSize; i++)
		{
			rightArray[i] = array[mid + 1 + i];
		}

		int i = 0, j = 0, k = left;

		while (i < leftSize && j < rightSize)
		{
			if (leftArray[i] <= rightArray[j])
			{
				array[k] = leftArray[i];
				i++;
			}
			else
			{
				array[k] = rightArray[j];
				j++;
			}
			k++;
		}

		while (i < leftSize)
		{
			array[k] = leftArray[i];
			i++;
			k++;
		}

		while (j < rightSize)
		{
			array[k] = rightArray[j];
			j++;
			k++;
		}
		delete[] leftArray;
		delete[] rightArray;
	}

/****************************************************************************
*							  Merge
*************************************************************************//**
*  @fn        void Sort<T>::MergeSort(T* array, int arraySize, int left, int mid, int right)
*  @brief     For Merge Sort
*  @param[out]T* array
*  @param[in] int arraySize
*  @param[in] int left
*  @param[in] int mid,
*  @param[in] int right
*  @return 　　void
*****************************************************************************/
	template<typename T> void Sort<T>::MergeDescend(T* array, int left, int mid, int right)
	{
		int leftSize  = mid - left + 1;
		int rightSize = right - mid;

		// create temporary subArray
		T* leftArray  = new T[leftSize];
		T* rightArray = new T[rightSize];

		for (int i = 0; i < leftSize; i++)
		{
			leftArray[i] = array[left + i];
		}
		for (int i = 0; i < rightSize; i++)
		{
			rightArray[i] = array[mid + 1 + i];
		}

		int i = 0, j = 0, k = left;

		while (i < leftSize && j < rightSize)
		{
			if (leftArray[i] >= rightArray[j])
			{
				array[k] = leftArray[i];
				i++;
			}
			else
			{
				array[k] = rightArray[j];
				j++;
			}
			k++;
		}

		while (i < leftSize)
		{
			array[k] = leftArray[i];
			i++;
			k++;
		}

		while (j < rightSize)
		{
			array[k] = rightArray[j];
			j++;
			k++;
		}
		delete[] leftArray;
		delete[] rightArray;
	}

	/****************************************************************************
	*							  Merge
	*************************************************************************//**
	*  @fn        void Sort<T>::MergeAscend(std::vector& vector, int left, int mid, int right)
	*  @brief     For Merge Sort
	*  @param[out]std::vector<T>& vector
	*  @param[in] int left
	*  @param[in] int mid,
	*  @param[in] int right
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::MergeAscend(std::vector<T>& vector, int left, int mid, int right)
	{
		int leftSize = mid - left + 1;
		int rightSize = right - mid;

		// create temporary subArray
		std::vector<T> leftArray, rightArray;
		leftArray .resize(leftSize);
		rightArray.resize(rightSize);

		for (int i = 0; i < leftSize; i++)
		{
			leftArray[i] = vector[(INT64)left + i];
		}
		for (int i = 0; i < rightSize; i++)
		{
			rightArray[i] = vector[(INT64)mid + 1 + i];
		}

		int i = 0, j = 0, k = left;

		while (i < leftSize && j < rightSize)
		{
			if (leftArray[i] <= rightArray[j])
			{
				vector[k] = leftArray[i];
				i++;
			}
			else
			{
				vector[k] = rightArray[j];
				j++;
			}
			k++;
		}

		while (i < leftSize)
		{
			vector[k] = leftArray[i];
			i++;
			k++;
		}

		while (j < rightSize)
		{
			vector[k] = rightArray[j];
			j++;
			k++;
		}
		leftArray .clear();
		rightArray.clear();
	}

	/****************************************************************************
	*							  Merge
	*************************************************************************//**
	*  @fn        void Sort<T>::MergeDescend(std::vector& vector, int left, int mid, int right)
	*  @brief     For Merge Sort
	*  @param[out]std::vector<T>& vector
	*  @param[in] int left
	*  @param[in] int mid,
	*  @param[in] int right
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::MergeDescend(std::vector<T>& vector, int left, int mid, int right)
	{
		int leftSize = mid - left + 1;
		int rightSize = right - mid;

		// create temporary subArray
		std::vector<T> leftArray, rightArray;
		leftArray.resize(leftSize);
		rightArray.resize(rightSize);

		for (int i = 0; i < leftSize; i++)
		{
			leftArray[i] = vector[left + i];
		}
		for (int i = 0; i < rightSize; i++)
		{
			rightArray[i] = vector[mid + 1 + i];
		}

		int i = 0, j = 0, k = left;

		while (i < leftSize && j < rightSize)
		{
			if (leftArray[i] >= rightArray[j])
			{
				vector[k] = leftArray[i];
				i++;
			}
			else
			{
				vector[k] = rightArray[j];
				j++;
			}
			k++;
		}

		while (i < leftSize)
		{
			vector[k] = leftArray[i];
			i++;
			k++;
		}

		while (j < rightSize)
		{
			vector[k] = rightArray[j];
			j++;
			k++;
		}
		leftArray.clear();
		rightArray.clear();
	}

	/****************************************************************************
	*							QuickSortAscend
	*************************************************************************//**
	*  @fn        void Sort<T>::QuickSortAscend(std::vector<T>& vector, int left, int right)
	*  @brief     Quick Sort (If the number of index is small (<= 64), switch to insertion sort for performance)
	*  @param[out]std::vector<T>& vector
	*  @param[in] int left index
	*  [param[in] int right index
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::QuickSortAscend(std::vector<T>& vector, int left, int right)
	{

		if (right - left <= QUICK_THREASHOLD)
		{
			InsertionSortAscend(vector);
			return;
		}

		// Define pivot
		T pivot = Median(vector[left], vector[(left + right) / 2], vector[right]);

		// Division
		int l = left;
		int r = right;

		while (l <= r)
		{
			while (l < right && (vector[l] - pivot) < 0)l++;
			while (r > left  && (vector[r] - pivot) >= 0)r--;
			if (l > r)break;
			std::swap(vector[l], vector[r]);
			l++; r--;
		}

		QuickSortAscend(vector, left, l - 1);
		QuickSortAscend(vector, l, right);
	}

	/****************************************************************************
	*							QuickSortDescend
	*************************************************************************//**
	*  @fn        void Sort<T>::QuickSortDescend(std::vector<T>& vector, int left, int right)
	*  @brief     Quick Sort (If the number of index is small (<= 64), switch to insertion sort for performance)
	*  @param[out]std::vector<T>& vector
	*  @param[in] int left index
	*  [param[in] int right index
	*  @return 　　void
	*****************************************************************************/
	template<typename T> void Sort<T>::QuickSortDescend(std::vector<T>& vector, int left, int right)
	{

		if (right - left <= QUICK_THREASHOLD)
		{
			InsertionSortDescend(vector);
			return;
		}

		// Define pivot
		T pivot = Median(vector[left], vector[(left + right) / 2], vector[right]);

		// Division
		int l = left;
		int r = right;

		while (l <= r)
		{
			while (l < right && (vector[l] - pivot) > 0)l++;
			while (r > left && (vector[r] - pivot) <= 0)r--;
			if (l > r)break;
			std::swap(vector[l], vector[r]);
			l++; r--;
		}

		QuickSortDescend(vector, left, l - 1);
		QuickSortDescend(vector, l, right);
	}

	/****************************************************************************
	*							  Median
	*************************************************************************//**
	*  @fn        static T Sort<T>::Median(T& a, T& b, T& c);
	*  @brief     For Quick Sort
	*  @param[out]T
	*  @return 　　void
	*****************************************************************************/
	template<typename T> T Sort<T>::Median(T& a, T& b, T& c)
	{
		if (a - b > 0) std::swap(a, b);
		if (a - c > 0) std::swap(a, c);
		if (b - c > 0) std::swap(b, c);
		return b;
	}
#pragma endregion Private Function
}

#endif