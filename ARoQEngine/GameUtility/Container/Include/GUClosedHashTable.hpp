//////////////////////////////////////////////////////////////////////////////////
///             @file   GUClosedHashTable.hpp
///             @brief  �ʂ̃f�[�^�\���ւ̃C���f�b�N�X�t���Ɏg�p�����A
/// �@�@�@�@�@�@�@�@�@�@�@�@�@�ÓI�T�C�Y�̃n�b�V���e�[�u����, Map���������ł�. 
///                     �Q�l���� : http://www.hi-ho.ne.jp/a_ogawa/document/pg_dynahash.pdf 
///             @author toide
///             @date   2024/02/19 0:20:14
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_CLOSED_HASH_TABLE_HPP
#define GU_CLOSED_HASH_TABLE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUClosedHashTable
	****************************************************************************/
	/* @class     GUStaticHashTable
	*  @brief     �ʂ̃f�[�^�\���ւ̃C���f�b�N�X�t���Ɏg�p�����ÓI�T�C�Y�̃n�b�V���e�[�u���ł�
	*             �Œ蒷�z����g���Ă��邽��, DynamicHashTable���������ł�.
	*             !HashTable�̃T�C�Y��2�̊K��ł���K�v������܂�.
	*             key : �n�b�V���l, id : �n�b�V���l�̌��ƂȂ���ID
	*****************************************************************************/
	template<uint16 HashTableSize, uint16 IndexSize>
	class ClosedHashTable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : HashTable�̒��g��S��1�ŏ���������. 
		*----------------------------------------------------------------------*/
		__forceinline void Clear() { Memory::Set(_hashTable, 0xff, HashTableSize * 2); }

		/*----------------------------------------------------------------------
		*  @brief : HashTable�Ƀn�b�V���l�ƂȂ�Key�ƃn�b�V���l�̌��ƂȂ�ID��o�^���܂�
		*----------------------------------------------------------------------*/
		void Register(const uint16 keyHash, const uint16 id);

		/*----------------------------------------------------------------------
		*  @brief : HashTable�Ƀn�b�V���l�ƂȂ�Key�ƃn�b�V���l�̌��ƂȂ�ID��o�^�������܂�.
		*----------------------------------------------------------------------*/
		void Cancel(const uint16 keyHash, const uint16 id);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		static constexpr uint16 INVALID_VALUE = 0xffff;

		uint16 First(const uint16 keyHash) const;

		uint16 Next(const uint16 id) const;

		bool IsValid(const uint16 id) const;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ClosedHashTable();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		// @brief : Bucket : �n�b�V���e�[�u���̊e�v�f�̂���
		//          �n�b�V���l���i�[����Ă���e�[�u�����Œ蒷�z��Ŏ擾
		uint16 _backets[HashTableSize];

		// @brief : �A�����X�g�̑���ƂȂ�Œ蒷�z���IndexSize
		uint16 _nextIndices[IndexSize];
	};

#pragma region Implement
	template<uint16 HashTableSize, uint16 IndexSize>
	__forceinline ClosedHashTable<HashTableSize, IndexSize>::ClosedHashTable()
	{
		static_assert((HashTableSize & (HashTableSize - 1)) == 0, "Hash table size must be power of 2");
		static_assert(IndexSize - 1 < 0xffff, "Index 0xffff is reserved for the initialization");
		Clear();
	}
#pragma endregion Implement
}

#endif