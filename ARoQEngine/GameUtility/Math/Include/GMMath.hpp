//////////////////////////////////////////////////////////////////////////////////
///  @file   GMMath.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/21 13:29:08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_MATH_HPP
#define GM_MATH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMMathConstants.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	/****************************************************************************
	*				  			   GMMath
	****************************************************************************/
	/* @brief  êîäw
	*****************************************************************************/
	class Math
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ç≈ëÂílÇï‘ÇµÇ‹Ç∑. 
		*************************************************************************/
		template<typename T>
		static T Max(const T a, const T b)
		{
			return a > b ? a : b;
		}

		/*!**********************************************************************
		*  @brief     ç≈è¨ílÇï‘ÇµÇ‹Ç∑.
		*************************************************************************/
		template<typename T>
		static T Min(const T a, const T b)
		{
			return a < b ? a : b;
		}

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

		#pragma endregion 

	};


}
#endif