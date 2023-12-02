//////////////////////////////////////////////////////////////////////////////////
///             @file   GUQueue.hpp
///             @brief  通常のキューです (現状スレッドセーフではありませんので、注意してください)
///             @author toide
///             @date   2023/11/05 15:07:26
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_QUEUE_HPP
#define GU_QUEUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Base/Include/ClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	enum class QueueMode
	{
		MPSC, //Multiple-producers, single-consumer queue.
		SPSC, //Single-producers, single-consumer queue.
	};

	/****************************************************************************
	*				  			   Queue
	*************************************************************************//**
	*  @class     Queue
	*  @brief     キュー
	*****************************************************************************/
	template<class ElementType, QueueMode Mode = QueueMode::SPSC>
	class Queue : public NonCopyable
	{
	private:
		struct Node
		{
			Node*       Next;
			ElementType Element;

			Node() : Next(nullptr) {};
			explicit Node(const ElementType& element) : Next(nullptr), Element(element) {};
			explicit Node(const ElementType&& element) : Next(nullptr), Element(element) {};
		};
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*-------------------------------------------------------------------
		-           @brief : Add the element to the queue head.
		---------------------------------------------------------------------*/
		bool Push(const ElementType& element);
		bool Push(const ElementType&& element);

		/*-------------------------------------------------------------------
		-           @brief : Remove the element to the queue tail.
		---------------------------------------------------------------------*/
		bool Pop();
		
		/*-------------------------------------------------------------------
		-           @brief : Clear the all queue
		---------------------------------------------------------------------*/
		inline void Clear() { while (Pop()); }

		/*-------------------------------------------------------------------
		-           @brief : return the queue front (=_tail) element
		---------------------------------------------------------------------*/
		inline        ElementType& Front()       { return _tail->Element; }
		inline const  ElementType& Front() const { return _tail->Element; }

		/*-------------------------------------------------------------------
		-           @brief : return the queue back (=_head) element
		---------------------------------------------------------------------*/
		inline       ElementType& Back()       { return _head->Element; }
		inline const ElementType& Back() const { return _head->Element; }

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		inline bool IsEmpty() const { return _queueSize <= 0; }

		/*-------------------------------------------------------------------
		-           @brief : return the queue size
		---------------------------------------------------------------------*/
		inline int Size() { return _queueSize; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Queue() : _head(nullptr), _tail(nullptr) {};

		~Queue()
		{
			Clear();
		}
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		Node* _head = nullptr; // 入れる場所の先頭ノード
		Node* _tail = nullptr; // 押し出す場所の先頭ノード
		int _queueSize = 0;
	};

	/*-------------------------------------------------------------------
	-           @brief : キューの先頭に要素を追加します.
	---------------------------------------------------------------------*/
	template<class ElementType, QueueMode Mode>
	bool Queue<ElementType, Mode>::Push(const ElementType& element)
	{
		Node* newNode = new Node(element);

		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-           Push処理の実装
		---------------------------------------------------------------------*/
		Node* oldHead = nullptr;
		if (Mode == QueueMode::MPSC)
		{
			// ノードをつける
			oldHead = _head;
			_head = newNode;
			oldHead->Next = newNode;
		}
		else
		{
			// ノードをつける
			oldHead = _head;
			_head   = newNode;

			if (oldHead)
			{
				oldHead->Next = newNode;
			}
			else
			{
				_tail = _head;
			}
		}

		_queueSize++;
	}

	template<class ElementType, QueueMode Mode>
	bool Queue<ElementType, Mode>::Push(const ElementType&& element)
	{
		Node* newNode = new Node(element);

		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-           Push処理の実装
		---------------------------------------------------------------------*/
		Node* oldHead = nullptr;
		if (Mode == QueueMode::MPSC)
		{
			// ノードをつける
			oldHead = _head;
			_head = newNode;
			oldHead->Next = newNode;
		}
		else
		{
			// ノードをつける
			oldHead = _head;
			_head = newNode;

			if (oldHead)
			{
				oldHead->Next = newNode;
			}
			else
			{
				_tail = _head;
			}
		}

		_queueSize++;
	}

	/*-------------------------------------------------------------------
	-           @brief : キューの終端の要素を削除します
	---------------------------------------------------------------------*/
	template<class ElementType, QueueMode Mode>
	bool Queue<ElementType, Mode>::Pop()
	{
		if (_tail == nullptr) { return false; }
		
		/*-------------------------------------------------------------------
		-           Pop処理の実装
		---------------------------------------------------------------------*/
		// 削除するノードの保持
		Node* oldTail = _tail;

		// 削除後に出てくるノードの取り出し
		Node* popNode = _tail->Next;

		// 次のノードを交換する
		_tail = popNode; 

		/*-------------------------------------------------------------------
		-           要素の削除
		---------------------------------------------------------------------*/
		delete oldTail;
		_queueSize--;
		return true;
	}
}
#endif