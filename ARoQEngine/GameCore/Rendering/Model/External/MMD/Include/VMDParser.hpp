//////////////////////////////////////////////////////////////////////////////////
///             @file   VMDParser.hpp
///             @brief  VMDParser
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VMD_PARSER_HPP
#define VMD_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Private/Include/VMDDefines.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::file::vmd
{
	/****************************************************************************
	*				  			VMDFile
	*************************************************************************//**
	*   @brief  ���[�V�����f�[�^���L�^����N���X�ł�. vmd�t�@�C����ǂݍ��ނ��Ƃ��ł��܂�.
	*****************************************************************************/
	struct VMDFile : public gu::NonCopyable
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     VMD�t�@�C����ǂݍ��ފ֐�
		*  @param[in] gu::tsring& �t�@�C���p�X
		*  @return    bool
		*************************************************************************/
		bool Read(const gu::tstring& filePath);

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�����������ފ֐�
		*  @param[in] gu::tsring& �t�@�C���p�X
		*  @return    void
		*************************************************************************/
		bool Write(const gu::tstring& filePath);

		#pragma endregion

		#pragma region Public Property
		
		/*! @brief ���[�V�����f�[�^�̃w�b�_���. ���f�����������Ă��܂�*/
		VMDHeader Header = {};

		/*! @brief �{�[���̃L�[�t���[�����*/
		gu::DynamicArray<VMDBoneKeyFrame> BoneFrames = {};

		/*! @brief �\��̃L�[�t���[�����*/
		gu::DynamicArray<VMDFaceKeyFrame> FaceFrames = {};

		/*! @brief �J�����̃L�[�t���[�����*/
		gu::DynamicArray<VMDCameraKeyFrame> CameraFrames = {};

		/*! @brief �Ɩ��̃L�[�t���[�����*/
		gu::DynamicArray<VMDLightKeyFrame> LightFrames = {};

		/*! @brief �Z���t�V���h�E�̃L�[�t���[����� */
		gu::DynamicArray<VMDSelfShadowKeyFrame> SelfShadowFrames = {};

		/*! @brief IK�̃L�[�t���[����� */
		gu::DynamicArray<VMDIKKeyFrame> IKFrames = {};

		/*! @brief VMD�t�@�C�����i�[����Ă���e�f�B���N�g��*/
		gu::tstring Directory = SP("");

		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		VMDFile() = default;

		/*! @brief �f�X�g���N�^*/
		~VMDFile();

		#pragma endregion

	private:
		#pragma region Private Function
		#pragma endregion

		#pragma region Private Property
		#pragma endregion

	};

}

#endif