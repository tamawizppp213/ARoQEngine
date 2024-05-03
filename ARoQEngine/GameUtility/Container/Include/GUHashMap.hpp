//////////////////////////////////////////////////////////////////////////////////
///  @file   GUHashMap.hpp
///  @brief  オープンアドレス法と線形探査, と2のべき乗でのテーブルサイズの制約を行うHashMapクラスです. @n 
///          Valueの探索はO(1)で指定されます. (衝突が起こった場合はO(1)の類ではありません)
///  @author toide
///  @date   2024/04/29 9:25:41
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_HASH_MAP_HPP
#define GU_HASH_MAP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUPair.hpp"

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
	/*!  @brief   オープンアドレス法と線形探査, と2のべき乗でのテーブルサイズの制約を行うHashMapクラスです. @n 
	*             Valueの探索はO(1)で指定されます. (衝突が起こった場合はO(1)の類ではありません)
	*****************************************************************************/
	template<typename Key, typename Value>
	class HashMap 
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		HashMap() = default;


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