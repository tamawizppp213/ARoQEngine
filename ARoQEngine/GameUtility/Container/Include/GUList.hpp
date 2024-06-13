//////////////////////////////////////////////////////////////////////////////////
///             @file   GUList.hpp
///             @brief  リスト関連です. UE5を参考にしています. https://dawnarc.com/2020/02/ue4c-container-notes/
///             @author toide
///             @date   2023/11/03 9:25:37
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_LIST_HPP
#define GU_LIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  		LinkedListIteratorBase
	****************************************************************************/
	/* @class     LinkedListIteratorBase
	*  @brief     イテレーターです
	*****************************************************************************/
	template<class NodeType, class ElementType>
	class LinkedListIterator
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		NodeType* GetNode() const { return _currentNode; }

		inline explicit operator bool() const { return _currentNode = nullptr; }
		bool operator==(const LinkedListIterator& rhs) const { return _currentNode == rhs._currentNode; }
		bool operator!=(const LinkedListIterator& rhs) const { return _currentNode != rhs._currentNode; }

		// iterator
		LinkedListIterator& operator++()
		{
			_currentNode = _currentNode->Next();
			return *this;
		}
		LinkedListIterator operator++(int)
		{
			auto temp = *this;
			++(*this);
			return temp;
		}
		LinkedListIterator& operator--()
		{
			_currentNode = _currentNode->Previous();
			return *this;
		}
		LinkedListIterator operator--(int)
		{
			auto temp = *this;
			--(*this);
			return temp;
		}

		// acccessor
		ElementType& operator->() const { return _currentNode->Value(); }
		ElementType& operator*() const { return _currentNode->Value(); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		explicit LinkedListIterator(NodeType* startingNode) : _currentNode(startingNode) {};

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		NodeType* _currentNode = nullptr;
	};

	/****************************************************************************
	*				  			LinkedListNode
	****************************************************************************/
	/* @class     LinkedListNode
	*  @brief     双方向リスト構造を作成するためのノードです.
	*****************************************************************************/
	template<class ElementType>
	class LinkedList
	{
	public:
		/****************************************************************************
		*				  			LinkedListNode
		****************************************************************************/
		/* @class     LinkedListNode
		/* @class     LinkedListNode
		*  @brief     双方向リスト構造を作成するためのノードです.
		*****************************************************************************/
		class LinkedListNode
		{
		public:
			friend class LinkedList;
			/****************************************************************************
			**                Public Function
			*****************************************************************************/

			/****************************************************************************
			**                Public Property
			*****************************************************************************/
			inline       LinkedListNode* Next() { return _next; }
			inline const LinkedListNode* Next() const { return _next; }

			inline       LinkedListNode* Previous() { return _previous; }
			inline const LinkedListNode* Previous() const { return _previous; }

			inline       ElementType& Value() { return _element; }
			inline const ElementType& Value() const { return _element; }

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			LinkedListNode(const ElementType& value) : _element(value), _next(nullptr), _previous(nullptr) {};

		protected:
			/****************************************************************************
			**                Protected Function
			*****************************************************************************/

			/****************************************************************************
			**                Protected Property
			*****************************************************************************/
			ElementType _element;

			LinkedListNode* _previous = nullptr;

			LinkedListNode* _next = nullptr;
		};

		using Iterator      = LinkedListIterator<LinkedListNode, ElementType>;
		using ConstIterator = LinkedListIterator<LinkedListNode, const ElementType>;
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*-------------------------------------------------------------------
		-           @brief : add the new node before the head node
		---------------------------------------------------------------------*/
		       bool PushFront(LinkedListNode* newNode);
		inline bool PushFront(const ElementType& element) { return PushFront(new LinkedListNode(element)); }
		
		/*-------------------------------------------------------------------
		-           @brief : add the new node after the tail node
		---------------------------------------------------------------------*/
		       bool PushBack(LinkedListNode* newNode);
		inline bool PushBack(const ElementType& element) { return PushBack(new LinkedListNode(element)); }
		
		/*-------------------------------------------------------------------
		-           @brief : Insert the specified value into the list at an arbitary point
		---------------------------------------------------------------------*/
		       bool InsertBefore(LinkedListNode* newNode, LinkedListNode* nextNode);
		inline bool InsertBefore(const ElementType& element, LinkedListNode* nextNode){return InsertBefore(new LinkedListNode(element), nextNode); };
			   bool InsertAfter(LinkedListNode* newNode, LinkedListNode* previousNode);
		inline bool InsertAfter(const ElementType& element, LinkedListNode* previousNode) { return InsertAfter(new LinkedListNode(element), previousNode); };
		
		/*-------------------------------------------------------------------
		-           @brief : Remove the node specified
		---------------------------------------------------------------------*/
		void Remove(LinkedListNode* removeNode, bool useDeleteNode = true);
		void Remove(const ElementType& element) { LinkedListNode* node = Find(element); Remove(node); }

		/*-------------------------------------------------------------------
		-           @brief : Finds the node corresponding to the value specified
		---------------------------------------------------------------------*/
		LinkedListNode* Find(const ElementType& element)
		{
			LinkedListNode* node = _head;

			while (node)
			{
				if (node->Value() == element) { break; }

				node = node->_next;
			}

			return node; // 見つからない場合はnullptrが返される.
		}

		/*-------------------------------------------------------------------
		-           @brief : true -> containes the element in the list
		---------------------------------------------------------------------*/
		bool Contains(const ElementType& element) { return (FindNode(element) != nullptr); };

		/*-------------------------------------------------------------------
		-           @brief : Clear the all nodes
		---------------------------------------------------------------------*/
		void Clear();

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*-------------------------------------------------------------------
		-           @brief : Return the item count in the list.
		---------------------------------------------------------------------*/
		inline size_t Size() const { return _listSize; }

		/*-------------------------------------------------------------------
		-           @brief : List size is zero
		---------------------------------------------------------------------*/
		inline bool IsEmpty() const { return _listSize == 0; }

		/*-------------------------------------------------------------------
		-           @brief : Return the node at the head of the list
		---------------------------------------------------------------------*/
		inline LinkedListNode* Head() const { return _head; }

		/*-------------------------------------------------------------------
		-           @brief : Return the node at the tail of the list
		---------------------------------------------------------------------*/
		inline LinkedListNode* Tail() const { return _tail; }

		/*-------------------------------------------------------------------
		-           @brief : Return the iterator
		---------------------------------------------------------------------*/
		Iterator      Begin       (const bool useReverse = false) { return Iterator(useReverse ? nullptr : Head()); }
		ConstIterator BeginAsConst(const bool useReverse = false) { return ConstIterator(useReverse ? nullptr : Head()); }
		Iterator      End(const bool useReverse = false)        { return Iterator(useReverse ? Tail() : nullptr); }
		ConstIterator EndAsConst(const bool useReverse = false) { return ConstIterator(useReverse ? Tail() : nullptr); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		LinkedList() : _head(nullptr), _tail(nullptr), _listSize(0) {};

		virtual ~LinkedList() { Clear(); };

	private:

		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		LinkedListNode* _head = nullptr;

		LinkedListNode* _tail = nullptr;

		size_t _listSize = 0;

	};

	/*-------------------------------------------------------------------
	-           @brief : 先頭にNodeをくっつけます
	---------------------------------------------------------------------*/
	template<class ElementType>
	bool LinkedList<ElementType>::PushFront(LinkedListNode* newNode)
	{
		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-             headNodeが既にあるならポインタを変更する
		---------------------------------------------------------------------*/
		if (_head)
		{
			newNode->_next   = _head;   // 新しいノードの次としてもともとのHeadを入れる
			_head->_previous = newNode; // もともとのHeadの前には新しいノードを加える
			_head = newNode;            // Headには新しいノードを加える
		}
		else
		{
			_head = _tail = newNode;
		}

		_listSize++;
		return true;
	}

	/*-------------------------------------------------------------------
	-           @brief : 後ろにNodeをくっつけます
	---------------------------------------------------------------------*/
	template<class ElementType>
	bool LinkedList<ElementType>::PushBack(LinkedListNode* newNode)
	{
		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-             tailNodeが既にあるならポインタを変更する
		---------------------------------------------------------------------*/
		if (_tail)
		{
			_tail->_next       = newNode; // 終端の次には新しいノードが来る
			newNode->_previous = _tail;   // 新しいノードの前にはもともとの終端
			_tail = newNode;              // ノードの更新
		}
		else
		{
			_head = _tail = newNode;
		}

		_listSize++;
		return true;
	}

	template<class ElementType>
	bool LinkedList<ElementType>::InsertBefore(LinkedListNode* newNode, LinkedListNode* nextNode)
	{
		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-             先頭時の処理
		---------------------------------------------------------------------*/
		if (nextNode == nullptr || nextNode == _head)
		{
			return PushFront(newNode);
		}

		/*-------------------------------------------------------------------
		-             挿入処理
		---------------------------------------------------------------------*/
		// new node側の接続を切り替える
		newNode->_previous = nextNode->_previousNode;
		newNode->_next     = nextNode;

		// もともとnextの前にくっついていたリストの接続を切り替える
		nextNode->_previous->_next = newNode;

		// nextのリストの接続を切り替える
		nextNode->_previous        = newNode;

		_listSize++;
		return true;
	}

	template<class ElementType>
	bool LinkedList<ElementType>::InsertAfter(LinkedListNode* newNode, LinkedListNode* previousNode)
	{
		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-             終端時の処理
		---------------------------------------------------------------------*/
		if (previousNode == nullptr || previousNode == _tail)
		{
			return PushBack(newNode);
		}

		/*-------------------------------------------------------------------
		-             挿入処理
		---------------------------------------------------------------------*/
		// newNode側の接続を切り替える
		newNode->_next     = previousNode->_next;
		newNode->_previous = previousNode;

		// もともとpreviousの後にくっついていたリストの接続を切り替える
		previousNode->_next->_previous = newNode;

		// previousのリストの接続を切り替える
		previousNode->_next = newNode;

		_listSize++;
		return true;
	}

	/*-------------------------------------------------------------------
	-             要素を取り除く
	---------------------------------------------------------------------*/
	template<class ElementType>
	void LinkedList<ElementType>::Remove(LinkedListNode* removeNode, bool useDeleteNode)
	{
		if (removeNode == nullptr) { return; }

		/*-------------------------------------------------------------------
		-             自分自身しか持ち合わせない場合の対応
		---------------------------------------------------------------------*/
		if (Size() == 1)
		{
			if (removeNode != _head) { return; }

			if (useDeleteNode) { Clear(); }
			else
			{
				// 切り離し
				removeNode->_next = removeNode->_previous = nullptr;
				_head = _tail = nullptr;
				_listSize = 0;
			}
		}

		/*-------------------------------------------------------------------
		-             先端ノードだった時の対応
		---------------------------------------------------------------------*/
		if (removeNode == _head)
		{
			_head = _head->_next;
			_head->_previous = nullptr;
		}
		/*-------------------------------------------------------------------
		-             終端ノードだった時の対応
		---------------------------------------------------------------------*/
		else if (removeNode == _tail)
		{
			_tail = _tail->_previous;
			_tail->_next = nullptr;
		}
		/*-------------------------------------------------------------------
		-             通常ノードだった時の対応
		---------------------------------------------------------------------*/
		else
		{
			removeNode->_next->_previous = removeNode->_previous;
			removeNode->_previous->_next = removeNode->_next;
		}
		/*-------------------------------------------------------------------
		-             削除する時は自身をしっかり消す, 削除しない場合はnodeを切り離す対応
		---------------------------------------------------------------------*/
		if (useDeleteNode)
		{
			delete removeNode;
		}
		else
		{
			removeNode->_next = removeNode->_previous = nullptr;
		}
		_listSize--;
	}

	/*-------------------------------------------------------------------
	-           @brief : 中身を全て消去します
	---------------------------------------------------------------------*/
	template<class ElementType>
	void LinkedList<ElementType>::Clear()
	{
		LinkedListNode* node = nullptr;

		// 先頭からさかのぼって一つずつ消去する
		while (_head)
		{
			node = _head->_next; // delete前に次のノードを読み込んでおく
			delete _head;
			_head = node;         // 次のノードにHeadを移行する
		}

		// 終了処理
		_head = nullptr;
		_tail = nullptr;
		_listSize = 0;
	}
}
#endif