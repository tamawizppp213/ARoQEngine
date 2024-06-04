//////////////////////////////////////////////////////////////////////////////////
///  @file   PMDParser.hpp
///  @brief  PMD File Parser
///  @author Toide Yutaro
///  @date   2024_05_29
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMD_PARSER_HPP
#define PMD_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Private/Include/PMDDefines.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace platform::core::file
{
	class IFileHandle;
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::file::pmd
{
	/****************************************************************************
	*				  			PMDFile
	*************************************************************************//**
	/*  @brief  PMD�̃t�@�C����ǂݍ��ރN���X�ł�. 
	*****************************************************************************/
	struct PMDFile : public gu::NonCopyable
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     PMD�t�@�C����ǂݍ��ފ֐�
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		bool Read(const gu::tstring& filePath);

		#pragma endregion

		#pragma region Public Property
		/*! @brief ���f���f�[�^�̃w�b�_���. ���f����, �o�[�W����, �R�����g�Ȃǂ������Ă��܂�*/
		details::PMDHeader Header = {};

		/*! @brief ���f���̒��_��� (���_�̍ő吔��65536�ł�)*/
		gu::DynamicArray<details::PMDVertex> Vertices = {};

		/*! @brief ���f���̃C���f�b�N�X��� (���_���̐���ɂ��, uint16�ƂȂ��Ă��܂�.) */
		gu::DynamicArray<gu::uint16> Indices = {};

		/*! @brief ���f���̃}�e���A����� */
		gu::DynamicArray<details::PMDMaterial> Materials = {};

		/*! @brief �{�[���̖��O, �e�{�[���ԍ�, �q�{�[���ԍ�, �{�[���̎��, IK�{�[���ԍ�, �{�[���̐擪�ʒu�������Ă��܂�.*/
		gu::DynamicArray<details::PMDBone> Bones = {};

		/*! @brief IK�{�[���̏�� */
		gu::DynamicArray<details::PMDBoneIK> IKs = {};

		/*! @brief �\��̖��O, �\��̒��_���, �\��̎�ނ������Ă��܂�.*/
		gu::DynamicArray<details::PMDFaceExpression>  FaceExpressions = {};

		/*! @brief �����̕\����݂���ꍇ�ɂ��̎�ނ��w�肷��C���f�b�N�X�ł�*/
		gu::DynamicArray<gu::uint16> FaceLabelIndices = {};

		/*! @brief �{�[���̕\���� */
		gu::DynamicArray<details::PMDBoneDisplayName> BoneDisplayNameList = {};

		/*! @brief �{�[���̕\�����X�g */
		gu::DynamicArray<details::PMDBoneNameTable> BoneNameTable = {};

		/*! @brief ����*/
		gu::DynamicArray<details::PMDRigidBody> RigidBodies = {};

		/*! @brief �֐�*/
		gu::DynamicArray<details::PMDJoint> Joints = {};

		/*! @brief �g�D�[���e�N�X�`�����g�p����ꍇ�̃p�X���i�[����Ă��郊�X�g*/
		gu::DynamicArray<gu::tstring> ToonTextureList = {};

		/*! @brief PMD�t�@�C�����i�[����Ă���e�f�B���N�g��*/
		gu::tstring Directory = SP("");

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^ */
		PMDFile() = default;

		/*! @brief �f�X�g���N�^ */
		~PMDFile();

		#pragma endregion

	private:
		#pragma region Private Function
		void ReadLocalizeData       (const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
		void ReadToonTextures       (const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
		void ReadPhysics            (const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
		#pragma endregion 
		
		#pragma region Private Property
		#pragma endregion
	};

}
#endif