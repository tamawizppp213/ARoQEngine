//////////////////////////////////////////////////////////////////////////////////
/// @file   GUDeque.hpp
/// @brief  循環バッファによる二重両端キュー. Pushで先頭や末尾に要素を追加し, Popで先頭や末尾から要素を取り出すことができます. @n
///         また, このクラスは[]によるランダムアクセスも可能です. @n
/// @author toide
/// @date   2024/02/01 21:33:53
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_DEQUE_HPP
#define GU_DEQUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
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
	*				  			   Deque
	****************************************************************************/
	/*!  @brief     循環バッファによる二重両端キュー. Pushで先頭や末尾に要素を追加し, Popで先頭や末尾から要素を取り出すことができます. @n
	*               また, このクラスは[]によるランダムアクセスも可能です. @n
	*****************************************************************************/
	template<class ElementType>
	class Deque
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     キューの先頭に要素を追加します.
		*  @param[in] const ElementType& 格納するデータ. (代入時には=を使ったコピーが生じます)
		*  @return    void
		*************************************************************************/
		void PushFront(const ElementType& element);

		/*!**********************************************************************
		*  @brief     キューの末尾に要素を追加します.
		*  @param[in] const ElementType& 格納するデータ (代入時には=を使ったコピーが生じます)
		*  @return    void
		*************************************************************************/
		void PushBack(const ElementType& element);

		/*!**********************************************************************
		*  @brief     キューの先頭から要素を除去します. デストラクタも呼ばれます
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		bool PopFront();

		/*!**********************************************************************
		*  @brief     キューの末尾から要素を除去します. デストラクタも呼ばれます
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		bool PopBack();

		/*!**********************************************************************
		*  @brief     キューのメモリを事前に確保します. 最初に容量を定義することで, メモリの再確保を防ぎます
		*  @param[in] const uint64 capacity 確保するキューの容量 (個数)
		*  @return    void
		*************************************************************************/
		void Reserve(const uint64 capacity);

		/*!**********************************************************************
		*  @brief     サイズを0にしますが, Capacity領域はそのままにします
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Clear();
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     キューサイズが0かどうかを返します
		*  @param[in] void
		*  @return    bool キューサイズが0ならtrue, それ以外はfalse
		*************************************************************************/
		__forceinline bool IsEmpty() const { return _queueSize == 0; }

		/*!**********************************************************************
		*  @brief     キューの先頭要素を返します
		*  @param[in] void
		*  @return    ElementType& キューの先頭の要素
		*************************************************************************/
		__forceinline       ElementType& Front()       { return _data[_frontIndex]; }

		/*!**********************************************************************
		*  @brief     キューの先頭要素をconst参照で返します
		*  @param[in] void
		*  @return    const ElementType& キューの先頭の要素
		*************************************************************************/
		__forceinline const ElementType& Front() const { return _data[_frontIndex]; }

		/*!**********************************************************************
		*  @brief     キューの末尾要素を返します
		*  @param[in] void
		*  @return    ElementType& キューの末尾の要素
		*************************************************************************/
		__forceinline       ElementType& Back()       { return _data[(_frontIndex + _capacity + _queueSize - 1) % _capacity]; }

		/*!**********************************************************************
		*  @brief     キューの末尾要素をconst参照で返します
		*  @param[in] void
		*  @return    const ElementType& キューの末尾の要素
		*************************************************************************/
		__forceinline const ElementType& Back() const { return _data[(_frontIndex + _capacity + _queueSize - 1) % _capacity]; }

		/*!**********************************************************************
		*  @brief     キューの先頭ポインタを返します
		*  @param[in] void
		*  @return    ElementType* キューの先頭ポインタ
		*************************************************************************/
		__forceinline       ElementType* Data()       { return &_data[_frontIndex]; }

		/*!**********************************************************************
		*  @brief     キューの先頭ポインタをconstで返します
		*  @param[in] void
		*  @return    const ElementType* キューの先頭ポインタ
		*************************************************************************/
		__forceinline const ElementType* Data() const { return &_data[_frontIndex]; }

		/*!**********************************************************************
		*  @brief     現在格納されているキューのサイズを取得します. (Reserveで事前に確保されている要素数ではありません)
		*  @param[in] void
		*  @return    uint64 キューのサイズ
		*************************************************************************/
		__forceinline uint64 Size() const { return _queueSize; }

		/*!**********************************************************************
		*  @brief     現在格納しうる最大のキュー = Reserveで確保されている個数を取得します
		*  @param[in] void
		*  @return    uint64 Reserveで確保されている個数
		*************************************************************************/
		__forceinline uint64 Capacity() const { return _capacity; }

		/*!**********************************************************************
		*  @brief     現在格納可能な残りのキューサイズを取得します
		*  @param[in] void
		*  @return    uint64 現在格納可能な残りのキューサイズ
		*************************************************************************/
		__forceinline uint64 Residue() const { return _capacity - _queueSize; }
		#pragma endregion

		#pragma region Public Operator
		/*! @brief ランダムアクセス用*/
		__forceinline       ElementType& operator[](const uint64 index)       noexcept { return _data[(_frontIndex + _capacity + index) % _capacity]; }
		
		/*! @brief ランダムアクセス用*/
		__forceinline const ElementType& operator[](const uint64 index) const noexcept { return _data[(_frontIndex + _capacity + index) % _capacity]; }
		
		/*! @brief コピー*/
		Deque<ElementType>& operator=(const Deque<ElementType>& right) 
		{ 
			CopyFrom(right); 
			return *this; 
		}

		/*! @brief ムーブ*/
		Deque<ElementType>& operator=(Deque<ElementType>&& right)
		{
			if(this != &right)
			{
				if (_data) { Memory::Free(_data); }
				Memory::Copy(this, &right, sizeof(right));
				right.Initialize();
			} 
			return *this;
		}
		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		Deque() = default;

		/*!**********************************************************************
		*  @brief     格納可能な要素数を指定して初期化します
		*  @param[in] const uint64 capacity 確保するキューの容量 (個数)
		*************************************************************************/
		explicit Deque(const uint64 capacity) { Reserve(capacity); }

		/*!**********************************************************************
		*  @brief     別のDequeをコピーして初期化します
		*  @param[in] const Deque<ElementType>& キューのコピー元
		*************************************************************************/
		Deque(const Deque<ElementType>& deque) { CopyFrom(deque); }

		/*!**********************************************************************
		*  @brief     {, , ...}で初期化します
		*  @param[in] 初期化子リスト
		*************************************************************************/
		Deque(const std::initializer_list<ElementType> list)
		{
			if (_data) { Memory::Free(_data); }

			const auto listSize     = list.size();
			const auto capacitySize = listSize * 2;

			_data = (ElementType*)Memory::Allocate(capacitySize * sizeof(ElementType)); // capacity用
			Memory::ForceExecuteCopyConstructors(_data, list.begin(), list.size());
			
			_capacity   = capacitySize;
			_queueSize  = listSize;
			_frontIndex = 0;
		}

		/*!**********************************************************************
		*  @brief     別のDequeをムーブして初期化します
		*  @param[in] Deque<ElementType>&& キューのムーブ元
		*************************************************************************/
		Deque(Deque<ElementType>&& deque) noexcept 
		{
			if (_data) { Memory::Free(_data); }
			
			Memory::ForceExecuteMoveConstructors(_data, deque, deque.Size());
			deque.Initialize();
		}

		/*! @brief デストラクタ*/
		~Deque()
		{
			if (Capacity() == 0) { return; }

			Clear();
			Memory::Free(_data);
		}
		#pragma endregion
	protected:
		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     別のDequeをコピーします
		*  @param[in] const Deque<ElementType>& キューのコピー元
		*************************************************************************/
		void CopyFrom(const Deque<ElementType>& deque);

		/*!**********************************************************************
		*  @brief     0初期化
		*  @param[in] void
		*************************************************************************/
		__forceinline void Initialize() noexcept
		{
			Memory::Zero(_data, _capacity * sizeof(ElementType));
			_queueSize  = 0;
			_frontIndex = 0;
		}
		#pragma endregion
		
		#pragma region Protected Property
		/*! @brief : 現在の容量*/
		uint64 _capacity = 0;

		/*! 現在の要素数 */
		uint64 _queueSize = 0;

		/* 初期Index(これは循環バッファを使用し, pushFrontによって初期値がずれるためである)*/
		uint64 _frontIndex = 0;

		/* @brief 要素を格納する配列*/
		ElementType* _data = nullptr;
		#pragma endregion
	};

	/*-------------------------------------------------------------------
	-           @brief : メモリをコピーします
	---------------------------------------------------------------------*/
	template<class ElementType>
	void Deque<ElementType>::CopyFrom(const Deque<ElementType>& deque)
	{
		/*-------------------------------------------------------------------
		-           メモリが既に存在していたらメモリを破棄する
		---------------------------------------------------------------------*/
		if (Size())
		{
			Clear();
			Memory::Free(_data);
		}

		/*-------------------------------------------------------------------
		-           メモリ領域を確保
		---------------------------------------------------------------------*/
		_data = Memory::Allocate(deque.Capacity() * sizeof(ElementType));

		/*-------------------------------------------------------------------
		-           CopyConsturctorを呼び出す
		---------------------------------------------------------------------*/
		Memory::ForceExecuteCopyConstructors(_data, deque.Data(), deque.Size());

		/*-------------------------------------------------------------------
		-           設定
		---------------------------------------------------------------------*/
		_capacity   = deque._capacity;
		_queueSize  = deque._queueSize;
		_frontIndex = deque._frontIndex;
	}

	template<class ElementType>
	void Deque<ElementType>::Clear()
	{
		if (Size())
		{
			// 循環バッファを採用しているため, BackがFrontよりも前のポインタになっている場合がある
			ElementType* frontPointer = &Front();
			ElementType* backPointer  = &Back();

			if (frontPointer < backPointer)
			{
				Memory::ForceExecuteDestructors(frontPointer, Size());
			}
			else
			{
				Memory::ForceExecuteDestructors(frontPointer, &_data[_capacity] - frontPointer);
				Memory::ForceExecuteDestructors(_data, backPointer - _data);
			}
		}

		_queueSize  = 0;
		_frontIndex = 0;
	}

	template<class ElementType>
	void Deque<ElementType>::PushFront(const ElementType& element)
	{
		// 配列を超えたらメモリを再確保する
		if (_capacity == 0) { Reserve(1); }
		if (_capacity <= _queueSize)
		{
			Reserve(_queueSize * 2);
		}

		_frontIndex = (_frontIndex + _capacity - 1) % _capacity;
		_data[_frontIndex] = element;
		_queueSize++;
	}

	template<class ElementType>
	void Deque<ElementType>::PushBack(const ElementType& element)
	{
		// 配列を超えてきたらメモリを再確保する
		if (_capacity == 0) { Reserve(1); }
		if (_capacity <= _queueSize) 
		{
			Reserve(_queueSize * 2);
		}

		_data[(_frontIndex + _queueSize) % _capacity] = element;
		++_queueSize;
	}

	template<class ElementType>
	bool Deque<ElementType>::PopFront()
	{
		if (_queueSize == 0) { return false; }

		Memory::ForceExecuteDestructors(&Front(), 1);
		_frontIndex = (_frontIndex + 1) % _capacity;
		--_queueSize;
		return true;
	}

	template<class ElementType>
	bool Deque<ElementType>::PopBack()
	{
		if (_queueSize == 0) { return false; }

		Memory::ForceExecuteDestructors(&Back(), 1);
		--_queueSize;
		return true;
	}


	template<class ElementType>
	void Deque<ElementType>::Reserve(const uint64 capacity)
	{
		/*-------------------------------------------------------------------
		-           もともとより小さかったら処理をしない
		---------------------------------------------------------------------*/
		if (capacity <= _capacity) { return; }

		/*-------------------------------------------------------------------
		-           もともと配列が存在していれば全てのメモリをコピーしたうえで削除
		---------------------------------------------------------------------*/
		auto newData = Memory::Allocate(capacity);
		
		if (_data != nullptr && _capacity > 0)
		{
			Memory::Copy(newData, _data, _capacity * sizeof(ElementType));
			Memory::Free(_data);
		}

		/*-------------------------------------------------------------------
		-           Dataの更新
		---------------------------------------------------------------------*/
		_data     = newData;
		_capacity = capacity;
	}
}

#endif