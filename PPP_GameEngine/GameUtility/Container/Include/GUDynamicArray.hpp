//////////////////////////////////////////////////////////////////////////////////
///             @file   GUArray.hpp
///             @brief  未実装
///             @author toide
///             @date   2023/11/25 14:04:42
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_DYNAMIC_ARRAY_HPP
#define GU_DYNAMIC_ARRAY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Container/Include/GUInitializerList.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   DynamicArray
	*************************************************************************//**
	*  @class     DynamicArray
	*  @brief     動的配列
	*****************************************************************************/
	template<class ElementType>
	class DynamicArray
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : サイズを強制的に変更します.
		*           条件に応じて以下の挙動をします. 
		*           size <= 元々入っているサイズ : 何も行わない
		*           size >  元々入っているサイズ : 指定サイズ =　capacityの領域を確保した上でコンストラクタ呼び出しまで行う
		*  
		*  @param[in] const uint64 配列のサイズ (capacityではありません)
		*  @param[in] const bool   コピーコンストラクタを使用するか (falseにすると効率化のために未初期化状態で始まります.)
		*  @param[in] const ElementType& コピー元の要素 (基本デフォルトコンストラクタから作られたもの.)
		/*----------------------------------------------------------------------*/
		void Resize(const uint64 size, const bool useCopyConstructor = true, const ElementType& defaultElement = ElementType());

		/*----------------------------------------------------------------------
		*  @brief : メモリ領域を事前に確保します. (ただし, コンストラクタの呼び出しは全く行いません)
		*           Resizeとの違いとして, Sizeの変更はなく, Capacityだけが変わります. 
		*           そのため, At関数などを用いて配列にランダムアクセスを行うとエラーとなります. 
		/*----------------------------------------------------------------------*/
		void Reserve(const uint64 capacity);

		/*----------------------------------------------------------------------
		*  @brief : サイズを0にしますが, Capacity領域自体はそのままにします.
		*           デストラクタは呼び出されます. 
		/*----------------------------------------------------------------------*/
		void Clear();

		/*----------------------------------------------------------------------
		*  @brief : CapacityをコンテナのSizeまで切り詰める
		*           capacityが0のとき, capacity以上にsizeがある時は処理を行いません.
		/*----------------------------------------------------------------------*/
		void ShrinkToFit();

		/*----------------------------------------------------------------------
		*  @brief : 配列を後ろに追加する. Capacityを超えた場合, 全体のメモリを2倍の大きさで再割り当てを行う.
		/*----------------------------------------------------------------------*/
		void Push(const ElementType& element);

		/*----------------------------------------------------------------------
		*  @brief : 配列を後ろから取り出す. その際, デストラクタを呼び出す.
		/*----------------------------------------------------------------------*/
		void Pop();

		/*----------------------------------------------------------------------
		*  @brief : 指定した要素が中に含まれている場合はtrueを返します.
		/*----------------------------------------------------------------------*/
		bool Contains(const ElementType& element) const;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : ヒープ領域に格納されている先頭のポインタを返す
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType* Data()       { return _data; }
		__forceinline const ElementType* Data() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief : 先頭の要素
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Front()       {return _data;}
		__forceinline const ElementType& Front() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief :　最後の要素
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Back()        { return _data[_size - 1]; }
		__forceinline const ElementType& Back() const { return _data[_size - 1]; }

		/*----------------------------------------------------------------------
		*  @brief : 配列が空かどうか
		/*----------------------------------------------------------------------*/
		__forceinline bool IsEmpty() const { return _size == 0; }

		/*----------------------------------------------------------------------
		*  @brief : 配列の要素数
		/*----------------------------------------------------------------------*/
		__forceinline uint64 Size() const { return _size; }

		/*----------------------------------------------------------------------
		*  @brief : 配列に詰め込める最大要素数
		/*----------------------------------------------------------------------*/
		__forceinline uint64 Capacity() const { return _capacity; }

		/*----------------------------------------------------------------------
		*  @brief : 要素のバイト数を返す
		/*----------------------------------------------------------------------*/
		__forceinline static constexpr uint32 ByteOfElement() { return sizeof(ElementType); }

		/*----------------------------------------------------------------------
		*  @brief : 配列に詰め込める残りの要素数を返す (capacity - size)
		/*----------------------------------------------------------------------*/
		__forceinline uint64 ResidueCount() const { return _capacity - _size; }

		/*----------------------------------------------------------------------
		*  @brief : 指定したIndexが領域内かどうか, 範囲外であっても途中で止めない
		/*----------------------------------------------------------------------*/
		__forceinline bool InRange(const uint64 index) const { return 0 <= index && index <= _size; }

		/*----------------------------------------------------------------------
		*  @brief : 指定したIndexが領域内かどうか, 範囲外であったら途中で止める
		/*----------------------------------------------------------------------*/
		__forceinline void CheckRange(const uint64 index) const
		{
			Checkf(0 <= index && index <= _size, "index is out of range. \n");
		}

		/*----------------------------------------------------------------------
		*  @brief : データを範囲確認を行った上で返す. 
		*           範囲外に入った場合はブレークポイントが強制的にかかり, 処理が止まります. 
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& At(const uint64 index)       { CheckRange(index); return _data[index]; }
		__forceinline const ElementType& At(const uint64 index) const { CheckRange(index); return _data[index]; }

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
		DynamicArray& operator=(std::initializer_list<ElementType> list)
		{
			Clear();
			CreateFromOtherArray(list.begin(), list.size());
			return *this;
		}

		// ほかのArray(型が同じ)から代入
		DynamicArray& operator=(const DynamicArray& other)
		{
			if (this != &other)
			{
				Clear();
				CreateFromOtherArray(other.Data(), other.Size());
			}
			return *this;
		}

		// ムーブコンストラクタ
		DynamicArray& operator=(DynamicArray&& other)
		{
			if (this != &other)
			{
				// ヒープ領域の全体私は時間がかかるため, あくまでポインタの付け替えだけで対応しました.
				_data     = other._data;     other._data     = nullptr;
				_size     = other._size;     other._size     = nullptr;
				_capacity = other._capacity; other._capacity = nullptr;
			}
			return *this;
		}
#pragma endregion Operator Function
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		DynamicArray() : _size(0), _capacity(0), _data(nullptr) {};

		// 配列の要素から作成するコンストラクタ
		DynamicArray(const ElementType* pointer, const uint64 count)
		{
			CreateFromOtherArray(pointer, count);
		}

		// 波括弧で初期化が出来るコンストラクタ
		DynamicArray(std::initializer_list<ElementType> list)
		{
			CreateFromOtherArray(list.begin(), list.size());
		}

		// コピーコンストラクタ
		DynamicArray(const DynamicArray& other) { CreateFromOtherArray(other.Data(), other.Size()); }

		// ムーブコンストラクタ
		DynamicArray(DynamicArray&& other)
		{
			// ヒープ領域の全体私は時間がかかるため, あくまでポインタの付け替えだけで対応しました.
			_data     = other._data;     other._data     = nullptr;
			_size     = other._size;     other._size     = nullptr;
			_capacity = other._capacity; other._capacity = nullptr;
		}

		~DynamicArray()
		{
			if (_data) 
			{
				Memory::ForceExecuteDestructors(_data, _size);
				Memory::Free(_data); 
			} 
		}
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		// @brief : 実際に数値が入力されているIndexの個数
		uint64 _size     = 0;

		// @brief : 最大限入れられる容量
		uint64 _capacity = 0;

		// @brief : 要素を格納する配列の先頭ポインタ
		ElementType* _data = nullptr;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void CreateFromOtherArray(const ElementType* pointer, const uint64 count);

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};

#pragma region Implement
#pragma region Constructor and Destructor
	
#pragma endregion Constructor and Destructor
	template<class ElementType>
	void DynamicArray<ElementType>::CreateFromOtherArray(const ElementType* pointer, const uint64 count)
	{
		Check(pointer != nullptr);
		Resize(count, false);

		if constexpr (IS_ARITHMETIC<ElementType>)
		{
			Memory::Copy(_data, pointer, sizeof(ElementType) * count)
		}
		else
		{
			Memory::ForceExecuteCopyConstructors(_data, pointer, count);
		}
	}

	/****************************************************************************
	*                    Resize
	*************************************************************************//**
	*  @fn       　void DynamicArray<ElementType>::Reserve(const uint64 capacity)
	*
	*  @brief     サイズを変更します
	*
	*  @param[in] const uint64 配列のサイズ (capacityではありません)
	*  @param[in] const bool   コピーコンストラクタを使用するか (falseにすると効率化のために未初期化状態で始まります.)
	* 　@param[in] const ElementType& コピー元の要素 (基本デフォルトコンストラクタから作られたもの.)
	*
	*  @return 　　void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Resize(const uint64 size, const bool useConstructor, const ElementType& defaultElement)
	{
		if (size <= _size) { return; }

		// 必ず初期化
		Reserve(size);
		
		// 初期化コピーを行う回数を設定する
		const auto residueSize = size - _size;

		// コピーコンストラクタを使う場合だけ初期化する
		if (useConstructor)
		{
			if constexpr (sizeof(ElementType) == 1) // bool or character or byte
			{
				Memory::Set(_data, static_cast<uint8>(defaultElement), size); // 1 byteであることは分かっているので
			}
			else
			{
				for (uint64 i = 0; i < size; ++i)
				{
					_data[i] = defaultElement;
				}
			}
		}

		// サイズの変更
		_size = size;
	}

	/****************************************************************************
	*                    Reserve
	*************************************************************************//**
	*  @fn       　void DynamicArray<ElementType>::Reserve(const uint64 capacity)
	*
	*  @brief     メモリ領域を事前に確保します. (ただし, コンストラクタの呼び出しは全く行いません)
	*
	*  @param[in] const uint64 capacity (最大限入れられる容量(配列数), バイトサイズではないことに注意)
	*
	*  @return 　　void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Reserve(const uint64 capacity)
	{
		/*-------------------------------------------------------------------
		-           もともとより小さかったら処理をしない
		---------------------------------------------------------------------*/
		if (capacity <= _capacity) { return; }

		/*-------------------------------------------------------------------
		-           もともと配列が存在していれば全てのメモリをコピーしたうえで削除する
		---------------------------------------------------------------------*/
		auto newData = Memory::Allocate(capacity);

		if (_data != nullptr && _capacity)
		{
			Memory::Copy(newData, _data, _capacity * sizeof(ElementType));
			Memory::Free(_data);
		}

		/*-------------------------------------------------------------------
		-           Dataの更新
		---------------------------------------------------------------------*/
		_data     = (ElementType*)newData;
		_capacity = capacity;
	}

	/****************************************************************************
	*                    Clear
	*************************************************************************//**
	*  @fn       　void DynamicArray<ElementType>::Clear()
	*
	*  @brief     サイズを0にしますが, Capacity領域自体はそのままにします
	*
	*  @param[in] void
	*
	*  @return 　　void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Clear()
	{
		/*-------------------------------------------------------------------
		-           サイズが0なら何もしない
		---------------------------------------------------------------------*/
		if (Size() == 0) { return; }

		/*-------------------------------------------------------------------
		-           デストラクタを一通り呼び出してからリセット
		---------------------------------------------------------------------*/
		Memory::ForceExecuteDestructors(_data, Size());

		_size = 0;
	}

	/****************************************************************************
	*                    ShrinkToFit
	*************************************************************************//**
	*  @fn       　void DynamicArray<ElementType>::ShrinkToFit()
	*
	*  @brief      CapacityをコンテナのSizeまで切り詰める 
	*
	*  @param[in] void
	*
	*  @return 　　void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::ShrinkToFit()
	{
		/*-------------------------------------------------------------------
		-           適切なCapacityかを調べる
		---------------------------------------------------------------------*/
		if (_capacity == 0)     { return; }
		if (_size >= _capacity) { return; }

		/*-------------------------------------------------------------------
		-           今回の場合は_capacityの方が常にsizeより大きいことが分かっているため, 
		            _dataに直接代入を行っている. 
		---------------------------------------------------------------------*/
		_data     = (ElementType*)Memory::Reallocate(_data, _size * sizeof(ElementType));
		_capacity = _size;
	}

	/****************************************************************************
	*                    Push
	*************************************************************************//**
	*  @fn       　void DynamicArray<ElementType>::ShrinkToFit()
	*
	*  @brief      配列を後ろに追加する. Capacityを超えた場合, 全体のメモリを2倍の大きさで再割り当てを行う.
	*
	*  @param[in] ElementType& element
	*
	*  @return 　　void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Push(const ElementType& element)
	{
		if (_capacity <= _size)
		{
			Reserve(_capacity == 0 ? 1 : _size * 2);
		}

		_data[_size] = element;
		++_size;
	}

	/****************************************************************************
	*                    Pop
	*************************************************************************//**
	*  @fn       　void DynamicArray<ElementType>::Pop()
	*
	*  @brief     最後の要素を取り出してデストラクタを呼び出す. (メモリ自体の破棄は行われず, その後サイズだけ変更します)
	*
	*  @param[in] void
	*
	*  @return 　　void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Pop()
	{
		if (_size == 0) { return; }

		Memory::ForceExecuteDestructors(&_data[_size - 1], 1);
		_size--;
	}

	/****************************************************************************
	*                    Contains
	*************************************************************************//**
	*  @fn       　bool DynamicArray<ElementType>::Contains(const ElementType& element) const
	*
	*  @brief     指定した要素が中に含まれている場合はtrueを返します.
	*
	*  @param[in] const ElementType& element
	*
	*  @return 　　bool 含まれている場合はtrue
	*****************************************************************************/
	template<class ElementType>
	bool DynamicArray<ElementType>::Contains(const ElementType& element) const
	{
		for (uint64 i = 0; i < _size; ++i)
		{
			if(*_data[i] == element)
			{
				return true;
			}
		}
		return false;
	}
#pragma endregion Implement
}

#endif