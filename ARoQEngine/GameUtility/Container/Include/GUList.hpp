//////////////////////////////////////////////////////////////////////////////////
///             @file   GUList.hpp
///             @brief  ���X�g�֘A�ł�. UE5���Q�l�ɂ��Ă��܂�. https://dawnarc.com/2020/02/ue4c-container-notes/
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
	*  @brief     �C�e���[�^�[�ł�
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
	*  @brief     �o�������X�g�\�����쐬���邽�߂̃m�[�h�ł�.
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
		*  @brief     �o�������X�g�\�����쐬���邽�߂̃m�[�h�ł�.
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

			return node; // ������Ȃ��ꍇ��nullptr���Ԃ����.
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
	-           @brief : �擪��Node���������܂�
	---------------------------------------------------------------------*/
	template<class ElementType>
	bool LinkedList<ElementType>::PushFront(LinkedListNode* newNode)
	{
		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-             headNode�����ɂ���Ȃ�|�C���^��ύX����
		---------------------------------------------------------------------*/
		if (_head)
		{
			newNode->_next   = _head;   // �V�����m�[�h�̎��Ƃ��Ă��Ƃ��Ƃ�Head������
			_head->_previous = newNode; // ���Ƃ��Ƃ�Head�̑O�ɂ͐V�����m�[�h��������
			_head = newNode;            // Head�ɂ͐V�����m�[�h��������
		}
		else
		{
			_head = _tail = newNode;
		}

		_listSize++;
		return true;
	}

	/*-------------------------------------------------------------------
	-           @brief : ����Node���������܂�
	---------------------------------------------------------------------*/
	template<class ElementType>
	bool LinkedList<ElementType>::PushBack(LinkedListNode* newNode)
	{
		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-             tailNode�����ɂ���Ȃ�|�C���^��ύX����
		---------------------------------------------------------------------*/
		if (_tail)
		{
			_tail->_next       = newNode; // �I�[�̎��ɂ͐V�����m�[�h������
			newNode->_previous = _tail;   // �V�����m�[�h�̑O�ɂ͂��Ƃ��Ƃ̏I�[
			_tail = newNode;              // �m�[�h�̍X�V
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
		-             �擪���̏���
		---------------------------------------------------------------------*/
		if (nextNode == nullptr || nextNode == _head)
		{
			return PushFront(newNode);
		}

		/*-------------------------------------------------------------------
		-             �}������
		---------------------------------------------------------------------*/
		// new node���̐ڑ���؂�ւ���
		newNode->_previous = nextNode->_previousNode;
		newNode->_next     = nextNode;

		// ���Ƃ���next�̑O�ɂ������Ă������X�g�̐ڑ���؂�ւ���
		nextNode->_previous->_next = newNode;

		// next�̃��X�g�̐ڑ���؂�ւ���
		nextNode->_previous        = newNode;

		_listSize++;
		return true;
	}

	template<class ElementType>
	bool LinkedList<ElementType>::InsertAfter(LinkedListNode* newNode, LinkedListNode* previousNode)
	{
		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-             �I�[���̏���
		---------------------------------------------------------------------*/
		if (previousNode == nullptr || previousNode == _tail)
		{
			return PushBack(newNode);
		}

		/*-------------------------------------------------------------------
		-             �}������
		---------------------------------------------------------------------*/
		// newNode���̐ڑ���؂�ւ���
		newNode->_next     = previousNode->_next;
		newNode->_previous = previousNode;

		// ���Ƃ���previous�̌�ɂ������Ă������X�g�̐ڑ���؂�ւ���
		previousNode->_next->_previous = newNode;

		// previous�̃��X�g�̐ڑ���؂�ւ���
		previousNode->_next = newNode;

		_listSize++;
		return true;
	}

	/*-------------------------------------------------------------------
	-             �v�f����菜��
	---------------------------------------------------------------------*/
	template<class ElementType>
	void LinkedList<ElementType>::Remove(LinkedListNode* removeNode, bool useDeleteNode)
	{
		if (removeNode == nullptr) { return; }

		/*-------------------------------------------------------------------
		-             �������g�����������킹�Ȃ��ꍇ�̑Ή�
		---------------------------------------------------------------------*/
		if (Size() == 1)
		{
			if (removeNode != _head) { return; }

			if (useDeleteNode) { Clear(); }
			else
			{
				// �؂藣��
				removeNode->_next = removeNode->_previous = nullptr;
				_head = _tail = nullptr;
				_listSize = 0;
			}
		}

		/*-------------------------------------------------------------------
		-             ��[�m�[�h���������̑Ή�
		---------------------------------------------------------------------*/
		if (removeNode == _head)
		{
			_head = _head->_next;
			_head->_previous = nullptr;
		}
		/*-------------------------------------------------------------------
		-             �I�[�m�[�h���������̑Ή�
		---------------------------------------------------------------------*/
		else if (removeNode == _tail)
		{
			_tail = _tail->_previous;
			_tail->_next = nullptr;
		}
		/*-------------------------------------------------------------------
		-             �ʏ�m�[�h���������̑Ή�
		---------------------------------------------------------------------*/
		else
		{
			removeNode->_next->_previous = removeNode->_previous;
			removeNode->_previous->_next = removeNode->_next;
		}
		/*-------------------------------------------------------------------
		-             �폜���鎞�͎��g�������������, �폜���Ȃ��ꍇ��node��؂藣���Ή�
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
	-           @brief : ���g��S�ď������܂�
	---------------------------------------------------------------------*/
	template<class ElementType>
	void LinkedList<ElementType>::Clear()
	{
		LinkedListNode* node = nullptr;

		// �擪���炳���̂ڂ��Ĉ����������
		while (_head)
		{
			node = _head->_next; // delete�O�Ɏ��̃m�[�h��ǂݍ���ł���
			delete _head;
			_head = node;         // ���̃m�[�h��Head���ڍs����
		}

		// �I������
		_head = nullptr;
		_tail = nullptr;
		_listSize = 0;
	}
}
#endif