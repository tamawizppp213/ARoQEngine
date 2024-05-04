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
#include "GameUtility/Container/Private/Iterator/Include/GUIteratorIncludes.hpp"
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
	* 　　　　　　　　https://qiita.com/ageprocpp/items/16aa225a1194fa0cf450
	*****************************************************************************/
	template<class ElementType>
	class DynamicArray
	{
	public:
		static constexpr uint64 INDEX_NONE = static_cast<uint64>(-1);

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     サイズを強制的に変更します.条件に応じて以下の挙動をします. @n
		*             size <= 元々入っているサイズ : 何も行わない @n
		*             size >  元々入っているサイズ : 指定サイズ =　capacityの領域を確保した上でコンストラクタ呼び出しまで行う
		*  @param[in] const uint64 配列のサイズ (capacityではありません)
		*  @param[in] const bool   コピーコンストラクタを使用するか (falseにすると効率化のために未初期化状態で始まります.)
		*  @param[in] const ElementType& コピー元の要素 (基本デフォルトコンストラクタから作られたもの.)
		*  @return    void
		*************************************************************************/
		void Resize(const uint64 size, const bool useCopyConstructor = true, const ElementType& defaultElement = ElementType());

		/*!**********************************************************************
		*  @brief     メモリ領域を事前に確保します. (ただし, コンストラクタの呼び出しは全く行いません) @n
		*             Resizeとの違いとして, Sizeの変更はなく, Capacityだけが変わります.  @n
		*             そのため, At関数などを用いて配列にランダムアクセスを行うとエラーとなります
		*  @param[in] const uint64 配列の要素数 (キャパシティ)
		*  @return    void
		*************************************************************************/
		void Reserve(const uint64 capacity);

		/*!**********************************************************************
		*  @brief     サイズを0にしますが, Capacity領域自体はそのままにします. @n
		*             デストラクタは呼び出されます. 
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Clear();

		/*!**********************************************************************
		*  @brief     CapacityをコンテナのSizeまで切り詰める @n
		*             capacityが0のとき, capacity以上にsizeがある時は処理を行いません.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void ShrinkToFit();

		/*!**********************************************************************
		*  @brief     配列を後ろに追加する. Capacityを超えた場合, 全体のメモリを2倍の大きさで再割り当てを行う.
		*  @param[in] const ElementType& 要素
		*  @return    void
		*************************************************************************/
		void Push(const ElementType& element);

		/*!**********************************************************************
		*  @brief     配列を後ろに追加する. Capacityを超えた場合, 全体のメモリを2倍の大きさで再割り当てを行う.
		*  @param[in] const ElementType&& move要素
		*  @return    void
		*************************************************************************/
		void Push(ElementType&& element);

		/*!**********************************************************************
		*  @brief     配列を後ろから取り出す. その際, デストラクタを呼び出す.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Pop();

		/*!**********************************************************************
		*  @brief     指定した要素が中に含まれている場合はtrueを返します.
		*  @param[in] const ElementType& 要素
		*  @return    bool trueの場合指定した要素が中に含まれている
		*************************************************************************/
		bool Contains(const ElementType& element) const;

		/*!**********************************************************************
		*  @brief     指定した配列インデックスを順方向から見つけます. 見つからなかった場合はIndexNoneを渡します
		*  @param[in] const ElementType& 要素
		*  @return    uint64 要素が同じものを見つけた場合はそのインデックスを, 見つからなかった場合はIndexNone(-1)を返します
		*************************************************************************/
		uint64 FindFromBegin(const ElementType& element) const;

		/*!**********************************************************************
		*  @brief     指定した配列インデックスを逆方向(配列要素の大きいもの)から見つけます. 見つからなかった場合はIndexNoneを渡します
		*  @param[in] const ElementType& 要素
		*  @return    uint64 要素が同じものを見つけた場合はそのインデックスを, 見つからなかった場合はIndexNone(-1)を返します
		*************************************************************************/
		uint64 FindFromEnd(const ElementType& element) const;

		/*!**********************************************************************
		*  @brief     指定したIndexにある要素を削除します
		*  @param[in] const uint64 index
		*  @param[in] const bool メモリ圧縮を行うか
		*  @return    void
		*************************************************************************/
		void RemoveAt(const uint64 index, const bool allowShrinking = true) { RemoveAtImplement(index, 1, allowShrinking); }

		/*!**********************************************************************
		*  @brief     指定したIndexにある要素を削除します
		*  @param[in] const uint64 削除開始index
		*  @param[in] const uint64 連続して削除する要素数
		*  @param[in] const bool メモリ圧縮を行うか
		*  @return    void
		*************************************************************************/
		void RemoveAt(const uint64 index, const uint64 removeCount, const bool allowShrinking = true) { RemoveAtImplement(index, removeCount, allowShrinking); }

		/*!**********************************************************************
		*  @brief     指定した要素を削除します
		*  @param[in] const ElementType& 要素
		*  @param[in] const bool メモリ圧縮を行うか
		*  @return    void
		*************************************************************************/
		void Remove(const ElementType& element, const bool allowShrinking = true) 
		{
			const auto index = FindFromBegin(element);
			if (index != INDEX_NONE)
			{ 
				RemoveAtImplement(index, 1, allowShrinking); 
			}
		}

		/*!**********************************************************************
		*  @brief     指定した要素を全て削除します. 返り値は削除した個数です.
		*  @param[in] const ElementType& 要素
		*  @param[in] const bool メモリ圧縮を行うか
		*  @return    gu::uint64 削除数
		*************************************************************************/
		gu::uint64 RemoveAll(const ElementType& element, const bool allowShrinking = true);
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     ヒープ領域に格納されている先頭のポインタを返す
		*  @param[in] void
		*  @return    ElementType* 要素の生ポインタ
		*************************************************************************/
		__forceinline ElementType* Data(){ return _data; }

		/*!**********************************************************************
		*  @brief     ヒープ領域に格納されている先頭のポインタを返す
		*  @param[in] void
		*  @return    ElementType* 要素の生ポインタ
		*************************************************************************/
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

		/*!**********************************************************************
		*  @brief     配列が空かどうかを判定します.
		*  @param[in] void
		*  @return    要素数が0であればtrueを返します
		*************************************************************************/
		__forceinline bool IsEmpty() const { return _size == 0; }

		/*!**********************************************************************
		*  @brief     既に確保済みの配列の要素数を返します. (Capacityではないです.) 
		*  @param[in] void
		*  @return    既に確保済みの配列の要素数
		*************************************************************************/
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

		#pragma endregion Public Property
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
				_size     = other._size;     other._size     = 0;
				_capacity = other._capacity; other._capacity = 0;
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

		DynamicArray(const uint64 size) { Resize(size); }

		DynamicArray(const uint64 size, const ElementType& defaultElement) { Resize(size, true, defaultElement); }

		// 波括弧で初期化が出来るコンストラクタ
		DynamicArray(std::initializer_list<ElementType> list)
		{
			CreateFromOtherArray(list.begin(), list.size());
		}

		// コピーコンストラクタ
		DynamicArray(const DynamicArray& other) { CreateFromOtherArray(other.Data(), other.Size()); }

		// ムーブコンストラクタ
		DynamicArray(DynamicArray&& other) noexcept
		{
			// ヒープ領域の全体私は時間がかかるため, あくまでポインタの付け替えだけで対応しました.
			_data     = other._data;     other._data     = nullptr;
			_size     = other._size;     other._size     = 0;
			_capacity = other._capacity; other._capacity = 0;
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
		**                Protected Property
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

		void RemoveAtImplement(const uint64 index, const uint64 count, const bool allowShrinking);

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
	};

#pragma region Implement
#pragma region Constructor and Destructor
	
#pragma endregion Constructor and Destructor
	template<class ElementType>
	void DynamicArray<ElementType>::CreateFromOtherArray(const ElementType* pointer, const uint64 count)
	{
		if (pointer == nullptr) { return; };

		Resize(count, false);

		if constexpr (type::IS_ARITHMETIC<ElementType>)
		{
			Memory::Copy(_data, pointer, sizeof(ElementType) * count);
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
				for (uint64 i = _size; i < _size + residueSize; ++i)
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
		auto newData = Memory::Allocate(capacity * sizeof(ElementType));

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
		if (_size != 0)
		{
			_data = (ElementType*)Memory::Reallocate(_data, _size * sizeof(ElementType));
		}
		else
		{
			if (_data)
			{
				Memory::Free(_data);
				_data = nullptr;
			}
		}
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
	template<class ElementType>
	void DynamicArray<ElementType>::Push(ElementType&& element)
	{
		if (_capacity <= _size)
		{
			Reserve(_capacity == 0 ? 1 : _size * 2);
		}
		_data[_size] = type::Forward<ElementType>(element);
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

	/****************************************************************************
	*                    RemoveAtImplement
	*************************************************************************//**
	*  @fn        void RemoveAtImplement(const uint64 index, const uint64 count, const bool allowShrinking)
	*
	*  @brief     Removeの実装
	*
	*  @param[in] const uint64 index
	*  @param[in] const uint64 size
	*  @param[in] const bool allowShrinking
	*
	*  @return 　　void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::RemoveAtImplement(const uint64 index, const uint64 removeCount, const bool allowShrinking)
	{
		/*-------------------------------------------------------------------
		-           範囲チェック
		---------------------------------------------------------------------*/
		if (removeCount <= 0)              { return; }
		if (!InRange(index + removeCount)) { return; }

		/*-------------------------------------------------------------------
		-           終了処理
		---------------------------------------------------------------------*/
		Memory::ForceExecuteDestructors(Data() + index, removeCount);

		/*-------------------------------------------------------------------
		-           メモリの移動
		---------------------------------------------------------------------*/
		const auto moveCount = _size - index - removeCount;
		
		if (moveCount > 0)
		{
			Memory::Move(&_data[index], &_data[index + removeCount], moveCount * sizeof(ElementType));
		}

		_size -= removeCount;

		if (allowShrinking)
		{
			ShrinkToFit();
		}
	}

	/****************************************************************************
	*                    FindFromBegin
	*************************************************************************//**
	*  @fn        uint64 DynamicArray<ElementType>::FindFromBegin(const ElementType& element) const
	*
	*  @brief     指定した配列インデックスを順方向に見つけます. 見つからなかった場合は-1を渡します
	*
	*  @param[in] const ElementType& element
	*
	*  @return 　　uint64
	*****************************************************************************/
	template<class ElementType>
	uint64 DynamicArray<ElementType>::FindFromBegin(const ElementType& element) const
	{
		ElementType* start = _data;

		for (ElementType* data = start; data != &_data[_size]; ++data)
		{
			if (*data == element)
			{
				return static_cast<uint64>(data - start);
			}
		}

		return INDEX_NONE;
	}

	/****************************************************************************
	*                    FindFromEnd
	*************************************************************************//**
	*  @fn        uint64 DynamicArray<ElementType>::FindFromEnd(const ElementType& element) const
	*
	*  @brief     指定した配列インデックスを逆方向に見つけます. 見つからなかった場合は-1を渡します
	*
	*  @param[in] const ElementType& element
	*
	*  @return 　　uint64
	*****************************************************************************/
	template<class ElementType>
	uint64 DynamicArray<ElementType>::FindFromEnd(const ElementType& element) const
	{
		const ElementType* end = Data() + _size;

		for (const ElementType* data = end; data != _data; )
		{
			--data;

			if (*data == element)
			{
				return static_cast<uint64>(data - start);
			}
		}

		return INDEX_NONE;
	}

	/****************************************************************************
	*                    RemoveAll
	*************************************************************************//**
	*  @fn        gu::uint64 DynamicArray<ElementType>::RemoveAll(const ElementType& element)
	*
	*  @brief     指定した要素を全て削除します.
	*
	*  @param[in] const ElementType& element
	*  @param[in] const bool allowShrinking (capacityを切り詰めるか)
	*
	*  @return 　　uint64
	*****************************************************************************/
	template<class ElementType>
	gu::uint64 DynamicArray<ElementType>::RemoveAll(const ElementType& element, const bool allowShrinking)
	{
		ElementType* start = _data;

		uint64 index = 0;
		uint64 findCounter = 0;

		for (ElementType* data = start; data != &_data[_size]; ++data, ++index)
		{
			if (*data == element)
			{
				RemoveAtImplement(findCounter, 1, false);
				index--;
				findCounter++;
			}
		}

		if (findCounter && allowShrinking)
		{
			ShrinkToFit();
		}

		return findCounter;
	}

#pragma endregion Implement
}

#endif