//////////////////////////////////////////////////////////////////////////////////
///  @file   BaseObject.hpp
///  @brief  シリアライズなどの基本的な機能を持つクラス
///  @author toide
///  @date   2024/06/29 0:13:19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{

	/****************************************************************************
	*				  			   BaseObject
	*************************************************************************//**
	/*  @brief  シリアライズなどの基本的な機能を持つクラス
	*****************************************************************************/
	class BaseObject
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      初期化されているかを調べます
		*  @param[in]  void
		*  @return     const bool ： trueで初期化済みを示す.
		**************************************************************************/

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:

		#pragma region Protected Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		BaseObject() = default;

		/*! @brief デストラクタ*/
		virtual ~BaseObject() = default;

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