//////////////////////////////////////////////////////////////////////////////////
///  @file   SFTexture.hpp
///  @brief  �e�N�X�`���ǂݍ��ݏ��̎擾. ���ۂ̃e�N�X�`�����[�h�͕ʓr�s��.
///  @author toide
///  @date   2024/07/22 0:54:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_TEXTURE_HPP
#define SF_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	/****************************************************************************
	*				  		SFTextureOperator
	****************************************************************************/
	/* @brief  �قȂ�e�N�X�`�����m���u�����h����ۂ̉��Z���@
	*****************************************************************************/
	enum class SFTextureOperator
	{
		None,		    //!< �������Ȃ�
		Add,		    //!< ���Z
		Subtract,	    //!< ���Z
		Multiply,	    //!< ��Z
		Divide,		    //!< ���Z
		SmoothAdd,	    //!< �X���[�X���Z (Left + Right) - (Left * Right)
		SignedAdd,	    //!< �����t�����Z (Left + Right) - 0.5
	};

	/****************************************************************************
	*				  		SFTextureAddressingMode
	****************************************************************************/
	/* @brief  �e�N�X�`���̃A�h���b�V���O���[�h (0...1]�͈̔͊O��UV���W�̈��������`���܂�)
	*****************************************************************************/
	enum class SFTextureAddressingMode
	{
		Wrap,		    //!< �e�N�X�`���̌J��Ԃ�
		Mirror,		    //!< �e�N�X�`���̃~���[�����O
		Clamp,		    //!< �e�N�X�`���̃N�����v
		Border,		    //!< �e�N�X�`���̋��E
	};

	/****************************************************************************
	*				  		SFTextureMapping
	****************************************************************************/
	/* @brief  �e�N�X�`���̃}�b�s���O���@
	*****************************************************************************/
	enum class SFTextureMapping
	{
		UV,       //!< UV���W
		Sphere,   //!< ���ʍ��W
		Cube,     //!< �����̍��W
		Cylinder, //!< �~�����W
		Plane,    //!< ���ʍ��W
		Other,    //!< ���̑�
	};

	/****************************************************************************
	*				  			   SFTexture
	****************************************************************************/
	/* @brief  �e�N�X�`���̏����ꎞ�I�Ɋi�[���邽�߂̍\����.
	*****************************************************************************/
	struct SFTexture
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property

		/*! @brief �e�N�X�`���̃t�@�C���p�X*/
		gu::tstring FilePath = SP("");

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		SFTexture() = default;

		/*! @brief �f�X�g���N�^*/
		~SFTexture() = default;

		/*! @brief �t�@�C���p�X�̎w��*/
		SFTexture(const gu::tstring& filePath) : FilePath(filePath) {};
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