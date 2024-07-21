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
	/* @brief  ���w
	*****************************************************************************/
	class Math
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �ő�l��Ԃ��܂�. 
		*************************************************************************/
		template<typename T>
		static T Max(const T a, const T b)
		{
			return a > b ? a : b;
		}

		/*!**********************************************************************
		*  @brief     �ŏ��l��Ԃ��܂�.
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