//////////////////////////////////////////////////////////////////////////////////
///             @file   GUArray.hpp
///             @brief  ������
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
	*  @brief     ���I�z��
	* �@�@�@�@�@�@�@�@https://qiita.com/ageprocpp/items/16aa225a1194fa0cf450
	*****************************************************************************/
	template<class ElementType>
	class DynamicArray
	{
	public:
		static constexpr uint64 INDEX_NONE = static_cast<uint64>(-1);

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �T�C�Y�������I�ɕύX���܂�.�����ɉ����Ĉȉ��̋��������܂�. @n
		*             size <= ���X�����Ă���T�C�Y : �����s��Ȃ� @n
		*             size >  ���X�����Ă���T�C�Y : �w��T�C�Y =�@capacity�̗̈���m�ۂ�����ŃR���X�g���N�^�Ăяo���܂ōs��
		*  @param[in] const uint64 �z��̃T�C�Y (capacity�ł͂���܂���)
		*  @param[in] const bool   �R�s�[�R���X�g���N�^���g�p���邩 (false�ɂ���ƌ������̂��߂ɖ���������ԂŎn�܂�܂�.)
		*  @param[in] const ElementType& �R�s�[���̗v�f (��{�f�t�H���g�R���X�g���N�^������ꂽ����.)
		*  @return    void
		*************************************************************************/
		void Resize(const uint64 size, const bool useCopyConstructor = true, const ElementType& defaultElement = ElementType());

		/*!**********************************************************************
		*  @brief     �������̈�����O�Ɋm�ۂ��܂�. (������, �R���X�g���N�^�̌Ăяo���͑S���s���܂���) @n
		*             Resize�Ƃ̈Ⴂ�Ƃ���, Size�̕ύX�͂Ȃ�, Capacity�������ς��܂�.  @n
		*             ���̂���, At�֐��Ȃǂ�p���Ĕz��Ƀ����_���A�N�Z�X���s���ƃG���[�ƂȂ�܂�
		*  @param[in] const uint64 �z��̗v�f�� (�L���p�V�e�B)
		*  @return    void
		*************************************************************************/
		void Reserve(const uint64 capacity);

		/*!**********************************************************************
		*  @brief     �T�C�Y��0�ɂ��܂���, Capacity�̈掩�̂͂��̂܂܂ɂ��܂�. @n
		*             �f�X�g���N�^�͌Ăяo����܂�. 
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Clear();

		/*!**********************************************************************
		*  @brief     Capacity���R���e�i��Size�܂Ő؂�l�߂� @n
		*             capacity��0�̂Ƃ�, capacity�ȏ��size�����鎞�͏������s���܂���.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void ShrinkToFit();

		/*!**********************************************************************
		*  @brief     �z������ɒǉ�����. Capacity�𒴂����ꍇ, �S�̂̃�������2�{�̑傫���ōĊ��蓖�Ă��s��.
		*  @param[in] const ElementType& �v�f
		*  @return    void
		*************************************************************************/
		void Push(const ElementType& element);

		/*!**********************************************************************
		*  @brief     �z������ɒǉ�����. Capacity�𒴂����ꍇ, �S�̂̃�������2�{�̑傫���ōĊ��蓖�Ă��s��.
		*  @param[in] const ElementType&& move�v�f
		*  @return    void
		*************************************************************************/
		void Push(ElementType&& element);

		/*!**********************************************************************
		*  @brief     �z�����납����o��. ���̍�, �f�X�g���N�^���Ăяo��.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Pop();

		/*!**********************************************************************
		*  @brief     �w�肵���v�f�����Ɋ܂܂�Ă���ꍇ��true��Ԃ��܂�.
		*  @param[in] const ElementType& �v�f
		*  @return    bool true�̏ꍇ�w�肵���v�f�����Ɋ܂܂�Ă���
		*************************************************************************/
		bool Contains(const ElementType& element) const;

		/*!**********************************************************************
		*  @brief     �w�肵���z��C���f�b�N�X�����������猩���܂�. ������Ȃ������ꍇ��IndexNone��n���܂�
		*  @param[in] const ElementType& �v�f
		*  @return    uint64 �v�f���������̂��������ꍇ�͂��̃C���f�b�N�X��, ������Ȃ������ꍇ��IndexNone(-1)��Ԃ��܂�
		*************************************************************************/
		uint64 FindFromBegin(const ElementType& element) const;

		/*!**********************************************************************
		*  @brief     �w�肵���z��C���f�b�N�X���t����(�z��v�f�̑傫������)���猩���܂�. ������Ȃ������ꍇ��IndexNone��n���܂�
		*  @param[in] const ElementType& �v�f
		*  @return    uint64 �v�f���������̂��������ꍇ�͂��̃C���f�b�N�X��, ������Ȃ������ꍇ��IndexNone(-1)��Ԃ��܂�
		*************************************************************************/
		uint64 FindFromEnd(const ElementType& element) const;

		/*!**********************************************************************
		*  @brief     �w�肵��Index�ɂ���v�f���폜���܂�
		*  @param[in] const uint64 index
		*  @param[in] const bool ���������k���s����
		*  @return    void
		*************************************************************************/
		void RemoveAt(const uint64 index, const bool allowShrinking = true) { RemoveAtImplement(index, 1, allowShrinking); }

		/*!**********************************************************************
		*  @brief     �w�肵��Index�ɂ���v�f���폜���܂�
		*  @param[in] const uint64 �폜�J�nindex
		*  @param[in] const uint64 �A�����č폜����v�f��
		*  @param[in] const bool ���������k���s����
		*  @return    void
		*************************************************************************/
		void RemoveAt(const uint64 index, const uint64 removeCount, const bool allowShrinking = true) { RemoveAtImplement(index, removeCount, allowShrinking); }

		/*!**********************************************************************
		*  @brief     �w�肵���v�f���폜���܂�
		*  @param[in] const ElementType& �v�f
		*  @param[in] const bool ���������k���s����
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
		*  @brief     �w�肵���v�f��S�č폜���܂�. �Ԃ�l�͍폜�������ł�.
		*  @param[in] const ElementType& �v�f
		*  @param[in] const bool ���������k���s����
		*  @return    gu::uint64 �폜��
		*************************************************************************/
		gu::uint64 RemoveAll(const ElementType& element, const bool allowShrinking = true);
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �q�[�v�̈�Ɋi�[����Ă���擪�̃|�C���^��Ԃ�
		*  @param[in] void
		*  @return    ElementType* �v�f�̐��|�C���^
		*************************************************************************/
		__forceinline ElementType* Data(){ return _data; }

		/*!**********************************************************************
		*  @brief     �q�[�v�̈�Ɋi�[����Ă���擪�̃|�C���^��Ԃ�
		*  @param[in] void
		*  @return    ElementType* �v�f�̐��|�C���^
		*************************************************************************/
		__forceinline const ElementType* Data() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief : �擪�̗v�f
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Front()       {return _data;}
		__forceinline const ElementType& Front() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief :�@�Ō�̗v�f
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Back()        { return _data[_size - 1]; }
		__forceinline const ElementType& Back() const { return _data[_size - 1]; }

		/*!**********************************************************************
		*  @brief     �z�񂪋󂩂ǂ����𔻒肵�܂�.
		*  @param[in] void
		*  @return    �v�f����0�ł����true��Ԃ��܂�
		*************************************************************************/
		__forceinline bool IsEmpty() const { return _size == 0; }

		/*!**********************************************************************
		*  @brief     ���Ɋm�ۍς݂̔z��̗v�f����Ԃ��܂�. (Capacity�ł͂Ȃ��ł�.) 
		*  @param[in] void
		*  @return    ���Ɋm�ۍς݂̔z��̗v�f��
		*************************************************************************/
		__forceinline uint64 Size() const { return _size; }

		/*----------------------------------------------------------------------
		*  @brief : �z��ɋl�ߍ��߂�ő�v�f��
		/*----------------------------------------------------------------------*/
		__forceinline uint64 Capacity() const { return _capacity; }

		/*----------------------------------------------------------------------
		*  @brief : �v�f�̃o�C�g����Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline static constexpr uint32 ByteOfElement() { return sizeof(ElementType); }

		/*----------------------------------------------------------------------
		*  @brief : �z��ɋl�ߍ��߂�c��̗v�f����Ԃ� (capacity - size)
		/*----------------------------------------------------------------------*/
		__forceinline uint64 ResidueCount() const { return _capacity - _size; }

		/*----------------------------------------------------------------------
		*  @brief : �w�肵��Index���̈�����ǂ���, �͈͊O�ł����Ă��r���Ŏ~�߂Ȃ�
		/*----------------------------------------------------------------------*/
		__forceinline bool InRange(const uint64 index) const { return 0 <= index && index <= _size; }

		/*----------------------------------------------------------------------
		*  @brief : �w�肵��Index���̈�����ǂ���, �͈͊O�ł�������r���Ŏ~�߂�
		/*----------------------------------------------------------------------*/
		__forceinline void CheckRange(const uint64 index) const
		{
			Checkf(0 <= index && index <= _size, "index is out of range. \n");
		}

		/*----------------------------------------------------------------------
		*  @brief : �f�[�^��͈͊m�F���s������ŕԂ�. 
		*           �͈͊O�ɓ������ꍇ�̓u���[�N�|�C���g�������I�ɂ�����, �������~�܂�܂�. 
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
		// �������̂��߂ɔ͈̓`�F�b�N���{���Ă��܂���. �͈̓`�F�b�N���s�������ꍇ��At���g�p���Ă�������
		__forceinline ElementType& operator[](const uint64 index)
		{
			return _data[index];
		}
		// �������̂��߂ɔ͈̓`�F�b�N���{���Ă��܂���. �͈̓`�F�b�N���s�������ꍇ��At���g�p���Ă�������
		__forceinline const ElementType& operator[](const uint64 index) const
		{
			return _data[index];
		}

		// �g���ʂ��g���đ�����s����悤�ɂ���
		DynamicArray& operator=(std::initializer_list<ElementType> list)
		{
			Clear();
			CreateFromOtherArray(list.begin(), list.size());
			return *this;
		}

		// �ق���Array(�^������)������
		DynamicArray& operator=(const DynamicArray& other)
		{
			if (this != &other)
			{
				Clear();
				CreateFromOtherArray(other.Data(), other.Size());
			}
			return *this;
		}

		// ���[�u�R���X�g���N�^
		DynamicArray& operator=(DynamicArray&& other)
		{
			if (this != &other)
			{
				// �q�[�v�̈�̑S�̎��͎��Ԃ������邽��, �����܂Ń|�C���^�̕t���ւ������őΉ����܂���.
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

		// �z��̗v�f����쐬����R���X�g���N�^
		DynamicArray(const ElementType* pointer, const uint64 count)
		{
			CreateFromOtherArray(pointer, count);
		}

		DynamicArray(const uint64 size) { Resize(size); }

		DynamicArray(const uint64 size, const ElementType& defaultElement) { Resize(size, true, defaultElement); }

		// �g���ʂŏ��������o����R���X�g���N�^
		DynamicArray(std::initializer_list<ElementType> list)
		{
			CreateFromOtherArray(list.begin(), list.size());
		}

		// �R�s�[�R���X�g���N�^
		DynamicArray(const DynamicArray& other) { CreateFromOtherArray(other.Data(), other.Size()); }

		// ���[�u�R���X�g���N�^
		DynamicArray(DynamicArray&& other) noexcept
		{
			// �q�[�v�̈�̑S�̎��͎��Ԃ������邽��, �����܂Ń|�C���^�̕t���ւ������őΉ����܂���.
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
		// @brief : ���ۂɐ��l�����͂���Ă���Index�̌�
		uint64 _size     = 0;

		// @brief : �ő���������e��
		uint64 _capacity = 0;

		// @brief : �v�f���i�[����z��̐擪�|�C���^
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
	*  @fn       �@void DynamicArray<ElementType>::Reserve(const uint64 capacity)
	*
	*  @brief     �T�C�Y��ύX���܂�
	*
	*  @param[in] const uint64 �z��̃T�C�Y (capacity�ł͂���܂���)
	*  @param[in] const bool   �R�s�[�R���X�g���N�^���g�p���邩 (false�ɂ���ƌ������̂��߂ɖ���������ԂŎn�܂�܂�.)
	* �@@param[in] const ElementType& �R�s�[���̗v�f (��{�f�t�H���g�R���X�g���N�^������ꂽ����.)
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Resize(const uint64 size, const bool useConstructor, const ElementType& defaultElement)
	{
		if (size <= _size) { return; }

		// �K��������
		Reserve(size);
		
		// �������R�s�[���s���񐔂�ݒ肷��
		const auto residueSize = size - _size;

		// �R�s�[�R���X�g���N�^���g���ꍇ��������������
		if (useConstructor)
		{
			if constexpr (sizeof(ElementType) == 1) // bool or character or byte
			{
				Memory::Set(_data, static_cast<uint8>(defaultElement), size); // 1 byte�ł��邱�Ƃ͕������Ă���̂�
			}
			else
			{
				for (uint64 i = _size; i < _size + residueSize; ++i)
				{
					_data[i] = defaultElement;
				}
			}
		}

		// �T�C�Y�̕ύX
		_size = size;
	}

	/****************************************************************************
	*                    Reserve
	*************************************************************************//**
	*  @fn       �@void DynamicArray<ElementType>::Reserve(const uint64 capacity)
	*
	*  @brief     �������̈�����O�Ɋm�ۂ��܂�. (������, �R���X�g���N�^�̌Ăяo���͑S���s���܂���)
	*
	*  @param[in] const uint64 capacity (�ő���������e��(�z��), �o�C�g�T�C�Y�ł͂Ȃ����Ƃɒ���)
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Reserve(const uint64 capacity)
	{
		/*-------------------------------------------------------------------
		-           ���Ƃ��Ƃ�菬���������珈�������Ȃ�
		---------------------------------------------------------------------*/
		if (capacity <= _capacity) { return; }

		/*-------------------------------------------------------------------
		-           ���Ƃ��Ɣz�񂪑��݂��Ă���ΑS�Ẵ��������R�s�[���������ō폜����
		---------------------------------------------------------------------*/
		auto newData = Memory::Allocate(capacity * sizeof(ElementType));

		if (_data != nullptr && _capacity)
		{
			Memory::Copy(newData, _data, _capacity * sizeof(ElementType));
			Memory::Free(_data);
		}

		/*-------------------------------------------------------------------
		-           Data�̍X�V
		---------------------------------------------------------------------*/
		_data     = (ElementType*)newData;
		_capacity = capacity;
	}

	/****************************************************************************
	*                    Clear
	*************************************************************************//**
	*  @fn       �@void DynamicArray<ElementType>::Clear()
	*
	*  @brief     �T�C�Y��0�ɂ��܂���, Capacity�̈掩�̂͂��̂܂܂ɂ��܂�
	*
	*  @param[in] void
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Clear()
	{
		/*-------------------------------------------------------------------
		-           �T�C�Y��0�Ȃ牽�����Ȃ�
		---------------------------------------------------------------------*/
		if (Size() == 0) { return; }

		/*-------------------------------------------------------------------
		-           �f�X�g���N�^����ʂ�Ăяo���Ă��烊�Z�b�g
		---------------------------------------------------------------------*/
		Memory::ForceExecuteDestructors(_data, Size());

		_size = 0;
	}

	/****************************************************************************
	*                    ShrinkToFit
	*************************************************************************//**
	*  @fn       �@void DynamicArray<ElementType>::ShrinkToFit()
	*
	*  @brief      Capacity���R���e�i��Size�܂Ő؂�l�߂� 
	*
	*  @param[in] void
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::ShrinkToFit()
	{
		/*-------------------------------------------------------------------
		-           �K�؂�Capacity���𒲂ׂ�
		---------------------------------------------------------------------*/
		if (_capacity == 0)     { return; }
		if (_size >= _capacity) { return; }

		/*-------------------------------------------------------------------
		-           ����̏ꍇ��_capacity�̕������size���傫�����Ƃ��������Ă��邽��, 
		            _data�ɒ��ڑ�����s���Ă���. 
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
	*  @fn       �@void DynamicArray<ElementType>::ShrinkToFit()
	*
	*  @brief      �z������ɒǉ�����. Capacity�𒴂����ꍇ, �S�̂̃�������2�{�̑傫���ōĊ��蓖�Ă��s��.
	*
	*  @param[in] ElementType& element
	*
	*  @return �@�@void
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
	*  @fn       �@void DynamicArray<ElementType>::Pop()
	*
	*  @brief     �Ō�̗v�f�����o���ăf�X�g���N�^���Ăяo��. (���������̂̔j���͍s��ꂸ, ���̌�T�C�Y�����ύX���܂�)
	*
	*  @param[in] void
	*
	*  @return �@�@void
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
	*  @fn       �@bool DynamicArray<ElementType>::Contains(const ElementType& element) const
	*
	*  @brief     �w�肵���v�f�����Ɋ܂܂�Ă���ꍇ��true��Ԃ��܂�.
	*
	*  @param[in] const ElementType& element
	*
	*  @return �@�@bool �܂܂�Ă���ꍇ��true
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
	*  @brief     Remove�̎���
	*
	*  @param[in] const uint64 index
	*  @param[in] const uint64 size
	*  @param[in] const bool allowShrinking
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::RemoveAtImplement(const uint64 index, const uint64 removeCount, const bool allowShrinking)
	{
		/*-------------------------------------------------------------------
		-           �͈̓`�F�b�N
		---------------------------------------------------------------------*/
		if (removeCount <= 0)              { return; }
		if (!InRange(index + removeCount)) { return; }

		/*-------------------------------------------------------------------
		-           �I������
		---------------------------------------------------------------------*/
		Memory::ForceExecuteDestructors(Data() + index, removeCount);

		/*-------------------------------------------------------------------
		-           �������̈ړ�
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
	*  @brief     �w�肵���z��C���f�b�N�X���������Ɍ����܂�. ������Ȃ������ꍇ��-1��n���܂�
	*
	*  @param[in] const ElementType& element
	*
	*  @return �@�@uint64
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
	*  @brief     �w�肵���z��C���f�b�N�X���t�����Ɍ����܂�. ������Ȃ������ꍇ��-1��n���܂�
	*
	*  @param[in] const ElementType& element
	*
	*  @return �@�@uint64
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
	*  @brief     �w�肵���v�f��S�č폜���܂�.
	*
	*  @param[in] const ElementType& element
	*  @param[in] const bool allowShrinking (capacity��؂�l�߂邩)
	*
	*  @return �@�@uint64
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