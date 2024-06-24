//////////////////////////////////////////////////////////////////////////////////
///             @file   GUClosedHashTable.hpp
///             @brief  別のデータ構造へのインデックス付けに使用される、
/// 　　　　　　　　　　　　　静的サイズのハッシュテーブルで, Mapよりも高速です. 
///                     参考資料 : http://www.hi-ho.ne.jp/a_ogawa/document/pg_dynahash.pdf 
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
	*  @brief     別のデータ構造へのインデックス付けに使用される静的サイズのハッシュテーブルです
	*             固定長配列を使っているため, DynamicHashTableよりも高速です.
	*             !HashTableのサイズは2の階乗である必要があります.
	*             key : ハッシュ値, id : ハッシュ値の元となったID
	*****************************************************************************/
	template<uint16 HashTableSize, uint16 IndexSize>
	class ClosedHashTable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : HashTableの中身を全て1で初期化する. 
		*----------------------------------------------------------------------*/
		__forceinline void Clear() { Memory::Set(_hashTable, 0xff, HashTableSize * 2); }

		/*----------------------------------------------------------------------
		*  @brief : HashTableにハッシュ値となるKeyとハッシュ値の元となるIDを登録します
		*----------------------------------------------------------------------*/
		void Register(const uint16 keyHash, const uint16 id);

		/*----------------------------------------------------------------------
		*  @brief : HashTableにハッシュ値となるKeyとハッシュ値の元となるIDを登録解除します.
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
		// @brief : Bucket : ハッシュテーブルの各要素のこと
		//          ハッシュ値が格納されているテーブルを固定長配列で取得
		uint16 _backets[HashTableSize];

		// @brief : 連結リストの代わりとなる固定長配列のIndexSize
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