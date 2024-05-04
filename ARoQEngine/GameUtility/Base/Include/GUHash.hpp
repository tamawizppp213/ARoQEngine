//////////////////////////////////////////////////////////////////////////////////
///  @file   GUHash.hpp
///  @brief  様々なハッシュ関数をまとめたクラスです. 
///  @author toide
///  @date   2024/04/29 10:24:55
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_HASH_HPP
#define GU_HASH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUHash
	*************************************************************************//**
	/*  @class     GUHash
	*   @brief     様々なハッシュ関数をまとめたクラスです. 
	*****************************************************************************/
	class Hash
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     FNV_1 : 軽量でありながら比較的高い分散を持つ. @n
		              セキュリティ用途に用いることができるほどの耐衝突性は無し (std::hashはこれ)
		*  @param[in] const void*   文字列データ
		*  @param[in] const uint64  バイト長
		*  @param[in] const uint32  初期値 (シード値):デフォルトは0です
		*  @return    uint32        ハッシュ値
		*************************************************************************/
		static uint32 FNV1a_32(const void* key, const uint64 byteLength, const uint32 seed = 0);

		/*!**********************************************************************
		*  @brief     FNV_1 : 軽量でありながら比較的高い分散を持つ. @n
					  セキュリティ用途に用いることができるほどの耐衝突性は無し (std::hashはこれ)
		*  @param[in] const uint8*  文字列データ
		*  @param[in] const uint64  バイト長
		*  @param[in] const uint64  初期値 (シード値):デフォルトは0です
		*  @return    gu::uint64 : ハッシュ値
		*************************************************************************/
		static uint64 FNV1a_64(const void* key, const uint64 byteLength, const uint64 seed = 0);

		/*!**********************************************************************
		*  @brief     Murmur2 : FNVと同等以上の性能で, 軽量かつ高い分散を持つ. アラインメント対応はしてません @n
		*  @param[in] const uint8*  文字列データ
		*  @param[in] const uint64  バイト長
		*  @param[in] const uint32  初期値 (シード値):デフォルトは0です
		*  @return    gu::uint32 : ハッシュ値
		*************************************************************************/
		static uint32 Murmur2_32(const void* key, const uint64 byteLength, const uint32 seed = 0);

		/*!**********************************************************************
		*  @brief     Murmur2 : FNVと同等以上の性能で, 軽量かつ高い分散を持つアラインメント. 対応はしてません @n
		*  @param[in] const uint8*  文字列データ
		*  @param[in] const uint64  バイト長
		*  @param[in] const uint64  初期値 (シード値):デフォルトは0です
		*  @return    gu::uint64 : ハッシュ値
		*************************************************************************/
		static uint64 Murmur2_64(const void* key, const uint64 byteLength, const uint64 seed = 0);

		/*!**********************************************************************
		*  @brief     XX_32 : xxhashアルゴリズムを用いた非暗号化ハッシュ. 高速に動作するため, 本エンジンの基本ハッシュ化アルゴリズムに設定
		*  @param[in] const uint8*  文字列データ
		*  @param[in] const uint64  バイト長
		*  @param[in] const uint32  初期値 (シード値):デフォルトは0です
		*  @return    gu::uint32 : ハッシュ値
		*************************************************************************/
		static uint32 XX_32(const void* key, const uint64 byteLength, const uint32 seed = 0);

		/*!**********************************************************************
		*  @brief     XX_64 : xxhashアルゴリズムを用いた非暗号化ハッシュ. 高速に動作するため, 本エンジンの基本ハッシュ化アルゴリズムに設定
		*  @param[in] const uint8*  文字列データ
		*  @param[in] const uint64  バイト長
		*  @param[in] const uint64  初期値 (シード値):デフォルトは0です
		*  @return    gu::uint64 : ハッシュ値
		*************************************************************************/
		static uint64 XX_64(const void* key, const uint64 byteLength, const uint64 seed = 0);
		#pragma endregion 

		#pragma region Public Property

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
		// FNV Const
		static constexpr uint32 FNV_OFFSET_BASIS_32 = 2166136261U;
		static constexpr uint64 FNV_OFFSET_BASIS_64 = 14695981039346656037U;
		static constexpr uint32 FNV_PRIME_32        = 16777619U;
		static constexpr uint64 FNV_PRIME_64        = 1099511628211LLU;

		// XX hash Const
		static constexpr uint32 XX_PRIME1_32 = 0x9E3779B1U;
		static constexpr uint32 XX_PRIME2_32 = 0x85EBCA77U;
		static constexpr uint32 XX_PRIME3_32 = 0xC2B2AE3DU;
		static constexpr uint32 XX_PRIME4_32 = 0x27D4EB2FU;
		static constexpr uint32 XX_PRIME5_32 = 0x165667B1U;

		static constexpr uint64 XX_PRIME1_64 = 11400714785074694791ULL;
		static constexpr uint64 XX_PRIME2_64 = 14029467366897019727ULL;
		static constexpr uint64 XX_PRIME3_64 = 1609587929392839161ULL;
		static constexpr uint64 XX_PRIME4_64 = 9650029242287828579ULL;
		static constexpr uint64 XX_PRIME5_64 = 2870177450012600261ULL;
		#pragma endregion 

	};

}
#endif