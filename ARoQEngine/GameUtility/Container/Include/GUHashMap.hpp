//////////////////////////////////////////////////////////////////////////////////
///  @file   GUHashMap.hpp
///  @brief  ���r���t�b�h�n�b�V���@�ɂ��n�b�V���e�[�u���ł�. �X���b�g����2�ׂ̂���̐��񂪂���܂�. @n 
///          Value�̒T����O(1)�Ŏw�肳��܂�. (�Փ˂��N�������ꍇ��O(1)�̗ނł͂���܂���) @n
///          ���n�b�V���͖�80���قǂ��܂����^�C�~���O�ōs���܂�. @n
///          �L�[�ɑ΂���n�b�V�����̓v���~�e�B�u�^�͖ܘ_, �C�ӂ̍\���̂�N���X�ɑ΂��Ă��K�؂ȏ������s���Ή\�ł�@n
///          �C�ӂ̍\���̂�N���X�ɓK�p����n�b�V������, �܂�, �Ώۂ̃N���X����public��[gu::uint64 GetTypedHash() const]���쐬���Ă�������. @n
///          ���̒���, �n�b�V�������{�����߂�64bit�̔�Í����n�b�V���֐�, gu::XX_64�Ȃǂ��Ăяo��, ��r���s�����������o�ϐ���S�ăn�b�V��������Α��v�ł�.
///  @note   �Q�l�R�[�h : https://github.com/skarupke/flat_hash_map/blob/master/flat_hash_map.hpp#L558  @n
///                     https://postd.cc/i-wrote-the-fastest-hashtable-1/ @n
///                     https://postd.cc/i-wrote-the-fastest-hashtable/
///  @author toide
///  @date   2024/04/29 9:25:41
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_HASH_MAP_HPP
#define GU_HASH_MAP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Container/Private/HashMap/Include/GUHashMapElement.hpp"
#include "GameUtility/Container/Private/HashMap/Include/GUHashMapIterator.hpp"
#include "GameUtility/Base/Include/GUTypeTraits.hpp"
#include "GameUtility/Base/Include/GUHash.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Base/Include/GUTypeCast.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUHashMap
	*************************************************************************//**
	/*!  @brief   ���r���t�b�h�n�b�V���@�ɂ��n�b�V���e�[�u���ł�. �X���b�g����2�ׂ̂���̐��񂪂���܂�. @n 
	              Value�̒T����O(1)�Ŏw�肳��܂�. (�Փ˂��N�������ꍇ��O(1)�̗ނł͂���܂���) @n
				  ���n�b�V���͖�80���قǂ��܂����^�C�~���O�ōs���܂�. @n
				  �L�[�ɑ΂���n�b�V�����̓v���~�e�B�u�^�͖ܘ_, �C�ӂ̍\���̂�N���X�ɑ΂��Ă��K�؂ȏ������s���Ή\�ł�@n
				  �C�ӂ̍\���̂�N���X�ɓK�p����n�b�V������, �܂�, �Ώۂ̃N���X����public��[gu::uint64 GetTypedHash() const]���쐬���Ă�������. @n
				  ���̒���, �n�b�V�������{�����߂�64bit�̔�Í����n�b�V���֐�, gu::XX_64�Ȃǂ��Ăяo��, ��r���s�����������o�ϐ���S�ăn�b�V��������Α��v�ł�.
	*****************************************************************************/
	template<typename Key, typename Value>
	class HashMap 
	{
		using Element        = details::hashmap::HashMapElement<Key, Value>;

	public:
		using Iterator       = details::hashmap::TemplatedIterator<Key, Value>;
		using Const_Iterator = details::hashmap::TemplatedIterator<Key, Value>;

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �������̈�����O�Ɋm�ۂ��܂�. ���̎�, �T�C�Y�̕ύX�Ȃǂ͍s���܂���. 
		*  @param[in] const gu::uint64 �v�f��
		*  @return    void
		*************************************************************************/
		void Reserve(const gu::uint64 capacity);

		/*!**********************************************************************
		*  @brief     �v�f����0�ɂ��܂���, ���O�Ɋm�ۂ��Ă��郁�����̈掩�̂͂��̂܂܂ɂ��܂�. @n
		*             �f�X�g���N�^�͌Ăяo����܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Clear();

		/*!**********************************************************************
		*  @brief     �w��̃L�[�������Ă���l���폜���܂�
		*  @param[in] const Key& �L�[�̒l
		*  @return    bool true�ō폜�ɐ���, false�őΏۂ�������Ȃ��������Ƃ������܂�.
		*************************************************************************/
		bool Remove(const Key& key);

		/*!**********************************************************************
		*  @brief     HashMap�Ɏw��̃L�[���܂܂�Ă��邩�𒲂ׂ܂�
		*  @param[in] const Key& �L�[�̒l
		*  @return    bool
		*************************************************************************/
		__forceinline bool Contains(const Key& key) const { return Find(key) != end(); }

		/*!**********************************************************************
		*  @brief     �������̈��v�f���܂Ő؂�l��, ���n�b�V������ @n
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void ShrinkToFit() { RehashForOtherMap(*this); }	

		/*!**********************************************************************
		*  @brief     �n�b�V���e�[�u���̃o�P�b�g����ύX��, �S�Ă̗v�f��V�����o�P�b�g�ɍĔz�u���܂�.
		*  @param[in] gu::uint64 �o�P�b�g�̌�
		*  @return    void
		*************************************************************************/
		void Rehash(gu::uint64 bucketCount);

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     HashMap���󂩂ǂ����𔻒肵�܂�.
		*  @param[in] void
		*  @return    bool : �v�f����0�ł����true��Ԃ��܂�
		*************************************************************************/
		__forceinline bool IsEmpty() const { return _elementCount == 0; }

		/*!**********************************************************************
		*  @brief     ���Ɋm�ۍς݂�Map�̗v�f����Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::uint64 : ���Ɋm�ۍς݂̔z��̗v�f��
		*************************************************************************/
		__forceinline gu::uint64 Size() const { return _elementCount; }

		/*!**********************************************************************
		*  @brief     �v�f���擾, ������Ȃ���΃G���[
		*  @param[in] const Key �L�[�̒l
		*  @return    void
		*************************************************************************/
		__forceinline Value& At(const Key& key)
		{
			Iterator found = this->Find(key);
			if (found == this->end()) { throw "Argument passed to At() was not in the map."; }
			return found->Value;
		}

		/*!**********************************************************************
		*  @brief     �v�f���擾, ������Ȃ���΃G���[
		*  @param[in] const Key �L�[�̒l
		*  @return    void
		*************************************************************************/
		__forceinline const Value& At(const Key& key) const 
		{
			Const_Iterator found = this->Find(key);
			if (found == this->end()) { throw "Argument passed to At() was not in the map."; }
			return found->Value;
		}

		/*!**********************************************************************
		*  @brief     �o�P�b�g�̌���Ԃ��܂�
		*  @param[in] void
		*  @return    gu::uint64 : ��
		*************************************************************************/
		__forceinline gu::uint64 BucketSize() const { return _slotsCountMinusOne ? _slotsCountMinusOne + 1 : 0; }

		/*!**********************************************************************
		*  @brief     2�{�̒l�Ƀe�[�u�����g������臒l (0.0f <= threshold < 1.0f�Őݒ肷��K�v������ƋL�ڂ�����܂���.) 
		*  @param[in] void
		*  @return    臒l
		*************************************************************************/
		__forceinline float GetExpandTableThreshold() const { return _expandTableThreshold; }

		/*!**********************************************************************
		*  @brief     2�{�̒l�Ƀe�[�u�����g������臒l (0.0f <= threshold < 1.0f�Őݒ肷��K�v������ƋL�ڂ�����܂���.)��ݒ肵�܂�.
		*  @param[in] float 臒l
		*  @return    void
		*************************************************************************/
		__forceinline void SetExpandTableThreshold(const float threshold) const 
		{
			Check(0.0f <= threshold && threshold < 1.0f);
			_expandTableThreshold = threshold;
		}

		#pragma endregion 

		#pragma region Public Iterator
		/*!**********************************************************************
		*  @brief     �����l�̃C�e���[�^���擾���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		Iterator begin()
		{
			for (Element* Iterator = _elements; iterator != end(); ++iterator)
			{
				if (iterator->HasValue()) { return iterator; }
			}
		}

		/*!**********************************************************************
		*  @brief     �����l�̃C�e���[�^���擾���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		Const_Iterator begin() const
		{
			for (Element* iterator = _elements; iterator != end(); ++iterator)
			{
				if (iterator->HasValue()) { return iterator; }
			}
		}

		/*!**********************************************************************
		*  @brief     �I�[�l�̃C�e���[�^���擾���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		Iterator end()
		{
			return { _elements + (_slotsCountMinusOne + _maxSearchCount) };
		}

		/*!**********************************************************************
		*  @brief     �I�[�l�̃C�e���[�^���擾���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		Iterator end() const
		{
			return { _elements + (_slotsCountMinusOne + _maxSearchCount) };
		}

		/*!**********************************************************************
		*  @brief     �Ώۂ̃C�e���[�^�������܂�
		*  @param[in] void
		*  @return    Iterator
		*************************************************************************/
		Iterator Find(const Key& key);

		/*!**********************************************************************
		*  @brief     �Ώۂ̃C�e���[�^�������܂�
		*  @param[in] void
		*  @return    Iterator
		*************************************************************************/
		Const_Iterator Find(const Key& key) const
		{
			return const_cast<HashMap*>(this)->Find(key);
		}

		#pragma endregion

		#pragma region Public Operator 
		/*! @brief �z��̗v�f���g����value���擾���܂�*/
		__forceinline Value& operator[](const Key& key)
		{
			return Emplace(key, Value()).Key->Value;
		}

		/*! @brief �z��̗v�f���g����value���擾���܂�*/
		__forceinline Value& operator[](Key&& key)
		{
			return Emplace(gu::type::Forward<Key>(key), Value()).Key->Value;
		}

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		HashMap()
		{
			Initialize();
		}

		/*! @brief �f�X�g���N�^*/
		~HashMap()
		{
			Clear();
			Deallocate(_elements);
		}
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function
		/*!**********************************************************************
		*  @brief     ��̏�ԂɃ��Z�b�g���܂�. 
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Initialize();

		/*!**********************************************************************
		*  @brief     �v�f�̃��[�u���s���L�[��ǉ����܂�. 
		*  @param[in] Key&& �v�f
		*  @param[in] Value&& �l
		*  @return    void
		*************************************************************************/
		gu::Pair<Iterator, bool> Emplace(const Key& key, Value&& value);

		/*!**********************************************************************
		*  @brief     �n�b�V���e�[�u����2�{�Ɋg�債�܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void Grow() { Rehash(2 * BucketSize() > 4 ? 2 * BucketSize() : 4); }

		/*!**********************************************************************
		*  @brief     ��������������܂�
		*  @return    void
		*************************************************************************/
		void Deallocate(Element* begin);

		/*!**********************************************************************
		*  @brief     �n�b�V���l�����Ƃ�, �o�P�b�g�̗��z�I��Index���擾���܂�.
		*  @param[in] const gu::uint64 �n�b�V���l
		*  @param[in] const gu::uint64 �o�P�b�g�̌�
		*  @return    gu::uint64
		*************************************************************************/
		__forceinline gu::uint64 CalculateIndexForHash(const gu::uint64 hash, const gu::uint64 bucketCount)
		{
			return hash & bucketCount;
		}

		/*!**********************************************************************
		*  @brief     ���̃T�C�Y���w��̂��̂�蒴���Ă����ꍇ�Ɏ���2�̏搔���擾���܂�. 
		*  @param[in] void
		*  @return    gu::uint64
		*************************************************************************/
		gu::uint64 CalculateNextPowerOfTwo(gu::uint64 exponential) const;

		/*!**********************************************************************
		*  @brief     ���̃T�C�Y���w��̂��̂�蒴���Ă����ꍇ�Ɏ���2�̏搔���擾���܂�.
		*  @param[in] void
		*  @return    gu::uint64
		*************************************************************************/
		gu::uint8 CalculateMaxSearchCount(const gu::uint64 bucketCount) const;
		#pragma endregion 

		#pragma region Private Property
		/*! @brief Map�Ɋ��Ɋi�[���Ă���v�f��*/
		gu::uint64 _elementCount = 0;

		/*! @brief �o�P�b�g�̌� - 1. ���̌���2�ׂ̂���ɐݒ肳��܂�.*/
		gu::uint64 _slotsCountMinusOne = 0;

		/*! @brief 2�{�̒l�Ƀe�[�u�����g������臒l (0.0f <= threshold < 1.0f�Őݒ肷��K�v������ƋL�ڂ�����܂���.) 
		    @note https://postd.cc/i-wrote-the-fastest-hashtable-1/ */
		float _expandTableThreshold = 0.8f;

		/*! @brief �L�[�o�����[�̃y�A*/
		details::hashmap::HashMapElement<Key, Value>* _elements = nullptr;

		/*! @brief �ő�̃o�P�b�g�̒T����*/
		gu::int8 _maxSearchCount = MIN_SEARCH_COUNT - 1;

		/*! @brief �T����*/
		static constexpr gu::int8 MIN_SEARCH_COUNT = 4;
		#pragma endregion 

	};

	/*!**********************************************************************
	*  @brief     �n�b�V���e�[�u���̃o�P�b�g����ύX��, �S�Ă̗v�f��V�����o�P�b�g�ɍĔz�u���܂�.
	*  @param[in] gu::uint64 �o�P�b�g�̌�
	*  @return    void
	*************************************************************************/
	template<typename Key, typename Value>
	void HashMap<Key, Value>::Rehash(gu::uint64 bucketCount)
	{
		/*-------------------------------------------------------------------
		-        �o�P�b�g�����v�Z���܂�. 
		---------------------------------------------------------------------*/
		const auto tableThreshold = static_cast<gu::uint64>(_elementCount / static_cast<double>(_expandTableThreshold));
		bucketCount = bucketCount > tableThreshold ? bucketCount : tableThreshold;

		// �܂��o�P�b�g(�n�b�V���l��T������)��������Ȃ��ꍇ�͏��������s��
		if (bucketCount == 0)
		{
			Initialize();
			return;
		}

		// �ȍ~�̓o�P�b�g�����݂���ꍇ�ɂ��Ĉ����܂�. 
		bucketCount = CalculateNextPowerOfTwo(bucketCount);

		// �o�P�b�g�T�C�Y���ς��Ȃ������ꍇ��Rehash�̕K�v�����Ȃ�����, ���̂܂܏I�����܂�. 
		if (bucketCount == BucketSize())
		{
			return;
		}

		/*-------------------------------------------------------------------
		-        �e�[�u�����X���b�g����������񐔏�����擾���܂�. 
		  (�T������𒴂����ꍇ, �e�[�u���̓X���b�g�̌�����������߂Ĕz����g������悤�ɂ��܂�.)
		---------------------------------------------------------------------*/
		const auto searchCount = CalculateMaxSearchCount(bucketCount);

		/*-------------------------------------------------------------------
		-       �V�����o�P�b�g���m�ۂ��܂�. 
		---------------------------------------------------------------------*/
		Element* newBuckets     = (Element*)(Memory::Allocate((bucketCount + searchCount) * sizeof(Element)));
		Element* specialEndItem = newBuckets + (bucketCount + searchCount - 1); 
		
		for (Element* iterator = newBuckets; iterator != specialEndItem; ++iterator)
		{
			iterator->ProbeDistance = Element::INVALID_VALUE;
		}
		specialEndItem->ProbeDistance = Element::SPECIAL_END_VALUE;

		gu::type::Swap(_elements, newBuckets);
		gu::type::Swap(_slotsCountMinusOne, bucketCount);

		/*-------------------------------------------------------------------
		-       �T���񐔂̍X�V
		---------------------------------------------------------------------*/
		--_slotsCountMinusOne;
		gu::int8 oldSearchCount = _maxSearchCount;
		_maxSearchCount = searchCount;

		/*-------------------------------------------------------------------
		-       �G���g���̍Ĕz�u
		---------------------------------------------------------------------*/
		_elementCount = 0;
		Element* iterator = newBuckets;
		Element* end = iterator + (bucketCount + oldSearchCount);
		for (; iterator != end; ++iterator)
		{
			if (iterator->HasValue())
			{
				Emplace(gu::type::Forward<Key>(iterator->Data.Key), gu::type::Forward<Value>(iterator->Data.Value));
				iterator->DestroyValue();
			}
		}

		Deallocate(newBuckets);
	}

	template<typename Key, typename Value>
	void HashMap<Key, Value>::Initialize()
	{
		Deallocate(_elements);
		_elements       = Element::EmptyDefaultTable();
		_slotsCountMinusOne    = 0;
		_elementCount   = 0;
		_maxSearchCount = MIN_SEARCH_COUNT - 1;
	}

	/*!**********************************************************************
	*  @brief     �������̈�����O�Ɋm�ۂ��܂�. ���̎�, �T�C�Y�̕ύX�Ȃǂ͍s���܂���.
	*  @param[in] const gu::uint64 �v�f��
	*  @return    void
	*************************************************************************/
	template<typename Key, typename Value>
	void HashMap<Key, Value>::Reserve(const gu::uint64 capacity)
	{
		const auto minLoadFactor   = _expandTableThreshold > 0.5f ? 0.5f : _expandTableThreshold;
		const auto requiredBuckets = static_cast<gu::uint64>(_elementCount / minLoadFactor);
	    
		if (requiredBuckets > BucketSize())
		{
			Rehash(requiredBuckets);
		}
	}

	/*!**********************************************************************
	*  @brief     �T�C�Y��0�ɂ��܂���, Capacity�̈掩�̂͂��̂܂܂ɂ��܂�. @n
	*             �f�X�g���N�^�͌Ăяo����܂�.
	*  @param[in] void
	*  @return    void
	*************************************************************************/
	template<typename Key, typename Value>
	void HashMap<Key, Value>::Clear()
	{
		for (auto it = _elements, end = it + (_slotsCountMinusOne + _maxSearchCount); it != end; ++it)
		{
			if (it->HasValue())
			{
				it->DestroyValue();
			}
		}

		_elementCount = 0;
	}

	/*!**********************************************************************
	*  @brief     �Ώۂ̃C�e���[�^�������܂�
	*  @param[in] void
	*  @return    Iterator
	*************************************************************************/
	template<typename Key, typename Value>
	HashMap<Key, Value>::Iterator HashMap<Key, Value>::Find(const Key& key)
	{
		/*-------------------------------------------------------------------
		-       �G���g����z�u���邽�߂̗��z�I�ȃC���f�b�N�X���擾���܂�.
		---------------------------------------------------------------------*/
		gu::uint64 keyHash = 0;
		if constexpr (gu::type::IS_ARITHMETIC<Key> || gu::type::IS_POINTER<Key>)
		{
			keyHash = Hash::XX_64(&key, sizeof(Key), 0);
		}
		else
		{
			keyHash = key.GetTypedHash();
		}
		const auto desiredIndex = CalculateIndexForHash(keyHash, _slotsCountMinusOne);

		/*-------------------------------------------------------------------
		-       �Ώۂ̃L�[���n�b�V���}�b�v����擾����
		---------------------------------------------------------------------*/
		Element* iterator = _elements + desiredIndex;
		
		for (gu::int8 distance = 0; iterator->ProbeDistance >= distance; ++distance, ++iterator)
		{
			if (key == iterator->Data.Key)
			{
				return { iterator };
			}
		}

		return end();
	}

	/*!**********************************************************************
	*  @brief     �w��̃L�[�������Ă���l���폜���܂�
	*  @param[in] const Key& �L�[�̒l
	*  @return    bool true�ō폜�ɐ���, false�őΏۂ�������Ȃ��������Ƃ������܂�.
	*************************************************************************/
	template<typename Key, typename Value>
	bool HashMap<Key, Value>::Remove(const Key& key)
	{
		Iterator found = Find(key);

		if (found == end()) { return false; }

		Element* current = found.Current;
		current->DestroyValue();
		--_elementCount;

		for (Element* next = current + 1; !next->IsDesiredPosition(); ++current, ++next)
		{
			current->ProbeDistance = next->ProbeDistance - 1;
			current->Data.Key      = gu::type::Forward<Key>  (next->Data.Key);
			current->Data.Value    = gu::type::Forward<Value>(next->Data.Value);
			next->DestroyValue();
		}
	}

	/*!**********************************************************************
	*  @brief     �v�f�̃��[�u���s���L�[��ǉ����܂�.
	*  @param[in] Key&& �v�f
	*  @return    void
	*************************************************************************/
	template<typename Key, typename Value>
	gu::Pair<typename HashMap<Key, Value>::Iterator, bool> HashMap<Key, Value>::Emplace(const Key& key, Value&& value)
	{
		/*-------------------------------------------------------------------
		-       �G���g����z�u���邽�߂̗��z�I�ȃC���f�b�N�X���擾���܂�.
		---------------------------------------------------------------------*/
		gu::uint64 keyHash = 0;
		if constexpr (gu::type::IS_ARITHMETIC<Key> || gu::type::IS_POINTER<Key>)
		{
			keyHash = Hash::XX_64(&key, sizeof(Key), 0);
		}
		else
		{
			keyHash = key.GetTypedHash();
		}

		const auto desiredIndex = CalculateIndexForHash(keyHash, _slotsCountMinusOne);

		/*-------------------------------------------------------------------
		-       �V�����G���g�����}�������ׂ��ʒu�������邽�߂Ƀn�b�V���e�[�u���𑖍���, 
		        ���ɐV�����L�[�����Ƀn�b�V���e�[�u���ɑ��݂��邩���m�F���܂�.
		---------------------------------------------------------------------*/
		Element* currentEntry        = _elements + (desiredIndex);
		gu::int8 distanceFromDesired = 0;

		for (; currentEntry->ProbeDistance >= distanceFromDesired; ++currentEntry, ++distanceFromDesired)
		{
			if (key == currentEntry->Data.Key)
			{
				// ���ɃL�[�̒l���o�^����Ă���ꍇ, ���̂܂܃L�[��ǉ������ɏI������
				return gu::Pair<Iterator, bool>(Iterator(currentEntry), false);
			}
		}

		/*-------------------------------------------------------------------
		-     �@�n�b�V���e�[�u������, �V�����G���g�����ő�̒T�������𒴂���ꍇ, �g���臒l�𒴂����ꍇ�Ɋg�傷��.
		---------------------------------------------------------------------*/
		if (_slotsCountMinusOne == 0 || distanceFromDesired == _maxSearchCount || _elementCount + 1 > (_slotsCountMinusOne + 1) * (_expandTableThreshold))
		{
			Grow();
			return Emplace(key, gu::type::Forward<Value>(value)).Key;
		}
		/*-------------------------------------------------------------------
		-     �@�w�肵���G���g�����󂾂����ꍇ�͂��̂܂ܑ}�����܂�.
		---------------------------------------------------------------------*/
		else if (currentEntry->IsEmpty())
		{
			currentEntry->Data.Key      = key;
			currentEntry->Data.Value    = gu::type::Forward<Value>(value);
			currentEntry->ProbeDistance = distanceFromDesired;
			++_elementCount;
			return { Iterator(currentEntry), true };
		}

		/*-------------------------------------------------------------------
		-     �@���r���t�b�h�@�Ɋ�Â���, �V�����G���g�����K�؂Ȉʒu�ɑ}�������܂ł̊Ԃ�,���ɓ����Ă���v�f�̕���
		       �{���̏ꏊ����̋������Z���Ȃ�΍�����悤�Ƃ��Ă���v�f�ƌ�������, ���o�����v�f��ʂ̏ꏊ�ɓ���邽�߂ɏՓˏ����𑱂��܂�
		---------------------------------------------------------------------*/
		gu::Pair<Key, Value> insertPair(key, gu::type::Forward<Value>(value));
		gu::type::Swap(distanceFromDesired, currentEntry->ProbeDistance);
		gu::type::Swap(insertPair, currentEntry->Data);

		Iterator result(currentEntry);
		for (++distanceFromDesired, ++currentEntry;; ++currentEntry)
		{
			if (currentEntry->IsEmpty())
			{
				currentEntry->Data = gu::type::Forward<gu::Pair<Key, Value>>(insertPair);
				currentEntry->ProbeDistance = distanceFromDesired;
				++_elementCount;
				return { result, true };
			}
			else if (currentEntry->ProbeDistance < distanceFromDesired)
			{
				gu::type::Swap(distanceFromDesired, currentEntry->ProbeDistance);
				gu::type::Swap(insertPair, currentEntry->Data);
				++distanceFromDesired;
			}
			else
			{
				++distanceFromDesired;
				if (distanceFromDesired == _maxSearchCount)
				{
					gu::type::Swap(insertPair, result.Current->Data);
					Grow();
					return Emplace(insertPair.Key, gu::type::Forward<Value>(insertPair.Value)).Key;
				}
			}
		}
	}

	namespace details
	{
		// ������log2
		inline gu::int8 log2(gu::uint64 value)
		{
			static constexpr int8 table[64] =
			{
				63,  0, 58,  1, 59, 47, 53,  2,
				60, 39, 48, 27, 54, 33, 42,  3,
				61, 51, 37, 40, 49, 18, 28, 20,
				55, 30, 34, 11, 43, 14, 22,  4,
				62, 57, 46, 52, 38, 26, 32, 41,
				50, 36, 17, 19, 29, 10, 13, 21,
				56, 45, 25, 31, 35, 16,  9, 12,
				44, 24, 15,  8, 23,  7,  6,  5
			};
			value |= value >> 1;
			value |= value >> 2;
			value |= value >> 4;
			value |= value >> 8;
			value |= value >> 16;
			value |= value >> 32;
			return table[((value - (value >> 1)) * 0x07EDD5E59A4E28C2) >> 58];
		}
	}

	/*!**********************************************************************
	*  @brief     ���̃T�C�Y���w��̂��̂�蒴���Ă����ꍇ�Ɏ���2�̏搔���擾���܂�. 
	*  @param[in] void
	*  @return    gu::uint64
	*************************************************************************/
	template<typename Key, typename Value>
	gu::uint64 HashMap<Key, Value>::CalculateNextPowerOfTwo(gu::uint64 exponential) const
	{
		--exponential;
		exponential |= exponential >> 1;
		exponential |= exponential >> 2;
		exponential |= exponential >> 4;
		exponential |= exponential >> 8;
		exponential |= exponential >> 16;
		exponential |= exponential >> 32;
		++exponential;
		return exponential;
	}

	/*!**********************************************************************
	*  @brief     ���̃T�C�Y���w��̂��̂�蒴���Ă����ꍇ�Ɏ���2�̏搔���擾���܂�.
	*  @param[in] void
	*  @return    gu::uint64
	*************************************************************************/
	template<typename Key, typename Value>
	gu::uint8 HashMap<Key, Value>::CalculateMaxSearchCount(const gu::uint64 bucketCount) const
	{
		const auto desiredSize = details::log2(bucketCount);
		return desiredSize > MIN_SEARCH_COUNT ? desiredSize : MIN_SEARCH_COUNT;
	}

	/*!**********************************************************************
	*  @brief     ��������������܂�
	*  @return    void
	*************************************************************************/
	template<typename Key, typename Value>
	void HashMap<Key, Value>::Deallocate(Element* begin)
	{
		if (begin != nullptr && begin != details::hashmap::HashMapElement<Key, Value>::EmptyDefaultTable())
		{
			Memory::Free(begin);
		}
	}
}

#endif