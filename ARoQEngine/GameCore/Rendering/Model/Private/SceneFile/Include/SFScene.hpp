//////////////////////////////////////////////////////////////////////////////////
///  @file   SFScene.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/21 23:18:03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_SCENE_HPP
#define SF_SCENE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "SFMesh.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	/****************************************************************************
	*				  			   SFScene
	****************************************************************************/
	/* @brief  �V�[�������ꎞ�I�Ɋi�[���邽�߂̍\����. 
	*****************************************************************************/
	struct SFScene
	{
	public:
		#pragma region Public Function
		
		/*! @brief 3�������b�V��*/
		gu::DynamicArray<SFMesh> Meshes = {};

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