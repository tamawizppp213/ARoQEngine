//////////////////////////////////////////////////////////////////////////////////
///  @file   PMDDefines.hpp
///  @brief  PMD�Ŏg�p�����`���܂Ƃ߂��w�b�_�t�@�C��.
///  @author toide
///  @date   2024/05/11 21:52:31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMD_DEFINES_HPP
#define PMD_DEFINES_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
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
namespace gc::file::pmd::details
{
	/****************************************************************************
	*				  		PMDHeader
	*************************************************************************//**
	/*  @brief   ���f���f�[�^�̃w�b�_���. ���f����, �o�[�W����, �R�����g�Ȃǂ������Ă��܂�. @n
	*            ���f�����͓��{��Ɖp���2��ނ�����܂�. �ő�20����@n
	*            �o�[�W������1.00�Ȃǂ̂悤�ɏ����_�ȉ�2���܂ł̒l������܂�.@n
	*            �R�����g�͓��{��Ɖp���2��ނ�����܂�. �R�����g��256�����܂œ���܂�. @n
	*            �{����string�^�ň����ׂ��ł���, Engine��FileSystem�̎d�l�ɍ��킹��tstring�^���g�p���Ă��܂�.
	*****************************************************************************/
	struct PMDHeader
	{
		/*! @brief �t�@�C�����ʎq (Pmd)*/
		gu::char8   Signature[3]    = {};

		/*! @brief �o�[�W�����ԍ� (1.00�Ȃ�)*/
		gu::float32 Version         = 0;

		/*! @brief ���f����(���{��)*/
		gu::tstring NameJapanese    = SP("");

		/*! @brief ���f����(�p��)*/
		gu::tstring NameEnglish     = SP("");

		/*! @brief ���쌠���̂悤�ȃR�����g(���{��) */
		gu::tstring CommentJapanese = SP("");

		/*! @brief ���쌠���̂悤�ȃR�����g(�p��) */
		gu::tstring CommentEnglish  = SP("");

		/*!**********************************************************************
		*  @brief     �������t�@�C���w�b�_�ɂȂ��Ă��邩 (Pmd�Ƃ����w�b�_���ɂȂ��Ă��邩)
		*  @param[in] void
		*  @return    bool : Pmd�Ƃ����w�b�_���ɂȂ��Ă���ꍇ��true��Ԃ��܂�. 
		*************************************************************************/
		bool IsValid() const;

		/*!**********************************************************************
		*  @brief     �g���ȊO�̃f�[�^(English)��ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
		
		/*!**********************************************************************
		*  @brief     �g���f�[�^(English)��ǂݍ���
		*  @param[in] const gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDVertex
	*************************************************************************//**
	/*  @brief   ���f���f�[�^�̒��_���, �ʒu, �@��, UV, �{�[�����, �G�b�W�̉�����񂪓����Ă��܂�. @n
	*            �ő咸�_����65536�ł�.
	*****************************************************************************/
	struct PMDVertex
	{
		/*! @brief ���_�̈ʒu��� */
		gm::Float3 Position;

		/*! @brief ���_�̖@�� */
		gm::Float3 Normal;

		/*! @brief �e�N�X�`���}�b�s���O�Ɏg�p�����UV���W */
		gm::Float2 UV;

		/*! @brief ���_���e�����󂯂�ߐڃ{�[���ԍ� */
		gu::uint16 BoneIndex[2];

		/*! @brief 2�̃{�[���ɑ΂���e���x����. �l���傫���ق�BoneIndex[0]�ɋ߂�. 0�`100�Ŏw�肵�܂� */
		gu::uint8  BoneWeight;

		/*! @brief �֊s�����s���ł��邩�������t���O */
		gu::uint8  EdgeInvisible;

		/*! @brief �t�@�C���n���h�����g����������*/
		PMDVertex(const gu::SharedPointer<platform::core::file::IFileHandle>& handle)
		{
			Read(handle);
		}

		/*!**********************************************************************
		*  @brief     ���_�f�[�^��ǂݍ��݂܂�.
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDMaterial
	*************************************************************************//**
	/*  @brief   ���f���f�[�^�̃}�e���A����� @n
	*            �}�e���A���͊g�U�F, ����̋���, ����F, ���F, �g�D�[���e�N�X�`���ԍ�, �֊s���t���O, ���_�C���f�b�N�X��, �e�N�X�`����, SphereMap���������Ă��܂�.
	*****************************************************************************/
	struct PMDMaterial
	{
		/*! @brief �g�U�F������RGBA�l. �e�v�f��0.0�`1.0�͈̔͂Œ�`����܂� */
		gm::Float4 Diffuse;

