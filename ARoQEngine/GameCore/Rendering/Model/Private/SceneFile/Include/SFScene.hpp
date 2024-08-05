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
#include "SFSceneNode.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	/****************************************************************************
	*				  			   SFSceneFlag
	****************************************************************************/
	/* @brief  �V�[�������쐬���鎞�̃t���O
	*****************************************************************************/
	enum class SFSceneFlag : gu::uint8
	{
		None = 0,		//!< �������Ȃ�

	};

	/****************************************************************************
	*				  			   SFScene
	****************************************************************************/
	/* @brief  �V�[�������ꎞ�I�Ɋi�[���邽�߂̍\����. 
	*****************************************************************************/
	struct SFScene
	{
	public:
		#pragma region Public Function
		
		#pragma endregion 

		#pragma region Public Property
		/*! @brief Root. Import�����������ꍇ, ���RootNode�͑��݂��܂�. */
		SFSceneNode* RootSceneNode = nullptr;

		/*! @brief 3�������b�V��[�}�e���A���̐��������쐬����܂�.*/
		gu::DynamicArray<SFMesh> Meshes = {};

		/*! @brief �}�e���A��*/
		gu::DynamicArray<SFMaterial> Materials = {};

		/*! @brief �e�N�X�`��*/
		gu::DynamicArray<SFTexture> Textures = {};

		/*! @brief �V�[�������t���O*/
		SFSceneFlag Flag = SFSceneFlag::None;
		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		SFScene() = default;

		/*! @brief �f�X�g���N�^*/
		~SFScene() = default;
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