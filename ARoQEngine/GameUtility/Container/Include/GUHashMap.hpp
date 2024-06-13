//////////////////////////////////////////////////////////////////////////////////
///  @file   GUHashMap.hpp
///  @brief  ロビンフッドハッシュ法によるハッシュテーブルです. スロット数は2のべき乗の制約があります. @n 
///          Valueの探索はO(1)で指定されます. (衝突が起こった場合はO(1)の類ではありません) @n
///          リハッシュは約80％ほどたまったタイミングで行われます. @n
///          キーに対するハッシュ化はプリミティブ型は勿論, 任意の構造体やクラスに対しても適切な準備を行えば可能です@n
///          任意の構造体やクラスに適用するハッシュ化は, まず, 対象のクラス内にpublicな[gu::uint64 GetTypedHash() const]を作成してください. @n
///          その中に, ハッシュ化を施すための64bitの非暗号化ハッシュ関数, gu::XX_64などを呼び出し, 比較を行いたいメンバ変数を全てハッシュ化すれば大丈夫です.
///  @note   参考コード : https://github.com/skarupke/flat_hash_map/blob/master/flat_hash_map.hpp#L558  @n
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
	/*!  @brief   ロビンフッドハッシュ法によるハッシュテーブルです. スロット数は2のべき乗の制約があります. @n 
	              Valueの探索はO(1)で指定されます. (衝突が起こった場合はO(1)の類ではありません) @n
				  リハッシュは約80％ほどたまったタイミングで行われます. @n
				  キーに対するハッシュ化はプリミティブ型は勿論, 任意の構造体やクラスに対しても適切な準備を行えば可能です@n
				  任意の構造体やクラスに適用するハッシュ化は, まず, 対象のクラス内にpublicな[gu::uint64 GetTypedHash() const]を作成してください. @n
				  その中に, ハッシュ化を施すための64bitの非暗号化ハッシュ関数, gu::XX_64などを呼び出し, 比較を行いたいメンバ変数を全てハッシュ化すれば大丈夫です.
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
		*  @brief     メモリ領域を事前に確保します. この時, サイズの変更などは行われません. 
		*  @param[in] const gu::uint64 要素数
		*  @return    void
		*************************************************************************/
		void Reserve(const gu::uint64 capacity);

		/*!**********************************************************************
		*  @brief     要素数を0にしますが, 事前に確保しているメモリ領域自体はそのままにします. @n
		*             デストラクタは呼び出されます.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Clear();

		/*!**********************************************************************
		*  @brief     指定のキーを持っている値を削除します
		*  @param[in] const Key& キーの値
		*  @return    bool trueで削除に成功, falseで対象が見つからなかったことを示します.
		*************************************************************************/
		bool Remove(const Key& key);

		/*!**********************************************************************
		*  @brief     HashMapに指定のキーが含まれているかを調べます
		*  @param[in] const Key& キーの値
		*  @return    bool
		*************************************************************************/
		__forceinline bool Contains(const Key& key) const { return Find(key) != end(); }

		/*!**********************************************************************
		*  @brief     メモリ領域を要素数まで切り詰め, リハッシュする @n
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void ShrinkToFit() { RehashForOtherMap(*this); }	

		/*!**********************************************************************
		*  @brief     ハッシュテーブルのバケット数を変更し, 全ての要素を新しいバケットに再配置します.
		*  @param[in] gu::uint64 バケットの個数
		*  @return    void
		*************************************************************************/
		void Rehash(gu::uint64 bucketCount);

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     HashMapが空かどうかを判定します.
		*  @param[in] void
		*  @return    bool : 要素数が0であればtrueを返します
		*************************************************************************/
		__forceinline bool IsEmpty() const { return _elementCount == 0; }

		/*!**********************************************************************
		*  @brief     既に確保済みのMapの要素数を返します.
		*  @param[in] void
		*  @return    gu::uint64 : 既に確保済みの配列の要素数
		*************************************************************************/
		__forceinline gu::uint64 Size() const { return _elementCount; }

		/*!**********************************************************************
		*  @brief     要素を取得, 見つからなければエラー
		*  @param[in] const Key キーの値
		*  @return    void
		*************************************************************************/
		__forceinline Value& At(const Key& key)
		{
			Iterator found = this->Find(key);
			if (found == this->end()) { throw "Argument passed to At() was not in the map."; }
			return found->Value;
		}

		/*!**********************************************************************
		*  @brief     要素を取得, 見つからなければエラー
		*  @param[in] const Key キーの値
		*  @return    void
		*************************************************************************/
		__forceinline const Value& At(const Key& key) const 
		{
			Const_Iterator found = this->Find(key);
			if (found == this->end()) { throw "Argument passed to At() was not in the map."; }
			return found->Value;
		}

		/*!**********************************************************************
		*  @brief     バケットの個数を返します
		*  @param[in] void
		*  @return    gu::uint64 : 個数
		*************************************************************************/
		__forceinline gu::uint64 BucketSize() const { return _slotsCountMinusOne ? _slotsCountMinusOne + 1 : 0; }

		/*!**********************************************************************
		*  @brief     2倍の値にテーブルを拡張する閾値 (0.0f <= threshold < 1.0fで設定する必要があると記載がありました.) 
		*  @param[in] void
		*  @return    閾値
		*************************************************************************/
		__forceinline float GetExpandTableThreshold() const { return _expandTableThreshold; }

		/*!**********************************************************************
		*  @brief     2倍の値にテーブルを拡張する閾値 (0.0f <= threshold < 1.0fで設定する必要があると記載がありました.)を設定します.
		*  @param[in] float 閾値
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
		*  @brief     初期値のイテレータを取得します
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
		*  @brief     初期値のイテレータを取得します
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
		*  @brief     終端値のイテレータを取得します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		Iterator end()
		{
			return { _elements + (_slotsCountMinusOne + _maxSearchCount) };
		}

		/*!**********************************************************************
		*  @brief     終端値のイテレータを取得します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		Iterator end() const
		{
			return { _elements + (_slotsCountMinusOne + _maxSearchCount) };
		}

		/*!**********************************************************************
		*  @brief     対象のイテレータを見つけます
		*  @param[in] void
		*  @return    Iterator
		*************************************************************************/
		Iterator Find(const Key& key);

		/*!**********************************************************************
		*  @brief     対象のイテレータを見つけます
		*  @param[in] void
		*  @return    Iterator
		*************************************************************************/
		Const_Iterator Find(const Key& key) const
		{
			return const_cast<HashMap*>(this)->Find(key);
		}

		#pragma endregion

		#pragma region Public Operator 
		/*! @brief 配列の要素を使ってvalueを取得します*/
		__forceinline Value& operator[](const Key& key)
		{
			return Emplace(key, Value()).Key->Value;
		}

		/*! @brief 配列の要素を使ってvalueを取得します*/
		__forceinline Value& operator[](Key&& key)
		{
			return Emplace(gu::type::Forward<Key>(key), Value()).Key->Value;
		}

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		HashMap()
		{
			Initialize();
		}

		/*! @brief デストラクタ*/
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
		*  @brief     空の状態にリセットします. 
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Initialize();

		/*!**********************************************************************
		*  @brief     要素のムーブを行いつつキーを追加します. 
		*  @param[in] Key&& 要素
		*  @param[in] Value&& 値
		*  @return    void
		*************************************************************************/
		gu::Pair<Iterator, bool> Emplace(const Key& key, Value&& value);

		/*!**********************************************************************
		*  @brief     ハッシュテーブルを2倍に拡大します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void Grow() { Rehash(2 * BucketSize() > 4 ? 2 * BucketSize() : 4); }

		/*!**********************************************************************
		*  @brief     メモリを解放します
		*  @return    void
		*************************************************************************/
		void Deallocate(Element* begin);

		/*!**********************************************************************
		*  @brief     ハッシュ値をもとに, バケットの理想的なIndexを取得します.
		*  @param[in] const gu::uint64 ハッシュ値
		*  @param[in] const gu::uint64 バケットの個数
		*  @return    gu::uint64
		*************************************************************************/
		__forceinline gu::uint64 CalculateIndexForHash(const gu::uint64 hash, const gu::uint64 bucketCount)
		{
			return hash & bucketCount;
		}

		/*!**********************************************************************
		*  @brief     次のサイズが指定のものより超えていた場合に次の2の乗数を取得します. 
		*  @param[in] void
		*  @return    gu::uint64
		*************************************************************************/
		gu::uint64 CalculateNextPowerOfTwo(gu::uint64 exponential) const;

		/*!**********************************************************************
		*  @brief     次のサイズが指定のものより超えていた場合に次の2の乗数を取得します.
		*  @param[in] void
		*  @return    gu::uint64
		*************************************************************************/
		gu::uint8 CalculateMaxSearchCount(const gu::uint64 bucketCount) const;
		#pragma endregion 

		#pragma region Private Property
		/*! @brief Mapに既に格納している要素数*/
		gu::uint64 _elementCount = 0;

		/*! @brief バケットの個数 - 1. この個数は2のべき乗に設定されます.*/
		gu::uint64 _slotsCountMinusOne = 0;

		/*! @brief 2倍の値にテーブルを拡張する閾値 (0.0f <= threshold < 1.0fで設定する必要があると記載がありました.) 
		    @note https://postd.cc/i-wrote-the-fastest-hashtable-1/ */
		float _expandTableThreshold = 0.8f;

		/*! @brief キーバリューのペア*/
		details::hashmap::HashMapElement<Key, Value>* _elements = nullptr;

		/*! @brief 最大のバケットの探索回数*/
		gu::int8 _maxSearchCount = MIN_SEARCH_COUNT - 1;

		/*! @brief 探索回数*/
		static constexpr gu::int8 MIN_SEARCH_COUNT = 4;
		#pragma endregion 

	};

	/*!**********************************************************************
	*  @brief     ハッシュテーブルのバケット数を変更し, 全ての要素を新しいバケットに再配置します.
	*  @param[in] gu::uint64 バケットの個数
	*  @return    void
	*************************************************************************/
	template<typename Key, typename Value>
	void HashMap<Key, Value>::Rehash(gu::uint64 bucketCount)
	{
		/*-------------------------------------------------------------------
		-        バケット数を計算します. 
		---------------------------------------------------------------------*/
		const auto tableThreshold = static_cast<gu::uint64>(_elementCount / static_cast<double>(_expandTableThreshold));
		bucketCount = bucketCount > tableThreshold ? bucketCount : tableThreshold;

		// まだバケット(ハッシュ値を探すもの)が見つからない場合は初期化を行う
		if (bucketCount == 0)
		{
			Initialize();
			return;
		}

		// 以降はバケットが存在する場合について扱います. 
		bucketCount = CalculateNextPowerOfTwo(bucketCount);

		// バケットサイズが変わらなかった場合はRehashの必要性がないため, そのまま終了します. 
		if (bucketCount == BucketSize())
		{
			return;
		}

		/*-------------------------------------------------------------------
		-        テーブルがスロットを検索する回数上限を取得します. 
		  (探索上限を超えた場合, テーブルはスロットの検索をあきらめて配列を拡張するようにします.)
		---------------------------------------------------------------------*/
		const auto searchCount = CalculateMaxSearchCount(bucketCount);

		/*-------------------------------------------------------------------
		-       新しいバケットを確保します. 
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
		-       探索回数の更新
		---------------------------------------------------------------------*/
		--_slotsCountMinusOne;
		gu::int8 oldSearchCount = _maxSearchCount;
		_maxSearchCount = searchCount;

		/*-------------------------------------------------------------------
		-       エントリの再配置
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
	*  @brief     メモリ領域を事前に確保します. この時, サイズの変更などは行われません.
	*  @param[in] const gu::uint64 要素数
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
	*  @brief     サイズを0にしますが, Capacity領域自体はそのままにします. @n
	*             デストラクタは呼び出されます.
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
	*  @brief     対象のイテレータを見つけます
	*  @param[in] void
	*  @return    Iterator
	*************************************************************************/
	template<typename Key, typename Value>
	HashMap<Key, Value>::Iterator HashMap<Key, Value>::Find(const Key& key)
	{
		/*-------------------------------------------------------------------
		-       エントリを配置するための理想的なインデックスを取得します.
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
		-       対象のキーをハッシュマップから取得する
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
	*  @brief     指定のキーを持っている値を削除します
	*  @param[in] const Key& キーの値
	*  @return    bool trueで削除に成功, falseで対象が見つからなかったことを示します.
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
	*  @brief     要素のムーブを行いつつキーを追加します.
	*  @param[in] Key&& 要素
	*  @return    void
	*************************************************************************/
	template<typename Key, typename Value>
	gu::Pair<typename HashMap<Key, Value>::Iterator, bool> HashMap<Key, Value>::Emplace(const Key& key, Value&& value)
	{
		/*-------------------------------------------------------------------
		-       エントリを配置するための理想的なインデックスを取得します.
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
		-       新しいエントリが挿入されるべき位置を見つけるためにハッシュテーブルを走査し, 
		        既に新しいキーが既にハッシュテーブルに存在するかを確認します.
		---------------------------------------------------------------------*/
		Element* currentEntry        = _elements + (desiredIndex);
		gu::int8 distanceFromDesired = 0;

		for (; currentEntry->ProbeDistance >= distanceFromDesired; ++currentEntry, ++distanceFromDesired)
		{
			if (key == currentEntry->Data.Key)
			{
				// 既にキーの値が登録されている場合, そのままキーを追加せずに終了する
				return gu::Pair<Iterator, bool>(Iterator(currentEntry), false);
			}
		}

		/*-------------------------------------------------------------------
		-     　ハッシュテーブルが空, 新しいエントリが最大の探索距離を超える場合, 拡大の閾値を超えた場合に拡大する.
		---------------------------------------------------------------------*/
		if (_slotsCountMinusOne == 0 || distanceFromDesired == _maxSearchCount || _elementCount + 1 > (_slotsCountMinusOne + 1) * (_expandTableThreshold))
		{
			Grow();
			return Emplace(key, gu::type::Forward<Value>(value)).Key;
		}
		/*-------------------------------------------------------------------
		-     　指定したエントリが空だった場合はそのまま挿入します.
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
		-     　ロビンフッド法に基づいて, 新しいエントリが適切な位置に挿入されるまでの間に,既に入っている要素の方が
		       本来の場所からの距離が短いならば今入れようとしている要素と交換して, 取り出した要素を別の場所に入れるために衝突処理を続けます
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
		// 高速なlog2
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
	*  @brief     次のサイズが指定のものより超えていた場合に次の2の乗数を取得します. 
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
	*  @brief     次のサイズが指定のものより超えていた場合に次の2の乗数を取得します.
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
	*  @brief     メモリを解放します
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