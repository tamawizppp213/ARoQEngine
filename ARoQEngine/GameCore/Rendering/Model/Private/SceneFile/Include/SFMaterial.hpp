//////////////////////////////////////////////////////////////////////////////////
///  @file   SFTexture.hpp
///  @brief  �e�N�X�`���ǂݍ��ݏ��̎擾. ���ۂ̃e�N�X�`�����[�h�͕ʓr�s��.
///  @author toide
///  @date   2024/07/22 0:54:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_MATERIAL_HPP
#define SF_MATERIAL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "SFTexture.hpp"
#include "GameUtility/Base/Include/GUOptional.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	/****************************************************************************
	*				  			   SFMaterial
	****************************************************************************/
	/* @brief  �e�N�X�`���̏����ꎞ�I�Ɋi�[���邽�߂̍\����.
	*****************************************************************************/
	struct SFMaterial
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*! @brief �}�e���A����*/
		gu::tstring Name = SP("");

		/*! @brief �}�e���A���̊g�U�F*/
		gm::Float4 Diffuse = gm::Float4(1.0f, 1.0f, 1.0f, 1.0f);

		/*! @brief �}�e���A���̔��ːF*/
		gm::Float3 Specular = gm::Float3(1.0f, 1.0f, 1.0f);

		/*! @brief �}�e���A���̔��ˋ��x*/
		gu::float32 SpecularPower = 1.0f;

		/*! @brief �}�e���A���̊��F*/
		gm::Float3 Ambient = gm::Float3(1.0f, 1.0f, 1.0f);

		/*! @brief DiffuseMap�̃e�N�X�`��*/
		gu::Optional<SFTexture> DiffuseMap = {};
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