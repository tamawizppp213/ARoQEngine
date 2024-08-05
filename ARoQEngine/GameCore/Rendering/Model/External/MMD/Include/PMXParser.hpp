//////////////////////////////////////////////////////////////////////////////////
///  @file   PMXParser.hpp
///  @brief  PMX�t�@�C����ǂݍ��ރN���X
///  @author Toide Yutaro
///  @date   2024_06_02
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMX_PARSER_HPP
#define PMX_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Private/Include/PMXDefines.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::pmx
{
	
	/****************************************************************************
	*				  			PMXFile
	****************************************************************************/
	/*  @brief  PMX�̃t�@�C����ǂݍ��ރN���X�ł�.
	*****************************************************************************/
	struct PMXFile : public gu::NonCopyable
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     PMX�t�@�C����ǂݍ��ފ֐�
		*  @param[in] const gu::tstring& �t�@�C���p�X
		*  @param[in] const bool �񓯊��œǂݍ��ނ��ǂ���
		*  @return    bool
		*************************************************************************/
		bool Read(const gu::tstring& filePath, const bool useAsync = false);

		#pragma endregion

		#pragma region Public Property
		/*! @brief ���f���f�[�^�̃w�b�_���, �t�@�C���̃o�[�W���������i�[���܂�.*/
		PMXHeader Header = {};

		/*! @brief ���f���̒��_���*/
		gu::DynamicArray<PMXVertex> Vertices = {};

		/*! @brief ���f���̃C���f�b�N�X���*/
		gu::DynamicArray<gu::uint32> Indices = {};

		/*! @brief ���f���̃}�e���A�����*/
		gu::DynamicArray<PMXMaterial> Materials = {};

		/*! @brief ���f���̃{�[�����*/
		gu::DynamicArray<PMXBone> Bones = {};

		/*! @brief ���f���̃��[�t���*/
		gu::DynamicArray<PMXMorph> Morphs = {};

		/*! @brief ���f���̕\���g���*/
		gu::DynamicArray<PMXDisplayFrame> DisplayFrames = {};

		/*! @brief ���f���̍��̏��*/
		gu::DynamicArray<PMXRigidBody> RigidBodies = {};

		/*! @brief ���f���̃W���C���g���*/
		gu::DynamicArray<PMXJoint> Joints = {};

		/*! @brief ���f���̃\�t�g�{�f�B���*/
		gu::DynamicArray<PMXSoftBody> SoftBodies = {};

		/*! @brief ���f���̃e�N�X�`�����X�g*/
		gu::DynamicArray<gu::tstring> TexturePathList = {};

		/*! @brief PMX�t�@�C�����i�[����Ă���e�f�B���N�g��*/
		gu::tstring Directory = SP("");
		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		PMXFile() = default;

		/*! @brief �f�X�g���N�^*/
		~PMXFile();

		#pragma endregion

	private:
		#pragma region Private Function
		#pragma endregion

		#pragma region Private Property
		#pragma endregion
	};

}

#endif