		/*! @brief ����̋���. �͈͐����͓��ɂ���܂���*/
		gu::float32 SpecularPower;

		/*! @brief ����F(RGB). �e�v�f��0.0�`1.0�͈̔͂Œ�`����܂�*/
		gm::Float3 Specular;

		/*! @brief ���F. ���͂̊�����󂯂���̃I�t�Z�b�g�l. (�^�����ɂȂ�̂�h��)*/
		gm::Float3 Ambient;

		/*! @brief �g�D�[���e�N�X�`���̔ԍ�*/
		gu::uint8  ToonTextureID;

		/*! @brief �֊s���t���O (1�ŗL����)*/
		gu::uint8  EdgeFlag;

		/*! @brief ���̃}�e���A���Ɋ֘A�t����ꂽ���_�C���f�b�N�X�̐���\���l*/
		gu::uint32 IndexCount;

		/*! @brief ���̃}�e���A���Ɋ֘A�t�����Ă���e�N�X�`����*/
		gu::tstring TexturePath = SP("");

		/*! @brief ���̃}�e���A���Ɋ֘A�t�����Ă���SphereMap(���˃}�b�v)�̖��O*/
		gu::tstring SphereMapPath = SP("");

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		PMDMaterial() = default;

		/*! @brief �t�@�C���n���h�����g����������*/
		PMDMaterial(const gu::SharedPointer<platform::core::file::IFileHandle>& handle, const gu::tstring& directory) 
		{
			Read(handle, directory);
		}

		/*!**********************************************************************
		*  @brief     �}�e���A����ǂݍ��݂܂�
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const gu::tstring& �f�B���N�g��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const gu::tstring& directory);
	};

	/****************************************************************************
	*				  		PMDBoneType
	*************************************************************************//**
	/*  @brief   ���f���f�[�^�̃{�[���̎��
	*****************************************************************************/
	enum class PMDBoneType : gu::uint8
	{
		Rotation          = 0,  //!< ��]
		RotationAndMove   = 1,  //!< ��]�ړ�
		IK                = 2,  //!< IK
		Unknown           = 3,  //!< �s��
		IKRotation        = 4,  //!< IK�e�����̉�]
		RotationEffectable= 5,  //!< ��]�e����
		IKTarget          = 6,  //!< IK�̐ڑ���
		Hide              = 7,  //!< ��\��
		Twist             = 8,  //!< ����
		RotationMotion    = 9   //!< ��]�^��
	};

	/****************************************************************************
	*				  		PMDBone
	*************************************************************************//**
	/*  @brief   ���f���f�[�^�̃{�[����� @n
	*            �{�[���̖��O, �e�{�[���ԍ�, �q�{�[���ԍ�, �{�[���̎��, IK�{�[���ԍ�, �{�[���̐擪�ʒu�������Ă��܂�.
	*****************************************************************************/
	struct PMDBone
	{
		/*! @brief �{�[���̓��{�ꖼ*/
		gu::tstring NameJapanese = SP("");

		/*! @brief �{�[���̉p�ꖼ*/
		gu::tstring NameEnglish = SP("");

		/*! @brief �{�[���̐e�{�[���ԍ�. �e�����Ȃ��ꍇ��-1*/
		gu::uint16 ParentBoneID = 0xFFFF;

		/*! @brief �{�[���̎q�{�[���ԍ�*/
		gu::uint16 ChildBoneID = 0;

		/*! @brief �{�[���̎��. 0:��], 1:��]�ړ�, 2:IK, 3:�s��, 4:IK�e�����̉�], 5:IK�e�����̉�]�ړ�, 6:��\��, 7:����, 8:��]�^��*/
		PMDBoneType BoneType = PMDBoneType::Unknown;

		/*! @brief IK�{�[���̔ԍ� (�����ꍇ��0)*/
		gu::uint16 IKBoneID = 0;

		/*! @brief �{�[���̐擪�ʒu*/
		gm::Float3 HeadPosition = {};

