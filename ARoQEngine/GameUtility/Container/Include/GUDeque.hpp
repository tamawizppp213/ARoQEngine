//////////////////////////////////////////////////////////////////////////////////
/// @file   GUDeque.hpp
/// @brief  �z�o�b�t�@�ɂ���d���[�L���[. Push�Ő擪�▖���ɗv�f��ǉ���, Pop�Ő擪�▖������v�f�����o�����Ƃ��ł��܂�. @n
///         �܂�, ���̃N���X��[]�ɂ�郉���_���A�N�Z�X���\�ł�. @n
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
	/*!  @brief     �z�o�b�t�@�ɂ���d���[�L���[. Push�Ő擪�▖���ɗv�f��ǉ���, Pop�Ő擪�▖������v�f�����o�����Ƃ��ł��܂�. @n
	*               �܂�, ���̃N���X��[]�ɂ�郉���_���A�N�Z�X���\�ł�. @n
	*****************************************************************************/
	template<class ElementType>
	class Deque
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �L���[�̐擪�ɗv�f��ǉ����܂�.
		*  @param[in] const ElementType& �i�[����f�[�^. (������ɂ�=���g�����R�s�[�������܂�)
		*  @return    void
		*************************************************************************/
		void PushFront(const ElementType& element);

		/*!**********************************************************************
		*  @brief     �L���[�̖����ɗv�f��ǉ����܂�.
		*  @param[in] const ElementType& �i�[����f�[�^ (������ɂ�=���g�����R�s�[�������܂�)
		*  @return    void
		*************************************************************************/
		void PushBack(const ElementType& element);

		/*!**********************************************************************
		*  @brief     �L���[�̐擪����v�f���������܂�. �f�X�g���N�^���Ă΂�܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		bool PopFront();

		/*!**********************************************************************
		*  @brief     �L���[�̖�������v�f���������܂�. �f�X�g���N�^���Ă΂�܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		bool PopBack();

		/*!**********************************************************************
		*  @brief     �L���[�̃����������O�Ɋm�ۂ��܂�. �ŏ��ɗe�ʂ��`���邱�Ƃ�, �������̍Ċm�ۂ�h���܂�
		*  @param[in] const uint64 capacity �m�ۂ���L���[�̗e�� (��)
		*  @return    void
		*************************************************************************/
		void Reserve(const uint64 capacity);

		/*!**********************************************************************
		*  @brief     �T�C�Y��0�ɂ��܂���, Capacity�̈�͂��̂܂܂ɂ��܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Clear();
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �L���[�T�C�Y��0���ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool �L���[�T�C�Y��0�Ȃ�true, ����ȊO��false
		*************************************************************************/
		__forceinline bool IsEmpty() const { return _queueSize == 0; }

		/*!**********************************************************************
		*  @brief     �L���[�̐擪�v�f��Ԃ��܂�
		*  @param[in] void
		*  @return    ElementType& �L���[�̐擪�̗v�f
		*************************************************************************/
		__forceinline       ElementType& Front()       { return _data[_frontIndex]; }

		/*!**********************************************************************
		*  @brief     �L���[�̐擪�v�f��const�Q�ƂŕԂ��܂�
		*  @param[in] void
		*  @return    const ElementType& �L���[�̐擪�̗v�f
		*************************************************************************/
		__forceinline const ElementType& Front() const { return _data[_frontIndex]; }

		/*!**********************************************************************
		*  @brief     �L���[�̖����v�f��Ԃ��܂�
		*  @param[in] void
		*  @return    ElementType& �L���[�̖����̗v�f
		*************************************************************************/
		__forceinline       ElementType& Back()       { return _data[(_frontIndex + _capacity + _queueSize - 1) % _capacity]; }

		/*!**********************************************************************
		*  @brief     �L���[�̖����v�f��const�Q�ƂŕԂ��܂�
		*  @param[in] void
		*  @return    const ElementType& �L���[�̖����̗v�f
		*************************************************************************/
		__forceinline const ElementType& Back() const { return _data[(_frontIndex + _capacity + _queueSize - 1) % _capacity]; }

		/*!**********************************************************************
		*  @brief     �L���[�̐擪�|�C���^��Ԃ��܂�
		*  @param[in] void
		*  @return    ElementType* �L���[�̐擪�|�C���^
		*************************************************************************/
		__forceinline       ElementType* Data()       { return &_data[_frontIndex]; }

		/*!**********************************************************************
		*  @brief     �L���[�̐擪�|�C���^��const�ŕԂ��܂�
		*  @param[in] void
		*  @return    const ElementType* �L���[�̐擪�|�C���^
		*************************************************************************/
		__forceinline const ElementType* Data() const { return &_data[_frontIndex]; }

		/*!**********************************************************************
		*  @brief     ���݊i�[����Ă���L���[�̃T�C�Y���擾���܂�. (Reserve�Ŏ��O�Ɋm�ۂ���Ă���v�f���ł͂���܂���)
		*  @param[in] void
		*  @return    uint64 �L���[�̃T�C�Y
		*************************************************************************/
		__forceinline uint64 Size() const { return _queueSize; }

		/*!**********************************************************************
		*  @brief     ���݊i�[������ő�̃L���[ = Reserve�Ŋm�ۂ���Ă�������擾���܂�
		*  @param[in] void
		*  @return    uint64 Reserve�Ŋm�ۂ���Ă����
		*************************************************************************/
		__forceinline uint64 Capacity() const { return _capacity; }

		/*!**********************************************************************
		*  @brief     ���݊i�[�\�Ȏc��̃L���[�T�C�Y���擾���܂�
		*  @param[in] void
		*  @return    uint64 ���݊i�[�\�Ȏc��̃L���[�T�C�Y
		*************************************************************************/
		__forceinline uint64 Residue() const { return _capacity - _queueSize; }
		#pragma endregion

		#pragma region Public Operator
		/*! @brief �����_���A�N�Z�X�p*/
		__forceinline       ElementType& operator[](const uint64 index)       noexcept { return _data[(_frontIndex + _capacity + index) % _capacity]; }
		
		/*! @brief �����_���A�N�Z�X�p*/
		__forceinline const ElementType& operator[](const uint64 index) const noexcept { return _data[(_frontIndex + _capacity + index) % _capacity]; }
		
		/*! @brief �R�s�[*/
		Deque<ElementType>& operator=(const Deque<ElementType>& right) 
		{ 
			CopyFrom(right); 
			return *this; 
		}

		/*! @brief ���[�u*/
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
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		Deque() = default;

		/*!**********************************************************************
		*  @brief     �i�[�\�ȗv�f�����w�肵�ď��������܂�
		*  @param[in] const uint64 capacity �m�ۂ���L���[�̗e�� (��)
		*************************************************************************/
		explicit Deque(const uint64 capacity) { Reserve(capacity); }

		/*!**********************************************************************
		*  @brief     �ʂ�Deque���R�s�[���ď��������܂�
		*  @param[in] const Deque<ElementType>& �L���[�̃R�s�[��
		*************************************************************************/
		Deque(const Deque<ElementType>& deque) { CopyFrom(deque); }

		/*!**********************************************************************
		*  @brief     {, , ...}�ŏ��������܂�
		*  @param[in] �������q���X�g
		*************************************************************************/
		Deque(const std::initializer_list<ElementType> list)
		{
			if (_data) { Memory::Free(_data); }

			const auto listSize     = list.size();
			const auto capacitySize = listSize * 2;

			_data = (ElementType*)Memory::Allocate(capacitySize * sizeof(ElementType)); // capacity�p
			Memory::ForceExecuteCopyConstructors(_data, list.begin(), list.size());
			
			_capacity   = capacitySize;
			_queueSize  = listSize;
			_frontIndex = 0;
		}

		/*!**********************************************************************
		*  @brief     �ʂ�Deque�����[�u���ď��������܂�
		*  @param[in] Deque<ElementType>&& �L���[�̃��[�u��
		*************************************************************************/
		Deque(Deque<ElementType>&& deque) noexcept 
		{
			if (_data) { Memory::Free(_data); }
			
			Memory::ForceExecuteMoveConstructors(_data, deque, deque.Size());
			deque.Initialize();
		}

		/*! @brief �f�X�g���N�^*/
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
		*  @brief     �ʂ�Deque���R�s�[���܂�
		*  @param[in] const Deque<ElementType>& �L���[�̃R�s�[��
		*************************************************************************/
		void CopyFrom(const Deque<ElementType>& deque);

		/*!**********************************************************************
		*  @brief     0������
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
		/*! @brief : ���݂̗e��*/
		uint64 _capacity = 0;

		/*! ���݂̗v�f�� */
		uint64 _queueSize = 0;

		/* ����Index(����͏z�o�b�t�@���g�p��, pushFront�ɂ���ď����l������邽�߂ł���)*/
		uint64 _frontIndex = 0;

		/* @brief �v�f���i�[����z��*/
		ElementType* _data = nullptr;
		#pragma endregion
	};

	/*-------------------------------------------------------------------
	-           @brief : ���������R�s�[���܂�
	---------------------------------------------------------------------*/
	template<class ElementType>
	void Deque<ElementType>::CopyFrom(const Deque<ElementType>& deque)
	{
		/*-------------------------------------------------------------------
		-           �����������ɑ��݂��Ă����烁������j������
		---------------------------------------------------------------------*/
		if (Size())
		{
			Clear();
			Memory::Free(_data);
		}

		/*-------------------------------------------------------------------
		-           �������̈���m��
		---------------------------------------------------------------------*/
		_data = Memory::Allocate(deque.Capacity() * sizeof(ElementType));

		/*-------------------------------------------------------------------
		-           CopyConsturctor���Ăяo��
		---------------------------------------------------------------------*/
		Memory::ForceExecuteCopyConstructors(_data, deque.Data(), deque.Size());

		/*-------------------------------------------------------------------
		-           �ݒ�
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
			// �z�o�b�t�@���̗p���Ă��邽��, Back��Front�����O�̃|�C���^�ɂȂ��Ă���ꍇ������
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
		-           ���Ƃ��Ƃ�菬���������珈�������Ȃ�
		---------------------------------------------------------------------*/
		if (capacity <= _capacity) { return; }

		/*-------------------------------------------------------------------
		-           ���Ƃ��Ɣz�񂪑��݂��Ă���ΑS�Ẵ��������R�s�[���������ō폜
		---------------------------------------------------------------------*/
		auto newData = Memory::Allocate(capacity);
		
		if (_data != nullptr && _capacity > 0)
		{
			Memory::Copy(newData, _data, _capacity * sizeof(ElementType));
			Memory::Free(_data);
		}

		/*-------------------------------------------------------------------
		-           Data�̍X�V
		---------------------------------------------------------------------*/
		_data     = newData;
		_capacity = capacity;
	}
}

#endif