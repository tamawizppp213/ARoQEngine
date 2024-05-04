//////////////////////////////////////////////////////////////////////////////////
///  @file   HashMapElement.hpp
///  @brief  temp
///  @author toide
///  @date   2024/05/04 0:20:02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_HASH_MAP_ELEMENT_HPP
#define GU_HASH_MAP_ELEMENT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Container/Include/GUPair.hpp"
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::details::hashmap
{
	/****************************************************************************
	*				  			   HashMapElement
	*************************************************************************//**
	/*  @class     HashMapElement
	*   @brief     ロビンフッド法を使用するための連想配列の要素
	*****************************************************************************/
	template<typename Key, typename Value>
	struct HashMapElement
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     空のデフォルトテーブルを渡す (値の変更はくれぐれもやらないようにしてください)
		*  @param[in] void
		*  @return    HashMapElement*
		*************************************************************************/
		static HashMapElement* EmptyDefaultTable()
		{
			static HashMapElement result[MIN_SEARCH_COUNT] = { {}, {}, {}, {SPECIAL_END_VALUE} };
			return result;
		}
		#pragma endregion 

		#pragma region Public Property
		/*! @brief 最小の探索回数*/
		static constexpr gu::int8 MIN_SEARCH_COUNT = 4;

		/*! @brief 無効値*/
		static constexpr gu::int8 INVALID_VALUE = -1;

		/*! @brief 特別な終端値*/
		static constexpr gu::int8 SPECIAL_END_VALUE = 0;

		/*! @brief 各エントリが理想的な位置からどれだけ離れているか*/
		gu::int8 ProbeDistance = INVALID_VALUE;

		/*! @brief HashMapの実データ*/
		gu::Pair<Key, Value> Data = {};

		/*!**********************************************************************
		*  @brief     値が既に初期化されているか
		*  @param[in] void
		*  @return    bool : trueで初期化済み
		*************************************************************************/
		__forceinline bool HasValue() const { return ProbeDistance >= 0; }

		/*!**********************************************************************
		*  @brief     値がまだ設定されていないか
		*  @param[in] void
		*  @return    bool : trueで未設定
		*************************************************************************/
		__forceinline bool IsEmpty() const { return ProbeDistance < 0; }

		/*!**********************************************************************
		*  @brief     望んだ場所にいるかどうか
		*  @param[in] void
		*  @return    bool : trueでその場所にいる
		*************************************************************************/
		__forceinline bool IsDesiredPosition() const { return ProbeDistance == 0; }

		/*!**********************************************************************
		*  @brief     値を破棄する
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void DestroyValue()
		{
			Memory::ForceExecuteDestructors(&Data.Value, 1);
			ProbeDistance = INVALID_VALUE;
		}

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor


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

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif