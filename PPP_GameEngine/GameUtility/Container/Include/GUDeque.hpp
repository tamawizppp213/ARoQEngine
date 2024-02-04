//////////////////////////////////////////////////////////////////////////////////
///             @file   GUDeque.hpp
///             @brief  二重両端キュー
///             @author toide
///             @date   2024/02/01 21:33:53
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GUDeque_HPP
#define GUDeque_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include <initializer_list>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUDeque
	*************************************************************************//**
	*  @class     GUDeque
	*  @brief     循環バッファによる二重両端キュー
	*****************************************************************************/
	template<class ElementType>
	class Deque
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*-------------------------------------------------------------------
		-           @brief : Add the element to the queue head.
		---------------------------------------------------------------------*/
		void PushFront(const ElementType&  element);

		/*-------------------------------------------------------------------
		-           @brief : Add the element to the queue tail.
		---------------------------------------------------------------------*/
		void PushBack(const ElementType& element);

		/*-------------------------------------------------------------------
		-           @brief : Remove the element to the queue head.
		---------------------------------------------------------------------*/
		bool PopFront();

		/*-------------------------------------------------------------------
		-           @brief : Remove the element to the queue tail.
		---------------------------------------------------------------------*/
		bool PopBack();

		/*-------------------------------------------------------------------
		-           @brief : Acquire the memory
		---------------------------------------------------------------------*/
		void Reserve(const uint64 capacity);

		/*-------------------------------------------------------------------
		-           @brief : サイズを0にします.
		---------------------------------------------------------------------*/
		void Clear() { _queueSize = 0; }

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		__forceinline bool IsEmpty() const { return _queueSize == 0; }

		/*-------------------------------------------------------------------
		-           @brief : return the queue front (=_tail) element
		---------------------------------------------------------------------*/
		__forceinline       ElementType& Front()       { return _data[_frontIndex]; }
		__forceinline const ElementType& Front() const { return _data[_frontIndex]; }

		/*-------------------------------------------------------------------
		-           @brief : return the queue front (=_tail) element
		---------------------------------------------------------------------*/
		__forceinline       ElementType& Back()       { return _data[_frontIndex + _queueSize - 1]; }
		__forceinline const ElementType& Back() const { return _data[_frontIndex + _queueSize - 1]; }

		/*-------------------------------------------------------------------
		-           @brief : 現在格納されているキューのサイズを取得します
		---------------------------------------------------------------------*/
		__forceinline uint64 Size() const { return _queueSize; }

		/*-------------------------------------------------------------------
		-           @brief : 現在格納しうる最大のキューを取得します
		---------------------------------------------------------------------*/
		__forceinline uint64 Capacity() const { return _capacity; }

		/*-------------------------------------------------------------------
		-           @brief : 現在格納可能な残りのキューサイズを取得します
		---------------------------------------------------------------------*/
		__forceinline uint64 Residue() const { return _capacity - _queueSize; }

#pragma region Operator 
		__forceinline       ElementType& operator[](const uint64 index)       noexcept { return _data[(_frontIndex + _capacity + index) % _capacity]; }
		__forceinline const ElementType& operator[](const uint64 index) const noexcept { return _data[(_frontIndex + _capacity + index) % _capacity]; }
		Deque<ElementType>& operator=(const Deque<ElementType>& right) { CopyFrom(right); return *this; }
		Deque<ElementType>& operator=(Deque<ElementType>&& right)
		{
			if(this != &right)
			{
				if (_data) { delete[](_data); }
				Memory::Copy(this, &right, sizeof(right));
				right.Initialize();
			} 
			return *this;
		}
#pragma endregion Operator

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Deque() = default;

		explicit Deque(const uint64 capacity) { Reserve(capacity); }

		Deque(const Deque<ElementType>& deque) { CopyFrom(deque); }

		Deque(const std::initializer_list<ElementType> list)
		{
			if (_data) { delete[](_data); }
			_data = new ElementType[list.size() * 2]; // capacity用
			Memory::Copy(_data, list.begin(), list.size() * sizeof(ElementType));
			_capacity   = list.size() * 2;
			_queueSize  = list.size();
			_frontIndex = 0;
		}

		Deque(Deque<ElementType>&& deque) noexcept 
		{
			if (_data) { delete[](_data); }
			Memory::Copy(this, &deque, sizeof(deque));
			deque.Initialize();
		}

		~Deque()
		{
			if (_data) 
			{ 
				delete[] _data;
			}
		}

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void CopyFrom(const Deque<ElementType>& deque) 
		{
			if (_data) { delete[](_data); }

			_data = new ElementType[deque.Capacity()];
			Memory::Copy(_data, deque._data, sizeof(ElementType) * deque.Capacity());
			_capacity   = deque._capacity;
			_queueSize  = deque._queueSize;
			_frontIndex = deque._frontIndex;
		}

		__forceinline void Initialize() noexcept
		{
			Memory::Zero(_data, _capacity * sizeof(ElementType));
			_queueSize  = 0;
			_frontIndex = 0;
		}

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		// @brief : 現在の容量
		uint64 _capacity = 0;

		// 現在の要素数
		uint64 _queueSize = 0;

		// 初期Index (これは循環バッファを使用し, pushFrontによって初期値がずれるためである)
		uint64 _frontIndex = 0;

		// @brief : 要素を格納する配列
		ElementType* _data = nullptr;
	};

	/*-------------------------------------------------------------------
	-           @brief : キューの先頭に要素を追加します.
	---------------------------------------------------------------------*/
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

	/*-------------------------------------------------------------------
	-           @brief : キューの後ろに要素を追加します.
	---------------------------------------------------------------------*/
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

		_frontIndex = (_frontIndex + 1) % _capacity;
		--_queueSize;
		return true;
	}

	template<class ElementType>
	bool Deque<ElementType>::PopBack()
	{
		if (_queueSize == 0) { return false; }

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
		-           もともと配列が存在していればメモリをコピーしたうえで削除
		---------------------------------------------------------------------*/
		auto newData = new ElementType[capacity];
		
		if (_data != nullptr && _capacity > 0)
		{
			Memory::Copy(newData, _data, _capacity * sizeof(ElementType));
			delete[](_data);
		}

		/*-------------------------------------------------------------------
		-           Dataの更新
		---------------------------------------------------------------------*/
		_data     = newData;
		_capacity = capacity;
	}
}

#endif