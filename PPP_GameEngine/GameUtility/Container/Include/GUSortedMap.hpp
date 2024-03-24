//////////////////////////////////////////////////////////////////////////////////
///             @file   GUSortedMap.hpp
///             @brief  Key�̏����ɏ]���ĕ��ׂ�ꂽ�ԍ��؃}�b�v
///                     std::map��60�p�[�Z���g�̑��x�Ō����ł��܂�. 
///             @author toide
///             @date   2024/02/25 0:00:11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_SORTED_MAP_HPP
#define GU_SORTED_MAP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GURedBlackTree.hpp"
#include "GUPair.hpp"
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
	*				  			   GUSortedMap
	*************************************************************************//**
	*  @class     GUSortedMap
	*  @brief     Key�̏����ɏ]���ĕ��ׂ�ꂽ�ԍ��؃}�b�v
	*****************************************************************************/
	template<class Key, class Value>
	class SortedMap
	{
	public:
		using TreeIterator      = details::tree::RedBlackTreeIterator<details::tree::RedBlackTreeNode<gu::Pair<Key, Value>>, gu::Pair<Key, Value>>;
		using TreeConstIterator = details::tree::RedBlackTreeIterator<details::tree::RedBlackTreeNode<gu::Pair<Key, Value>>, gu::Pair<Key, Value>>;
		// ����TreeConstIterator��const�o�Ȃ��̂Œ���
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �v�f��}������ 
		/*----------------------------------------------------------------------*/
		__forceinline void Insert(const gu::Pair<Key, Value>& element) { _tree.Insert(element); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f���폜����
		/*----------------------------------------------------------------------*/
		__forceinline void Clear() { return _tree.Clear(); }

		/*----------------------------------------------------------------------
		*  @brief : �R���e�i����ł��邩�ǂ����𒲂ׂ�
		/*----------------------------------------------------------------------*/
		__forceinline bool Contains(const Key& element) const { return _tree.Contains(gu::Pair<Key, Value>(element, Value())); }

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �R���e�i����ł��邩�ǂ����𒲂ׂ�
		/*----------------------------------------------------------------------*/
		__forceinline bool IsEmpty() const { return _tree.Size() == 0; }

		/*----------------------------------------------------------------------
		*  @brief : �v�f�����擾����
		/*----------------------------------------------------------------------*/
		__forceinline uint64 Size() const { return _tree.Size(); }

		/*----------------------------------------------------------------------
		*  @brief : �v�f���擾, ������Ȃ���΃G���[
		/*----------------------------------------------------------------------*/
		__forceinline Value& At(const Key& key) 
		{
			const auto pair = _tree.Search(gu::Pair<Key, Value>(key, Value()));
			if (pair) { return pair->Value; }
			else      { Checkf(false, "not include sorted map key"); return _empty;}
		}
		__forceinline const Value& At(const Key& key) const
		{
			const auto pair = _tree.Search(gu::Pair<Key, Value>(key, Value()));
			if (pair) { return pair->Value; }
			else { Checkf(false, "not include sorted map key"); return _empty;}
		}

		/*----------------------------------------------------------------------
		*  @brief : �v�f���擾, ������Ȃ���΃G���[
		/*----------------------------------------------------------------------*/
		__forceinline gu::Pair<Key, Value>& Pair(const Key& key)
		{
			const auto pair = _tree.Search(gu::Pair<Key, Value>(key, Value()));
			if (pair) { return *pair; }
			else { Checkf(false, "not include sorted map key"); }
		}
		__forceinline const gu::Pair<Key, Value>& Pair(const Key& key) const 
		{
			const auto pair = _tree.Search(gu::Pair<Key, Value>(key, Value()));
			if (pair) { return *pair; }
			else { Checkf(false, "not include sorted map key"); }
		}

#pragma region Operator Function
		__forceinline Value& operator[](const Key& key)
		{
			gu::Pair<Key, Value> initPair(key);
			const auto pair = _tree.Search(initPair); // Value�͓��ɉ������Ȃ�(�ʂ̕��@��͍��������Ƃ���)
			if (pair)
			{
				return pair->Value;
			}
			else
			{
				initPair.Value = Value();
				return _tree.Insert(initPair)->Value.Value;
			}
		}

#pragma endregion Operator Function

		TreeIterator      begin()       { return _tree.begin(); }
		TreeConstIterator begin() const { return _tree.begin(); }
		TreeIterator      end  ()       { return _tree.end(); }
		TreeConstIterator end  () const { return _tree.end(); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SortedMap() = default;

		SortedMap(const SortedMap& other) = default;

		SortedMap& operator=(const SortedMap&) = default;

		~SortedMap()
		{
			_tree.Clear();
		}
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		// @brief : ����
		gu::RedBlackTree<gu::Pair<Key, Value>> _tree;

		Value _empty = Value();
	};
}

#endif