		/*!**********************************************************************
		*  @brief     �{�[����ǂݍ��݂܂�
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const gu::tstring& �f�B���N�g��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     �g���f�[�^(English)��ǂݍ���
		*  @param[in] const gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDBoneIK
	*************************************************************************//**
	/*  @brief   ���f���f�[�^�̃{�[��IK���@n
	*****************************************************************************/
	struct PMDBoneIK
	{
		/*! @brief ���g��IKBone���擾����ID*/
		gu::uint16 SelfIKBoneID = 0;

		/*! @brief ���̕ϐ��Ɍ�������SelfIKBone��Ԃ�Chain�{�[���𓮂������ƂɂȂ�܂�. */
		gu::uint16 TargetBoneID = 0;

		/*! @brief IK�����s��, �ڕW�̃{�[���ɋ߂Â��邽�߂̃��[�v���s��*/
		gu::uint16 IterationCount = 0;

		/*! @brief �ő�̊p�x����*/
		gu::float32 AngleLimit = 0;

		/*! @brief �Ԃ�IK�{�[���Ɋ܂܂��ID�̃��X�g*/
		gu::DynamicArray<gu::uint16> Chains = gu::DynamicArray<gu::uint16>();

		/*!**********************************************************************
		*  @brief     �{�[��IK��ǂݍ��݂܂�
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const gu::tstring& �f�B���N�g��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		PMDBoneIK() = default;

		/*! @brief �t�@�C���n���h�����g����������*/
		PMDBoneIK(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
		{
			Read(fileHandle);
		}

		/*! @brief �f�X�g���N�^*/
		~PMDBoneIK()
		{
			Chains.Clear(); Chains.ShrinkToFit();
		}
	};

	/****************************************************************************
	*				  		PMDFacePart
	*************************************************************************//**
	/*  @brief  ��̃p�[�c�̎�ނ��w�肵�܂�.
	*****************************************************************************/
	enum class PMDFacePart : gu::uint8
	{
		Base    = 0, //!< ��{
		EyeBrow = 1, //!< ��
		Eye     = 2, //!< ��
		Lip     = 3, //!< ��
		Other   = 4  //!< ���̑�
	};

	/****************************************************************************
	*				  		PMDFaceExpression
	*************************************************************************//**
	/*  @brief  �\��ɂ�钸�_���[�t�B���O�����i�[���܂�. @n
	*           �\�, �\��̃p�[�c���, ���_�����̈ʒu���X�g, ���_�̃C���f�b�N�X���X�g�������Ă��܂�.
	*****************************************************************************/
	struct PMDFaceExpression
	{
		/*! @brief �\� (���{��)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief �\� (�p��)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief �\��̃p�[�c���*/
		PMDFacePart Part = PMDFacePart::Base;

		/*! @brief �\��̒��_�����̈ʒu���X�g*/
		gu::DynamicArray<gm::Float3> Vertices = {};

		/*! @brief ���_�̃C���f�b�N�X���X�g*/
		gu::DynamicArray<gu::uint32> Indices = {};

		/*!**********************************************************************
		*  @brief     �\���ǂݍ��݂܂�
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     �g���f�[�^(English)��ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		PMDFaceExpression() = default;

		/*! @brief �f�X�g���N�^*/
		~PMDFaceExpression()
		{
			Vertices.Clear(); Vertices.ShrinkToFit();
			Indices .Clear(); Indices .ShrinkToFit();
		}
	};

	/****************************************************************************
	*				  		PMDBoneDisplayName
	*************************************************************************//**
	/*  @brief  �{�[���̕\�������擾���܂�.
	*****************************************************************************/
	struct PMDBoneDisplayName
	{
		/*! @brief �\� (���{��)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief �\� (�p��)*/
		gu::tstring NameEnglish  = SP("");

		/*!**********************************************************************
		*  @brief     �{�[���̕\������ǂݍ��݂܂�
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     �g���f�[�^(English)��ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDBoneNameTable
	*************************************************************************//**
	/*  @brief  �{�[���̎����ƕ\�����̑Ή�����邽�߂̃C���f�b�N�X��ۑ����܂�. 
	*****************************************************************************/
	struct PMDBoneNameTable
	{
		/*! @brief ���{�[�����w�肷��C���f�b�N�X*/
		gu::uint16 BoneID = 0; 

