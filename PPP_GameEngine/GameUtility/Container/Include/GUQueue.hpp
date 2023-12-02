//////////////////////////////////////////////////////////////////////////////////
///             @file   GUQueue.hpp
///             @brief  �ʏ�̃L���[�ł� (����X���b�h�Z�[�t�ł͂���܂���̂ŁA���ӂ��Ă�������)
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
	*  @brief     �L���[
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
		Node* _head = nullptr; // �����ꏊ�̐擪�m�[�h
		Node* _tail = nullptr; // �����o���ꏊ�̐擪�m�[�h
		int _queueSize = 0;
	};

	/*-------------------------------------------------------------------
	-           @brief : �L���[�̐擪�ɗv�f��ǉ����܂�.
	---------------------------------------------------------------------*/
	template<class ElementType, QueueMode Mode>
	bool Queue<ElementType, Mode>::Push(const ElementType& element)
	{
		Node* newNode = new Node(element);

		if (newNode == nullptr) { return false; }

		/*-------------------------------------------------------------------
		-           Push�����̎���
		---------------------------------------------------------------------*/
		Node* oldHead = nullptr;
		if (Mode == QueueMode::MPSC)
		{
			// �m�[�h������
			oldHead = _head;
			_head = newNode;
			oldHead->Next = newNode;
		}
		else
		{
			// �m�[�h������
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
		-           Push�����̎���
		---------------------------------------------------------------------*/
		Node* oldHead = nullptr;
		if (Mode == QueueMode::MPSC)
		{
			// �m�[�h������
			oldHead = _head;
			_head = newNode;
			oldHead->Next = newNode;
		}
		else
		{
			// �m�[�h������
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
	-           @brief : �L���[�̏I�[�̗v�f���폜���܂�
	---------------------------------------------------------------------*/
	template<class ElementType, QueueMode Mode>
	bool Queue<ElementType, Mode>::Pop()
	{
		if (_tail == nullptr) { return false; }
		
		/*-------------------------------------------------------------------
		-           Pop�����̎���
		---------------------------------------------------------------------*/
		// �폜����m�[�h�̕ێ�
		Node* oldTail = _tail;

		// �폜��ɏo�Ă���m�[�h�̎��o��
		Node* popNode = _tail->Next;

		// ���̃m�[�h����������
		_tail = popNode; 

		/*-------------------------------------------------------------------
		-           �v�f�̍폜
		---------------------------------------------------------------------*/
		delete oldTail;
		_queueSize--;
		return true;
	}
}
#endif