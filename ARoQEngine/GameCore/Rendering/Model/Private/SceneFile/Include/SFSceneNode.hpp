//////////////////////////////////////////////////////////////////////////////////
///  @file   SFScene.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/21 23:18:03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_SCENE_NODE_HPP
#define SF_SCENE_NODE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "SFMesh.hpp"
#include "SFMaterial.hpp"
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
	struct SFSceneNode
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �Ώۂ̖��O��SceneNode�m�F
		*  @param[in] const gu::tstring& ���b�V����
		*  @return    SFSceneNode*
		*************************************************************************/
		SFSceneNode* FindNode(const gu::tstring& name);

		/*!**********************************************************************
		*  @brief     �Ώۂ̖��O��SceneNode�m�F
		*  @param[in] const gu::tstring& ���b�V����
		*  @return    SFSceneNode*
		*************************************************************************/
		const SFSceneNode* FindNode(const gu::tstring& name) const;

		#pragma endregion 

		#pragma region Public Property
		/*! @brief �e�m�[�h*/
		SFSceneNode* Parent = nullptr;

		/*! @brief �q�m�[�h*/
		gu::DynamicArray<SFSceneNode*> Children = {};

		/*! @brief �m�[�h��*/
		gu::tstring Name = SP("");

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		SFSceneNode() = default;

		/*! @brief �f�X�g���N�^*/
		~SFSceneNode() = default;

		explicit SFSceneNode(const gu::tstring& name);
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