//////////////////////////////////////////////////////////////////////////////////
///             @file   GUStaticArray.hpp
///             @brief  temp
///             @author toide
///             @date   2024/02/18 3:08:51
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_STATIC_ARRAY_HPP
#define GU_STATIC_ARRAY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Container/Include/GUInitializerList.hpp"
#include "../Private/Iterator/Include/GUIteratorIncludes.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			   GUStaticArray
	****************************************************************************/
	/* @class     GUStaticArray
	*  @brief     temp
	*****************************************************************************/
	template<class ElementType, uint64 ArraySize>
	class StaticArray
	{
	public:
		static constexpr uint64 INDEX_NONE = static_cast<uint64>(-1);

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : コンテナを指定された値で埋める
		/*----------------------------------------------------------------------*/
		void Fill(const ElementType& element);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : ヒープ領域に格納されている先頭のポインタを返す
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType* Data()       { return _data;}
		__forceinline const ElementType* Data() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief : 先頭の要素
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Front()       { return _data; }
		__forceinline const ElementType& Front() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief : 最後の要素
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Back()       { return _data[ArraySize - 1]; }
		__forceinline const ElementType& Back() const { return _data[ArraySize - 1]; }

		/*----------------------------------------------------------------------
		*  @brief : 配列が空かどうか
		/*----------------------------------------------------------------------*/
		__forceinline bool IsEmpty() const { return ArraySize == 0; }

		/*----------------------------------------------------------------------
		*  @brief : 配列の要素数
		/*----------------------------------------------------------------------*/
		__forceinline uint64 Size() const { return ArraySize; }

		/*----------------------------------------------------------------------
		*  @brief : 指定したIndexが領域内かどうか, 範囲外であっても途中で止めない
		/*----------------------------------------------------------------------*/
		__forceinline bool InRange(const uint64 index) const { return 0 <= index && index <= ArraySize; }

		/*----------------------------------------------------------------------
		*  @brief : 指定したIndexが領域内かどうか, 範囲外であったら途中で止める
		/*----------------------------------------------------------------------*/
		__forceinline void CheckRange(const uint64 index) const
		{
			Checkf(0 <= index && index <= ArraySize, "index is out of range. \n");
		}

		/*----------------------------------------------------------------------
		*  @brief : データを範囲確認を行った上で返す.
		*           範囲外に入った場合はブレークポイントが強制的にかかり, 処理が止まります.
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& At(const uint64 index)       { CheckRange(index); return _data[index]; }
		__forceinline const ElementType& At(const uint64 index) const { CheckRange(index); return _data[index]; }

#pragma region Iterator Function
		Iterator<ElementType>             begin       ()       { return Iterator<ElementType>            (_data); }
		ConstIterator<ElementType>        begin       () const { return ConstIterator<ElementType>       (_data); }
		Iterator<ElementType>             end         ()       { return Iterator<ElementType>            (_data + _size); }
		ConstIterator<ElementType>        end         () const { return ConstIterator<ElementType>       (_data + _size); }
		ReverseIterator<ElementType>      rbegin      ()       { return ReverseIterator<ElementType>     (_data + _size); }
		ReverseConstIterator<ElementType> rbegin      () const { return ReverseConstIterator<ElementType>(_data + _size); }

#pragma endregion Iterator Function
		
#pragma region Operator Function
		// 高速化のために範囲チェックを施していません. 範囲チェックを行いたい場合はAtを使用してください
		__forceinline ElementType& operator[](const uint64 index)
		{
			return _data[index];
		}
		// 高速化のために範囲チェックを施していません. 範囲チェックを行いたい場合はAtを使用してください
		__forceinline const ElementType& operator[](const uint64 index) const
		{
			return _data[index];
		}

		// 波括弧を使って代入が行えるようにする
		StaticArray& operator=(std::initializer_list<ElementType> list)
		{
			if (_data) { delete[](_data); }
			CreateFromOtherArray(list.begin(), list.size());
			return *this;
		}

		// ほかのArray(型が同じ)から代入
		StaticArray& operator=(const StaticArray& other)
		{
			if (this != &other)
			{
				if (_data) { delete[](_data); }
				CreateFromOtherArray(other.Data(), other.Size());
			}
			return *this;
		}

		// ムーブコンストラクタ
		StaticArray& operator=(StaticArray&& other)
		{
			if (this != &other)
			{
				// ヒープ領域の全体私は時間がかかるため, あくまでポインタの付け替えだけで対応しました.
				_data = other._data;     other._data = nullptr;
				_size = other._size;     other._size = 0;
			}
			return *this;
		}
#pragma endregion Operator Function
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		StaticArray() { _data = new ElementType[ArraySize]; }

		StaticArray(std::initializer_list<ElementType> list)
		{
			CreateFromOtherArray(list.begin(), list.size());
		}

		// コピーコンストラクタ
		StaticArray(const StaticArray& other) { CreateFromOtherArray(other.Data(), other.Size()); }

		// ムーブコンストラクタ
		StaticArray(StaticArray&& other) noexcept
		{
			// ヒープ領域の全体私は時間がかかるため, あくまでポインタの付け替えだけで対応しました.
			_data = other._data;     other._data = nullptr;
			_size = other._size;     other._size = 0;
		}

		~StaticArray()
		{
			if (_data) { delete[](_data); }
		}

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void CreateFromOtherArray(const ElementType* pointer, const uint64 count);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/

		// @brief : 要素を格納する配列の先頭ポインタ
		ElementType* _data = nullptr;
	};

#pragma region Implement
	template<class ElementType, uint64 ArraySize>
	void StaticArray<ElementType, ArraySize>::CreateFromOtherArray(const ElementType* pointer, const uint64 count)
	{
		Check(ArraySize == count);
		Check(ArraySize > 0);

		// nullチェック
		if (pointer == nullptr) { return; }

		// 一度破棄
		if (_data) { delete[](_data); }

		// 配列の領域を確保してコピー
		_data = new ElementType[count];
		Memory::Copy(_data, pointer, sizeof(ElementType) * count);
	}

	/*----------------------------------------------------------------------
	*  @brief : コンテナを指定された値で埋める
	/*----------------------------------------------------------------------*/
	template<class ElementType, uint64 ArraySize>
	void StaticArray<ElementType, ArraySize>::Fill(const ElementType& element)
	{
		Check(ArraySize > 0);

		if constexpr (sizeof(ElementType) == 1)
		{
			Memory::Set(_data, static_cast<uint8>(element), ArraySize);
		}
		else
		{
			for (uint64 i = 0; i < ArraySize; ++i)
			{
				_data[i] = element;
			}
		}
	}
#pragma endregion Implement
}

#endif