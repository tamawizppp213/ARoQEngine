//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAlignment.hpp
///  @brief  第1引数で入力された値を第2引数の倍数に近い値に揃えるアラインメント処理を実行するためのクラスです. @n
///          AlignUp       : 最も第2引数の倍数に近い値のうち, 大きい方の値に揃えます. bit演算のため, 高速ではあるがAlignmentは2のべき乗である必要がある.@n 
///          AlignDown     : 最も第2引数の倍数に近い値のうち, 小さい方の値に揃えます. 2のべき乗にします @n
///          AlignArbitary : 最も第2引数の倍数に近い値のうち, 大きい方の値に揃えます.　bit演算を使用しないため, 2のべき乗である必要はありません@n 
///  @author toide
///  @date   2024/04/22 1:41:58
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_ALIGNMENT_HPP
#define GU_ALIGNMENT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUTypeTraits.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{ 
	/****************************************************************************
	*				  			   Alignment
	*************************************************************************//**
	/* @brief      第1引数で入力された値を第2引数の倍数に近い値に揃えるアラインメント処理を実行するためのクラスです. @n
    *              AlignUp       : 最も第2引数の倍数に近い値のうち, 大きい方の値に揃えます. bit演算のため, 高速ではあるがAlignmentは2のべき乗である必要がある.@n 
    *              AlignDown     : 最も第2引数の倍数に近い値のうち, 小さい方の値に揃えます. 2のべき乗にします @n
    *              AlignArbitary : 最も第2引数の倍数に近い値のうち, 大きい方の値に揃えます.　bit演算を使用しないため, 2のべき乗である必要はありません@n 
	*****************************************************************************/
	class Alignment
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     Alignmentの最も近い倍数（2のべき乗でなければならない）に値を揃える。
		*  @param[in] アラインメントを行う値
		*  @param[in] 2のべき乗で指定する, 倍数にそろえたい値
		*  @return    アラインメントされた後の値
		*************************************************************************/
		template<typename T>
		__forceinline static constexpr T AlignUp(const T value, const gu::uint64 alignment)
		{
			static_assert(gu::type::IS_INTEGRAL<T> || gu::type::IS_POINTER<T>, "Align expects an integer or pointer type");

			return (T)(((uint64)value + alignment - 1) & ~(alignment - 1));
		}

		/*!**********************************************************************
		*  @brief     Alignmentに最も近い低い方の値（2のべき乗でなければならない）に値を揃える。
		*  @param[in] アラインメントを行う値
		*  @param[in] 2のべき乗の値
		*  @return    アラインメントされた後の値
		*************************************************************************/
		template<typename T>
		__forceinline static constexpr T AlignDown(const T value, const gu::uint64 alignment)
		{
			static_assert(gu::type::IS_INTEGRAL<T> || gu::type::IS_POINTER<T>, "Align down expects an integer or pointer type");

			return (T)(((uint64)value) & ~(alignment - 1));
		}

		/*!**********************************************************************
		*  @brief     アラインメント済みかどうか
		*  @param[in] アラインメントを行う値
		*  @param[in] 2のべき乗の値
		*  @return    アラインメントされた後の値
		*************************************************************************/
		template<typename T>
		__forceinline static constexpr bool IsAligned(const T value, const gu::uint64 alignment)
		{
			static_assert(gu::type::IS_INTEGRAL<T> || gu::type::IS_POINTER<T>, "IsAligned expects an integer or pointer type");

			return !((uint64)value & (alignment - 1));
		}

		/*!**********************************************************************
		*  @brief     Alignmentの最も近い倍数に揃える. この時, 2のべき乗以外でも対応可能です
		*  @param[in] アラインメントを行う値
		*  @param[in] 2のべき乗の値
		*  @return    アラインメントされた後の値
		*************************************************************************/
		template<typename T>
		__forceinline static constexpr T AlignUpArbitary(const T value, const gu::uint64 alignment)
		{
			static_assert(gu::type::IS_INTEGRAL<T> || gu::type::IS_POINTER<T>, "AlignArbitrary expects an integer or pointer type");

			return (T)((((uint64)value + alignment - 1) / alignment) * alignment);
		}

		#pragma endregion 

		#pragma region Public Member Variables

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

		#pragma region Protected Member Variables

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Member Variables

		#pragma endregion 

	};

}
#endif