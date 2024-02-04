//////////////////////////////////////////////////////////////////////////////////
///             @file   GUDeque.hpp
///             @brief  ��d���[�L���[
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
	*  @brief     �z�o�b�t�@�ɂ���d���[�L���[
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
		-           @brief : �T�C�Y��0�ɂ��܂�.
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
		-           @brief : ���݊i�[����Ă���L���[�̃T�C�Y���擾���܂�
		---------------------------------------------------------------------*/
		__forceinline uint64 Size() const { return _queueSize; }

		/*-------------------------------------------------------------------
		-           @brief : ���݊i�[������ő�̃L���[���擾���܂�
		---------------------------------------------------------------------*/
		__forceinline uint64 Capacity() const { return _capacity; }

		/*-------------------------------------------------------------------
		-           @brief : ���݊i�[�\�Ȏc��̃L���[�T�C�Y���擾���܂�
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
			_data = new ElementType[list.size() * 2]; // capacity�p
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
		// @brief : ���݂̗e��
		uint64 _capacity = 0;

		// ���݂̗v�f��
		uint64 _queueSize = 0;

		// ����Index (����͏z�o�b�t�@���g�p��, pushFront�ɂ���ď����l������邽�߂ł���)
		uint64 _frontIndex = 0;

		// @brief : �v�f���i�[����z��
		ElementType* _data = nullptr;
	};

	/*-------------------------------------------------------------------
	-           @brief : �L���[�̐擪�ɗv�f��ǉ����܂�.
	---------------------------------------------------------------------*/
	template<class ElementType>
	void Deque<ElementType>::PushFront(const ElementType& element)
	{
		// �z��𒴂����烁�������Ċm�ۂ���
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
	-           @brief : �L���[�̌��ɗv�f��ǉ����܂�.
	---------------------------------------------------------------------*/
	template<class ElementType>
	void Deque<ElementType>::PushBack(const ElementType& element)
	{
		// �z��𒴂��Ă����烁�������Ċm�ۂ���
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
		-           ���Ƃ��Ƃ�菬���������珈�������Ȃ�
		---------------------------------------------------------------------*/
		if (capacity <= _capacity) { return; }

		/*-------------------------------------------------------------------
		-           ���Ƃ��Ɣz�񂪑��݂��Ă���΃��������R�s�[���������ō폜
		---------------------------------------------------------------------*/
		auto newData = new ElementType[capacity];
		
		if (_data != nullptr && _capacity > 0)
		{
			Memory::Copy(newData, _data, _capacity * sizeof(ElementType));
			delete[](_data);
		}

		/*-------------------------------------------------------------------
		-           Data�̍X�V
		---------------------------------------------------------------------*/
		_data     = newData;
		_capacity = capacity;
	}
}

#endif