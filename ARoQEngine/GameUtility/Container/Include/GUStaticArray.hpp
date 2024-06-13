//////////////////////////////////////////////////////////////////////////////////
///             @file   GUStaticArray.hpp
///             @brief  temp
///             @author toide
///             @date   2024/02/18 3:08:51
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_STATIC_ARRAY_HPP
#define GU_STATIC_ARRAY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Container/Include/GUInitializerList.hpp"
#include "../Private/Iterator/Include/GUIteratorIncludes.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			   GUStaticArray
	****************************************************************************/
	/* @class     GUStaticArray
	*  @brief     temp
	*****************************************************************************/
	template<class ElementType, uint64 ArraySize>
	class StaticArray
	{
	public:
		static constexpr uint64 INDEX_NONE = static_cast<uint64>(-1);

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �R���e�i���w�肳�ꂽ�l�Ŗ��߂�
		/*----------------------------------------------------------------------*/
		void Fill(const ElementType& element);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �q�[�v�̈�Ɋi�[����Ă���擪�̃|�C���^��Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType* Data()       { return _data;}
		__forceinline const ElementType* Data() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief : �擪�̗v�f
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Front()       { return _data; }
		__forceinline const ElementType& Front() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief : �Ō�̗v�f
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Back()       { return _data[ArraySize - 1]; }
		__forceinline const ElementType& Back() const { return _data[ArraySize - 1]; }

		/*----------------------------------------------------------------------
		*  @brief : �z�񂪋󂩂ǂ���
		/*----------------------------------------------------------------------*/
		__forceinline bool IsEmpty() const { return ArraySize == 0; }

		/*----------------------------------------------------------------------
		*  @brief : �z��̗v�f��
		/*----------------------------------------------------------------------*/
		__forceinline uint64 Size() const { return ArraySize; }

		/*----------------------------------------------------------------------
		*  @brief : �w�肵��Index���̈�����ǂ���, �͈͊O�ł����Ă��r���Ŏ~�߂Ȃ�
		/*----------------------------------------------------------------------*/
		__forceinline bool InRange(const uint64 index) const { return 0 <= index && index <= ArraySize; }

		/*----------------------------------------------------------------------
		*  @brief : �w�肵��Index���̈�����ǂ���, �͈͊O�ł�������r���Ŏ~�߂�
		/*----------------------------------------------------------------------*/
		__forceinline void CheckRange(const uint64 index) const
		{
			Checkf(0 <= index && index <= ArraySize, "index is out of range. \n");
		}

		/*----------------------------------------------------------------------
		*  @brief : �f�[�^��͈͊m�F���s������ŕԂ�.
		*           �͈͊O�ɓ������ꍇ�̓u���[�N�|�C���g�������I�ɂ�����, �������~�܂�܂�.
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& At(const uint64 index)       { CheckRange(index); return _data[index]; }
		__forceinline const ElementType& At(const uint64 index) const { CheckRange(index); return _data[index]; }

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
		StaticArray& operator=(std::initializer_list<ElementType> list)
		{
			if (_data) { delete[](_data); }
			CreateFromOtherArray(list.begin(), list.size());
			return *this;
		}

		// �ق���Array(�^������)������
		StaticArray& operator=(const StaticArray& other)
		{
			if (this != &other)
			{
				if (_data) { delete[](_data); }
				CreateFromOtherArray(other.Data(), other.Size());
			}
			return *this;
		}

		// ���[�u�R���X�g���N�^
		StaticArray& operator=(StaticArray&& other)
		{
			if (this != &other)
			{
				// �q�[�v�̈�̑S�̎��͎��Ԃ������邽��, �����܂Ń|�C���^�̕t���ւ������őΉ����܂���.
				_data = other._data;     other._data = nullptr;
				_size = other._size;     other._size = 0;
			}
			return *this;
		}
#pragma endregion Operator Function
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		StaticArray() { _data = new ElementType[ArraySize]; }

		StaticArray(std::initializer_list<ElementType> list)
		{
			CreateFromOtherArray(list.begin(), list.size());
		}

		// �R�s�[�R���X�g���N�^
		StaticArray(const StaticArray& other) { CreateFromOtherArray(other.Data(), other.Size()); }

		// ���[�u�R���X�g���N�^
		StaticArray(StaticArray&& other) noexcept
		{
			// �q�[�v�̈�̑S�̎��͎��Ԃ������邽��, �����܂Ń|�C���^�̕t���ւ������őΉ����܂���.
			_data = other._data;     other._data = nullptr;
			_size = other._size;     other._size = 0;
		}

		~StaticArray()
		{
			if (_data) { delete[](_data); }
		}

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void CreateFromOtherArray(const ElementType* pointer, const uint64 count);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/

		// @brief : �v�f���i�[����z��̐擪�|�C���^
		ElementType* _data = nullptr;
	};

#pragma region Implement
	template<class ElementType, uint64 ArraySize>
	void StaticArray<ElementType, ArraySize>::CreateFromOtherArray(const ElementType* pointer, const uint64 count)
	{
		Check(ArraySize == count);
		Check(ArraySize > 0);

		// null�`�F�b�N
		if (pointer == nullptr) { return; }

		// ��x�j��
		if (_data) { delete[](_data); }

		// �z��̗̈���m�ۂ��ăR�s�[
		_data = new ElementType[count];
		Memory::Copy(_data, pointer, sizeof(ElementType) * count);
	}

	/*----------------------------------------------------------------------
	*  @brief : �R���e�i���w�肳�ꂽ�l�Ŗ��߂�
	/*----------------------------------------------------------------------*/
	template<class ElementType, uint64 ArraySize>
	void StaticArray<ElementType, ArraySize>::Fill(const ElementType& element)
	{
		Check(ArraySize > 0);

		if constexpr (sizeof(ElementType) == 1)
		{
			Memory::Set(_data, static_cast<uint8>(element), ArraySize);
		}
		else
		{
			for (uint64 i = 0; i < ArraySize; ++i)
			{
				_data[i] = element;
			}
		}
	}
#pragma endregion Implement
}

#endif