		/*! @brief �\�������w�肷��C���f�b�N�X*/
		gu::uint8 DisplayID = 0;

		/*!**********************************************************************
		*  @brief     �{�[���̑Ή��\��ǂݍ��݂܂�
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

	};

	/****************************************************************************
	*				  		PMDRigidBodyShape
	*************************************************************************//**
	/*  @brief  ���̂̌`����w�肵�܂�. ��{�I�Ȍ`��͋�, ��, �J�v�Z����3��ނł�.
	*****************************************************************************/
	enum class PMDRigidBodyShape : gu::uint8
	{
		Sphere  = 0, //!< ��
		Box     = 1, //!< ��
		Capsule = 2  //!< �J�v�Z��
	};

	/****************************************************************************
	*				  		PMDRigidBodyType
	*************************************************************************//**
	/*  @brief  ���̂̎�ނ��w�肵�܂�
	*****************************************************************************/
	enum class PMDRigidBodyType : gu::uint8
	{
		BoneConnected  = 0, //!< �{�[���ɐڑ�
		Physics        = 1, //!< �������Z
		PhysicsAndBone = 2  //!< �������Z�ƃ{�[���ɐڑ�
	};

	/****************************************************************************
	*				  		PMDRigidBody
	*************************************************************************//**
	/*  @brief  ���̂̐ݒ��ۑ����܂� @n
	*****************************************************************************/
	struct PMDRigidBody
	{
		/*! @brief ���̖̂��O*/
		gu::tstring Name = SP("");

		/*! @brief ���̂̊֘A�{�[���ԍ�*/
		gu::uint16 BoneID = 0;

		/*! @brief ���̂̃O���[�v�ԍ�*/
		gu::uint8 GroupID = 0;

		/*! @brief ���̂̃}�X�N�ԍ�*/
		gu::uint16 MaskID = 0;

		/*! @brief ���̂̌`��*/
		PMDRigidBodyShape Shape = PMDRigidBodyShape::Sphere;

		/*! @brief ���̂̃T�C�Y*/
		gm::Float3 Size = {};

		/*! @brief ���̂̈ʒu*/
		gm::Float3 Position = {};

		/*! @brief ���̂̕��� (rad)*/
		gm::Float3 Orientation = {};

		/*! @brief ���̂̎���*/
		gu::float32 Mass = 0;

		/*! @brief ���̂̈ړ�����*/
		gu::float32 DampingTranslate = 0;

		/*! @brief ���̂̉�]����*/
		gu::float32 DampingAngular = 0;

		/*! @brief ���̂̔�����*/
		gu::float32 Restitution = 0;

		/*! @brief ���̖̂��C��*/
		gu::float32 Friction = 0;

		/*! @brief ���̂̎��*/
		PMDRigidBodyType Type = PMDRigidBodyType::BoneConnected;

		/*!**********************************************************************
		*  @brief     ���̂̐ݒ��ǂݍ��݂܂�
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDJoint
	*************************************************************************//**
	/*  @brief  �֐߂̐ݒ��ۑ����܂�
	*****************************************************************************/
	struct PMDJoint
	{
		/*! @brief �֐߂̖��O*/
		gu::tstring Name = SP("");

		/*! @brief ����A*/
		gu::uint32 RigidBodyA = 0;

		/*! @brief ����B*/
		gu::uint32 RigidBodyB = 0;

		/*! @brief �ʒu*/
		gm::Float3 Position = {};

		/*! @brief ��] (rad)*/
		gm::Float3 Orientation = {};

		/*! @brief �ړ����� (�ŏ��l)*/
		gm::Float3 TranslationLimitMin = {};

		/*! @brief �ړ����� (�ő�l)*/
		gm::Float3 TranslationLimitMax = {};

		/*! @brief ��]���� (�ŏ��l)*/
		gm::Float3 OrientationLimitMin = {};

		/*! @brief ��]���� (�ő�l)*/
		gm::Float3 OrientationLimitMax = {};

		/*! @brief �ړ��̂΂˒萔*/
		gm::Float3 SpringTranslation = {};

		/*! @brief ��]�̂΂˒萔*/
		gm::Float3 SpringOrientation = {};

		/*!**********************************************************************
		*  @brief     �֐߂̐ݒ��ǂݍ��݂܂�
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};
}


#endif