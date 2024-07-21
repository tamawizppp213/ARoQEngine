//////////////////////////////////////////////////////////////////////////////////
///  @file   PMXDefines.hpp
///  @brief  temp
///  @author toide
///  @date   2024/05/31 0:33:40
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMX_DEFINES_HPP
#define PMX_DEFINES_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Base/Include/GUEnumClassFlags.hpp"

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

namespace engine::file::pmx
{
	/****************************************************************************
	*				  			PMXEncode
	****************************************************************************/
	/*  @brief  �����R�[�h�̃G���R�[�f�B���O�̎�ނ��w�肵�܂�. 
	*****************************************************************************/
	enum class PMXEncode : gu::uint8
	{
		UTF16 = 0,
		UTF8  = 1
	};

	/****************************************************************************
	*				  		PMXHeader
	****************************************************************************/
	/*  @brief   ���f���f�[�^�̃w�b�_���, �t�@�C���̃o�[�W���������i�[���܂�.
	*****************************************************************************/
	struct PMXHeader
	{
		/*! @brief �t�@�C�����ʎq (PMX �Ƃ����w�b�_���ɂȂ��Ă��邩). ASCII��0x50,0x4d,0x58,0x20*/
		char Signature[4] = {};

		/*! @brief �o�[�W�����ԍ� (2.0 �� 2.1 ��). */
		gu::float32 Version = 0.0f;

		/*! @brief �㑱�̃f�[�^��(�w�b�_���)�̃o�C�g�T�C�Y*/
		gu::uint8 SubsequentDataSize = 0;

		/*! @brief �G���R�[�h���� (0:UTF16, 1:UTF8) */
		PMXEncode Encode = PMXEncode::UTF16;

		/*! @brief �ǉ���UV�� (0�`4�܂ł͈̔�)*/
		gu::uint8 AdditionalUVCount = 0;

		/*! @brief ���_�C���f�b�N�X�T�C�Y (1, 2, 4�̂����ꂩ)*/
		gu::uint8 VertexIndexSize = 0;

		/*! @brief �e�N�X�`���C���f�b�N�X�T�C�Y (1, 2, 4�̂����ꂩ)*/
		gu::uint8 TextureIndexSize = 0;

		/*! @brief �}�e���A���C���f�b�N�X�T�C�Y (1, 2, 4�̂����ꂩ)*/
		gu::uint8 MaterialIndexSize = 0;

		/*! @brief �{�[���C���f�b�N�X�T�C�Y (1, 2, 4�̂����ꂩ)*/
		gu::uint8 BoneIndexSize = 0;

		/*! @brief ���[�t�C���f�b�N�X�T�C�Y (1, 2, 4�̂����ꂩ)*/
		gu::uint8 MorphIndexSize = 0;

		/*! @brief ���̃C���f�b�N�X�T�C�Y (1, 2, 4�̂����ꂩ)*/
		gu::uint8 RigidBodyIndexSize = 0;

		/*! @brief OtherData*/
		gu::DynamicArray<gu::uint8> OtherData = {};

		/*! @brief ���f����(���{��)*/
		gu::tstring NameJapanese    = SP("");

		/*! @brief ���f����(�p��)*/
		gu::tstring NameEnglish     = SP("");

		/*! @brief ���쌠���̂悤�ȃR�����g(���{��) */
		gu::tstring CommentJapanese = SP("");

		/*! @brief ���쌠���̂悤�ȃR�����g(�p��) */
		gu::tstring CommentEnglish  = SP("");

		/*!**********************************************************************
		*  @brief     �������t�@�C���w�b�_�ɂȂ��Ă��邩 (Pmx �Ƃ����w�b�_���ɂȂ��Ă��邩)
		*  @param[in] void
		*  @return    bool : Pmx �Ƃ����w�b�_���ɂȂ��Ă���ꍇ��true��Ԃ��܂�.
		*************************************************************************/
		bool IsValid() const;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  			PMXVertexWeightType
	****************************************************************************/
	/*  @brief  �{�[���E�F�C�g�̊i�[���ɂ��Ē�`���܂�. 
	*****************************************************************************/
	enum class PMXBoneWeightType : gu::uint8
	{
		BDEF1 = 0, //!< �{�[���̂�
		BDEF2 = 1, //!< �{�[��2�ƃ{�[��1�̃E�F�C�g�l (PMD����)
		BDEF4 = 2, //!< �{�[��4�Ƃ��ꂼ��̃E�F�C�g�l. �E�F�C�g���v��1.0�ł���Ƃ���ۏ؂͖���
		SDEF  = 3, //!< BDEF2�Ɠ�������, SDEF�p��float3��3�ǉ�����Ă���. ���ۂ̌v�Z�ł͕␳�l�̎Z�o���K�v
		QDEF  = 4, //!< boneIndices[0-3] + boneWeights[0-3])
	};

	/****************************************************************************
	*				  		PMXVertex
	****************************************************************************/
	/*  @brief   PMX�t�@�C���̒��_�����i�[���܂�.
	*****************************************************************************/
	struct PMXVertex
	{
		/*! @brief ���_�̈ʒu��� */
		gm::Float3 Position = {};

		/*! @brief ���_�̖@�� */
		gm::Float3 Normal = {};

		/*! @brief �e�N�X�`���}�b�s���O�Ɏg�p�����UV���W */
		gm::Float2 UV = {};

		/*! @brief �ǉ���UV���W */
		gm::Float4 AdditionalUV[4] = {};

		/*! @brief ���_�̃E�F�C�g�^�C�v */
		PMXBoneWeightType WeightType = PMXBoneWeightType::BDEF1;

		/*! @brief �{�[���C���f�b�N�X */
		gu::int32 BoneIndex[4] = {};

		/*! @brief �{�[���E�F�C�g�l */
		gu::float32 BoneWeight[4] = {};

		/*! @brief �G�b�W�{�� */
		gu::float32 EdgeScale = 0.0f;

		/*! @brief SDEF�Ŏg�p����萔�l*/
		gm::Float3 SDEF_C = 0.0f;

		/*! @brief SDEF�Ŏg�p����R0*/
		gm::Float3 SDEF_R0 = {};

		/*! @brief SDEF�Ŏg�p����R1*/
		gm::Float3 SDEF_R1 = {};

		/*! @brief �t�@�C���n���h�����g����������*/
		PMXVertex(const gu::SharedPointer<platform::core::file::IFileHandle>& handle, const PMXHeader& header)
		{
			Read(handle, header);
		}

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		 PMXDrawModeFlags
	****************************************************************************/
	/*  @brief  DrawMode�̃t���O���`���܂�.
	*****************************************************************************/
	enum class PMXDrawModeFlags : gu::uint8
	{
		None           = 0x00, //!< �t���O����
		BothSurface    = 0x01, //!< ���ʕ`��
		GroundShadow   = 0x02, //!< �n�ʉe
		CastSelfShadow = 0x04, //!< �Z���t�V���h�E�}�b�v�ւ̕`��
		ReceiveShadow  = 0x08, //!< �Z���t�V���h�E�}�b�v���󂯎���ăL�������g�ɕ`�悷��
		Edge           = 0x10, //!< �G�b�W�`��
		VetexColor     = 0x20, //!< ���_�J���[ (2.1�g��)
		Point          = 0x40, //!< �|�C���g�`��, �_�Q�`�� (2.1�g��)
		Line           = 0x80, //!< ���`�� (2.1�g��)
	};
	ENUM_CLASS_FLAGS(PMXDrawModeFlags);

	/****************************************************************************
	*				  		 PMXSphereMapOperator
	****************************************************************************/
	/*  @brief  �X�t�B�A�}�b�v�\�����̌v�Z���@���w�肵�܂�. 
	*****************************************************************************/
	enum class PMXSphereMapOperator : gu::uint8
	{
		Invalid    = 0, //!< ����
		Multiply   = 1, //!< ��Z
		Add        = 2, //!< ���Z
		SubTexture = 3, //!< �T�u�e�N�X�`��
	};

	/****************************************************************************
	*				  		 PMXToonTextureMode
	****************************************************************************/
	/*  @brief  �g�D�[���e�N�X�`���̎�ނ��w�肵�܂�.
	*****************************************************************************/
	enum class PMXToonTextureMode : gu::uint8
	{
		Separate = 0, //!< �ʃe�N�X�`��
		Common   = 1, //!< ���L�e�N�X�`��
	};

	/****************************************************************************
	*				  		PMXMaterial
	****************************************************************************/
	/*  @brief   PMX�t�@�C���̃}�e���A�������i�[���܂�.
	*****************************************************************************/
	struct PMXMaterial
	{
		/*! @brief �}�e���A����(���{��)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief �}�e���A����(�p��)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief ����*/
		gu::tstring Memo = SP("");

		/*! @brief �}�e���A���̊g�U�F */
		gm::Float4 Diffuse = {};

		/*! @brief �}�e���A���̔��ːF */
		gm::Float3 Specular = {};

		/*! @brief �}�e���A���̔��ˋ��x */
		gu::float32 SpecularPower = 0.0f;

		/*! @brief �}�e���A���̊��F */
		gm::Float3 Ambient = {};

		/*! @brief �}�e���A���̕`��t���O */
		PMXDrawModeFlags DrawMode = PMXDrawModeFlags::None;

		/*! @brief �}�e���A���̃G�b�W�F */
		gm::Float4 EdgeColor = {};

		/*! @brief �}�e���A���̃G�b�W�T�C�Y */
		gu::float32 EdgeSize = 0.0f;

		/*! @brief �}�e���A���̃e�N�X�`���C���f�b�N�X */
		gu::int32 DiffuseTextureIndex = 0;

		/*! @brief �}�e���A���̃X�t�B�A�}�b�v�e�N�X�`���C���f�b�N�X */
		gu::int32 SphereMapTextureIndex = 0;

		/*! @brief �X�t�B�A�}�b�v�̌v�Z���@ */
		PMXSphereMapOperator SphereMapMode = PMXSphereMapOperator::Invalid;

		/*! @brief �g�D�[���e�N�X�`���̎�� */
		PMXToonTextureMode ToonTextureMode = PMXToonTextureMode::Separate;

		/*! @brief �g�D�[���e�N�X�`���̃C���f�b�N�X */
		gu::int32 ToonTextureIndex = 0;

		/*! @brief �ʂ̌� (�K��3�̔{���ɂȂ�܂�)*/
		gu::int32 FaceCount = 0;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);

	};

	/****************************************************************************
	*				  		PMXIKLink
	****************************************************************************/
	/*  @brief   IK�����N�����i�[���܂�.
	*****************************************************************************/
	struct PMXIKLink
	{
		/*! @brief IK�����N�̃{�[���C���f�b�N�X */
		gu::int32 TargetBoneIndex = 0;

		/*! @brief IK�����N�̗L���� */
		gu::uint8 UseAngleLimit = 0;

		/*! @brief IK�����N�̊p�x�����̉��� */
		gm::Float3 AngleLimitLower = {};

		/*! @brief IK�����N�̊p�x�����̏�� */
		gm::Float3 AngleLimitUpper = {};

		PMXIKLink() = default;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXBoneFlags
	****************************************************************************/
	/*  @brief   Bone�̃t���O���`���܂�.
	*****************************************************************************/
	enum class PMXBoneFlags : gu::uint16
	{
		None               = 0x0000, //!< �t���O����
		Connect            = 0x0001, //!< �ڑ���{�[������i0�F���W�ŃI�t�Z�b�g�w��A1�F�{�[���w��j
		AllowRotation      = 0x0002, //!< ��]�\
		AllowTranslate     = 0x0004, //!< �ړ��\
		Visible            = 0x0008, //!< �\��
		AllowControl       = 0x0010, //!< ����\
		IK                 = 0x0020, //!< IK
		AppendLocal        = 0x0080, //!< ���[�J���t�^ | �t�^�Ώ� 0:���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1:�e�̃��[�J���ό`��
		AppendRotate       = 0x0100, //!< ��]�t�^
		AppendTranslate    = 0x0200, //!< �ړ��t�^
		FixedAxis          = 0x0400, //!< �Œ莲
		LocalAxis          = 0x0800, //!< ���[�J����
		DeformAfterPhysics = 0x1000, //!< ������ό`
		DeformOuterParent  = 0x2000, //!< �O���e�ό`
	};

	ENUM_CLASS_FLAGS(PMXBoneFlags);

	/****************************************************************************
	*				  		PMXBone
	****************************************************************************/
	/*  @brief   �{�[�������i�[���܂�.
	*****************************************************************************/
	struct PMXBone
	{
		/*! @brief �{�[����(���{��)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief �{�[����(�p��)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief �{�[���̈ʒu */
		gm::Float3 Position = {};

		/*! @brief �e�{�[���̃C���f�b�N�X */
		gu::int32 ParentBoneIndex = 0;

		/*! @brief �{�[���̕ό`�K�w */
		gu::int32 DeformDepth = 0;

		/*! @brief �{�[���̕ό`�t���O */
		PMXBoneFlags BoneFlag = PMXBoneFlags::None;

		/*! @brief �ʒu�̃I�t�Z�b�g*/
		gm::Float3 PositionOffset = {};

		/*! @brief �{�[���̐ڑ��� */
		gu::int32 TargetBoneIndex = 0;

		/*! @brief �t�^�e�{�[���̃C���f�b�N�X*/
		gu::int32 AppendBoneIndex = 0;

		/*! @brief �t�^�e�{�[���̃E�F�C�g */
		gu::float32 AppendWeight = 0;

		/*! @brief �{�[���̉�]�\�� (PMXBoneFlag: FixedAxis���L���ȏꍇ�Ɏg�p�\)*/
		gm::Float3 FixedRotationAxis = {};

		/*! @brief �{�[���̃��[�J��X�� (PMXBoneFlag: LocalAxis���L���ȏꍇ�Ɏg�p�\)*/
		gm::Float3 LocalXAxis = {};

		/*! @brief �{�[���̃��[�J��Z�� (PMXBoneFlag: LocalAxis���L���ȏꍇ�Ɏg�p�\)*/
		gm::Float3 LocalZAxis = {};

		/*! @brief �L�[ (PMXBoneFlag: DeformOuterParent���L���ȏꍇ�Ɏg�p�\)*/
		gu::int32 Key = 0;

		/*! @brief IK���g�p����Ƃ��̐ڑ���̃{�[���̃C���f�b�N�X(PMXBoneFlags: IKBone���L���ȏꍇ�Ɏg�p�\) */
		gu::int32 IKTargetBoneIndex = 0;

		/*! @brief IK�̎��s��(PMXBoneFlags: IKBone���L���ȏꍇ�Ɏg�p�\) : �ő��255��܂ł�����Ƃ���*/
		gu::int32 IKIteration = 0;

		/*! @brief IK�̈��̎��s������̊p�x����. ���W�A���p�ł���, PMD��IK�l�Ƃ�4�{�قȂ�̂Œ���(PMXBoneFlags: IKBone���L���ȏꍇ�Ɏg�p�\) */
		gu::float32 IKLoopAngleLimit = 0.0f;

		/*! @brief IK�����N*/
		gu::DynamicArray<PMXIKLink> IKLinks = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXMorphType
	****************************************************************************/
	/*  @brief   �\��[�t�̎�ނ��w�肵�܂�.
	*****************************************************************************/
	enum class PMXMorphType : gu::uint8
	{
		Group         = 0,  //!< �O���[�v
		Vertex        = 1,  //!< ���_
		Bone          = 2,  //!< �{�[��
		UV            = 3,  //!< UV
		AdditionalUV1 = 4,  //!< �ǉ�UV1
		AdditionalUV2 = 5,  //!< �ǉ�UV2
		AdditionalUV3 = 6,  //!< �ǉ�UV3
		AdditionalUV4 = 7,  //!< �ǉ�UV4
		Material      = 8,  //!< �}�e���A��
		Flip          = 9,  //!< �t���b�v
		Impulse       = 10, //!< �C���p���X
	};

	/****************************************************************************
	*				  		PMXFacePart
	****************************************************************************/
	/*  @brief  �\��̕��ʂ��w�肵�܂�.
	*****************************************************************************/
	enum class PMXFacePart : gu::uint8
	{
		Base    = 0, //!< ��{
		Eye     = 1, //!< ��
		EyeBrow = 2, //!< ��
		Mouth   = 3, //!< ��
		Other   = 4, //!< ���̑�
	};

	/****************************************************************************
	*				  		PMXPositionMorph
	****************************************************************************/
	/*  @brief ���W�̃I�t�Z�b�g���w�肷�郂�[�t�ł�.
	*****************************************************************************/
	struct PMXPositionMorph
	{
		/*! @brief �Ώۂ̒��_�C���f�b�N�X */
		gu::int32 VertexIndex = 0;

		/*! @brief �ړ��� */
		gm::Float3 Offset = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXUVMorph
	****************************************************************************/
	/*  @brief UV���W�̃I�t�Z�b�g���w�肷�郂�[�t�ł�.
	*****************************************************************************/
	struct PMXUVMorph
	{
		/*! @brief �Ώۂ̒��_�C���f�b�N�X */
		gu::int32 VertexIndex = 0;

		/*! @brief UV�l�̈ړ��� */
		gm::Float4 Offset = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXBoneMorph
	****************************************************************************/
	/*  @brief �{�[���̈ʒu�Ɖ�]�ʂ𒲐����郂�[�t�ł�.
	*****************************************************************************/
	struct PMXBoneMorph
	{
		/*! @brief �Ώۂ̃{�[���C���f�b�N�X */
		gu::int32 BoneIndex = 0;

		/*! @brief �ړ��� */
		gm::Float3 PositionOffset = {};

		/*! @brief ��]�� */
		gm::Float4 RotationOffset = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXMaterialOperationType
	****************************************************************************/
	/*  @brief�@�}�e���A���̉��Z��ʂ��w�肵�܂�
	*****************************************************************************/
	enum class PMXMaterialOperationType : gu::uint8
	{
		Mul = 0, //!< ��Z
		Add = 1, //!< ���Z
	};

	/****************************************************************************
	*				  		PMXMaterialMorph
	****************************************************************************/
	/*  @brief�@�}�e���A���̐F�𒲐����郂�[�t�ł�.
	*****************************************************************************/
	struct PMXMaterialMorph
	{
		/*! @brief �Ώۂ̃}�e���A���C���f�b�N�X */
		gu::int32 MaterialIndex = 0;

		/*! @brief �}�e���A���̑���t���O */
		PMXMaterialOperationType OperationType = PMXMaterialOperationType::Mul;

		/*! @brief �}�e���A���̊g�U�F */
		gm::Float4 Diffuse = {};

		/*! @brief �}�e���A���̔��ːF */
		gm::Float3 Specular = {};

		/*! @brief �}�e���A���̔��ˋ��x */
		gu::float32 SpecularPower = 0.0f;

		/*! @brief �}�e���A���̊��F */
		gm::Float3 Ambient = {};

		/*! @brief �}�e���A���̃G�b�W�F */
		gm::Float4 EdgeColor = {};

		/*! @brief �}�e���A���̃G�b�W�T�C�Y */
		gu::float32 EdgeSize = 0.0f;

		/*! @brief �e�N�X�`���W��*/
		gm::Float4 TextureCoefficient = {};

		/*! @brief �X�t�B�A�}�b�v�W��*/
		gm::Float4 SphereMapCoefficient = {};

		/*! @brief �g�D�[���e�N�X�`���W��*/
		gm::Float4 ToonTextureCoefficient = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXGroupMorph
	****************************************************************************/
	/*  @brief �O���[�v���[�t
	*****************************************************************************/
	struct PMXGroupMorph
	{
		/*! @brief �Ώۂ̃O���[�v�C���f�b�N�X */
		gu::int32 GroupIndex = 0;

		/*! @brief �O���[�v�̈ړ��� */
		gu::float32 Weight = 0.0f;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXFlipMorph
	****************************************************************************/
	/*  @brief �t���b�v���[�t�͕����o�^���ꂽ���[�t�̂���, ��������L���ɂȂ郂�[�t�ł�.
	*****************************************************************************/
	struct PMXFlipMorph
	{
		/*! @brief �Ώۂ̃��[�t�C���f�b�N�X */
		gu::int32 MorphIndex = 0;

		/*! @brief ���[�t�̈ړ��� */
		gu::float32 Weight = 0.0f;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXImpulseMorph
	****************************************************************************/
	/*  @brief �C���p���X���[�t (�������Z�ɂ�郂�[�t) �ł�.
	*****************************************************************************/
	struct PMXImpulseMorph
	{
		/*! @brief �Ώۂ̃{�[���C���f�b�N�X */
		gu::int32 RigidBodyIndex = 0;

		/*! @brief ���[�J���t���O*/
		gu::uint8 LocalFlag = 0;

		/*! @brief �ړ����x*/
		gm::Float3 TranslateVelocity = {};

		/*! @brief ��]���x*/
		gm::Float3 RotateTorque = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXMorph
	****************************************************************************/
	/*  @brief �\����̂悤��, �f�t�H���g�̃��f������ό`�������郂�[�t�ł�.
	*****************************************************************************/
	struct PMXMorph
	{
		/*! @brief ���[�t��(���{��)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief ���[�t��(�p��)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief ���[�t�̕��� */
		PMXFacePart FacePart = PMXFacePart::Base;

		/*! @brief ���[�t�̎�� */
		PMXMorphType MorphType = PMXMorphType::Group;

		/*! @brief �ʒu���W�̃I�t�Z�b�g���w�肷�郂�[�t */
		gu::DynamicArray<PMXPositionMorph> PositionMorphs = {};

		/*! @brief UV���W�̃I�t�Z�b�g���w�肷�郂�[�t */
		gu::DynamicArray<PMXUVMorph> UVMorphs = {};

		/*! @brief ���[�t�̃I�t�Z�b�g */
		gu::DynamicArray<PMXBoneMorph> BoneMorphs = {};

		/*! @brief ���[�t�̃I�t�Z�b�g */
		gu::DynamicArray<PMXMaterialMorph> MaterialMorphs = {};

		/*! @brief ���[�t�̃I�t�Z�b�g */
		gu::DynamicArray<PMXGroupMorph> GroupMorphs = {};

		/*! @brief ���[�t�̃I�t�Z�b�g */
		gu::DynamicArray<PMXFlipMorph> FlipMorphs = {};

		/*! @brief ���[�t�̃I�t�Z�b�g */
		gu::DynamicArray<PMXImpulseMorph> ImpulseMorphs = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);

		~PMXMorph()
		{
			PositionMorphs.Clear(); PositionMorphs.ShrinkToFit();
			UVMorphs      .Clear(); UVMorphs      .ShrinkToFit();
			BoneMorphs    .Clear(); BoneMorphs    .ShrinkToFit();
			MaterialMorphs.Clear(); MaterialMorphs.ShrinkToFit();
			GroupMorphs   .Clear(); GroupMorphs   .ShrinkToFit();
			FlipMorphs    .Clear(); FlipMorphs    .ShrinkToFit();
			ImpulseMorphs .Clear(); ImpulseMorphs .ShrinkToFit();

		}
	};

	/****************************************************************************
	*				  		PMXDisplayFrameFlag
	****************************************************************************/
	/*  @brief �ʏ�g������g�� : ����g�Ƃ�, Root�p��PMD�݊��p�̘g�ł�.
	*****************************************************************************/
	enum class PMXDisplayFrameFlag : gu::uint8
	{
		Default = 0, //!< �ʏ�g
		Special = 1, //!< ����g
	};

	enum class PMXDisplayFrameType : gu::uint8
	{
		Bone  = 0, //!< �{�[��
		Morph = 1, //!< ���[�t
	};

	/****************************************************************************
	*				  		PMXDisplayFrameElement
	****************************************************************************/
	/*  @brief �@�\���g�̗v�f���w�肵�܂�.
	*****************************************************************************/
	struct PMXDisplayFrameElement 
	{
		/*! @brief �\���g�̎�� */
		PMXDisplayFrameType DisplayFrameType = PMXDisplayFrameType::Bone;

		/*! @brief �\���g�̃C���f�b�N�X */
		gu::int32 Index = 0;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXDisplayFrame
	****************************************************************************/
	/*  @brief �{�[�� / ���[�t�̕\���g���`���܂�.
	*****************************************************************************/
	struct PMXDisplayFrame
	{
		/*! @brief �\���g��(���{��)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief �\���g��(�p��)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief �\���g�̓���g��*/
		PMXDisplayFrameFlag FrameFlag = PMXDisplayFrameFlag::Default;

		/*! @brief �\���g�̗v�f*/
		gu::DynamicArray<PMXDisplayFrameElement> Elements = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);

		~PMXDisplayFrame()
		{
			Elements.Clear(); Elements.ShrinkToFit();
		}
	};

	/****************************************************************************
	*				  		PMXRigidBodyShape
	****************************************************************************/
	/*  @brief ���̂̌`��
	*****************************************************************************/
	enum class PMXRigidBodyShape : gu::uint8
	{
		Sphere  = 0, //!< ��
		Box     = 1, //!< ��
		Capsule = 2, //!< �J�v�Z��
	};

	/****************************************************************************
	*				  		PMXRigidBodyCalculationType
	****************************************************************************/
	/*  @brief ���̂̌v�Z���@�ɂ���
	*****************************************************************************/
	enum class PMXRigidBodyCalculationType : gu::uint8
	{
		Static              = 0, //!< �ÓI
		Dynamic             = 1, //!< ���I
		DynamicAndBoneMerge = 2, //!< �������Z
	};

	/****************************************************************************
	*				  		PMXRigidBody
	****************************************************************************/
	/*  @brief ����
	*****************************************************************************/
	struct PMXRigidBody
	{
		/*! @brief ���̖�(���{��)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief ���̖�(�p��)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief ���̂̊֘A�{�[���C���f�b�N�X*/
		gu::int32 RelatedBoneIndex = 0;

		/*! @brief �O���[�v*/
		gu::uint8 Group = 0;

		/*! @brief �}�X�N (��Փ˃O���[�v�t���O)*/
		gu::uint16 Mask = 0;

		/*! @brief �`��*/
		gu::uint8 Shape = 0;

		/*! @brief �T�C�Y*/
		gm::Float3 Size = {};

		/*! @brief �ʒu*/
		gm::Float3 Position = {};

		/*! @brief ��]*/
		gm::Float3 Rotation = {};

		/*! @brief ����*/
		gu::float32 Mass = 0.0f;

		/*! @brief �ړ�����*/
		gu::float32 TranslateDamping = 0.0f;

		/*! @brief ��]����*/
		gu::float32 RotationDamping = 0.0f;

		/*! @brief ������*/
		gu::float32 Repulsion = 0.0f;

		/*! @brief ���C��*/
		gu::float32 Friction = 0.0f;

		/*! @brief �������Z�̎��*/
		PMXRigidBodyCalculationType CalculationType = PMXRigidBodyCalculationType::Static;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXJointType
	****************************************************************************/
	/*  @brief Joint�̎�� joint�Ƃ͍��̓��m��ڑ����邽�߂̐ڑ����@�̂��Ƃł�
	*****************************************************************************/
	enum class PMXJointType : gu::uint8
	{
		GenericSpring6DOF = 0, //!< 6���R�x�o�l�t���W���C���g (PMX2.0�����PMD�Ɠ��l) : �Ή�btGeneric6DofSpringConstraint
		Generic6DOF       = 1, //!< 6���R�x�W���C���g                            : �Ή�btGeneric6DofConstraint
		PointToPoint      = 2, //!< �e���� / �΂˒萔������                       : �Ή�btPoint2PointConstraint
		ConeTwist         = 3, //!< �~���͈͎w��@�@�@�@�@�@�@�@�@�@�@               : �Ή�btConeTwistConstraint
		Slider            = 4, //!< �X���C�_�[�W���C���g (���ړ��W���C���g) 		     : �Ή�btSliderConstraint
		Hinge             = 5, //!< �q���W�W���C���g   (��]�W���C���g) 		     : �Ή�btHingeConstraint
	};

	/****************************************************************************
	*				  		PMXJoint
	****************************************************************************/
	/*  @brief Joint
	*****************************************************************************/
	struct PMXJoint
	{
		/*! @brief Joint��(���{��)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief Joint��(�p��)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief Joint�̎��*/
		PMXJointType JointType = PMXJointType::GenericSpring6DOF;

		/*! @brief ����A�̃C���f�b�N�X*/
		gu::int32 RigidBodyIndexA = 0;

		/*! @brief ����B�̃C���f�b�N�X*/
		gu::int32 RigidBodyIndexB = 0;

		/*! @brief �ʒu*/
		gm::Float3 Position = {};

		/*! @brief ��] (radian)*/
		gm::Float3 Rotation = {};

		/*! @brief �ړ������̉���*/
		gm::Float3 TranslationLimitLower = {};

		/*! @brief �ړ������̏��*/
		gm::Float3 TranslationLimitUpper = {};

		/*! @brief ��]�����̉��� (radian)*/
		gm::Float3 RotationLimitLower = {};

		/*! @brief ��]�����̏�� (radian)*/
		gm::Float3 RotationLimitUpper = {};

		/*! @brief �ړ��o�l�萔*/
		gm::Float3 TranslationSpringConstant = {};

		/*! @brief ��]�o�l�萔*/
		gm::Float3 RotationSpringConstant = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXSoftBodyShape
	****************************************************************************/
	/*  @brief SoftBody�̌`�� 
	*****************************************************************************/
	enum class PMXSoftBodyShape : gu::uint8
	{
		TriangleMesh = 0, //!< �O�p�`���b�V��
		Rope         = 1, //!< ���[�v
	};

	/****************************************************************************
	*				  		PMXSoftBodyFlags
	****************************************************************************/
	/*  @brief SoftBody�̃t���O
	*****************************************************************************/
	enum class PMXSoftBodyFlags : gu::uint8
	{
		None       = 0x00,
		BLink      = 0x01, //!< BLink
		Cluster    = 0x02, //!< �N���X�^�쐬
		HybridLink = 0x04  //!< �����N���G
	};
	ENUM_CLASS_FLAGS(PMXSoftBodyFlags);

	/****************************************************************************
	*				  		PMXAeroModel
	****************************************************************************/
	/*  @brief AeroModel
	*****************************************************************************/
	enum class PMXAeroModel
	{
		VPoint    = 0, //!< V�|�C���g 
		VTwoSided = 1, //!< V���� 
		VOneSided = 2, //!< V�Ж�
		FTwoSided = 3, //!< F����
		FOneSided = 4, //!< F�Ж�
	};

	/****************************************************************************
	*				  		PMXSoftBodyAnchorRigidBody
	****************************************************************************/
	/*  @brief �A���J�[����
	*****************************************************************************/
	struct PMXAnchorRigidBody
	{
		/*! @brief �֘A���̃C���f�b�N�X*/
		gu::int32 RigidBodyIndex = 0;

		/*! @brief �֘A���_�C���f�b�N�X*/
		gu::int32 VertexIndex = 0;

		/*! @brief Near���[�h*/
		bool IsNearMode = false;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXSoftBodyConfig
	****************************************************************************/
	/*  @brief �\�t�g�{�f�B�̐ݒ�
	*****************************************************************************/
	struct PMXSoftBodyConfig
	{
		/*! @brief ���x�␳�W�� (Baumgarte)*/
		gu::float32 VelocityCorrection = 0.0f;

		/*! @brief �����W�� [0, 1]*/
		gu::float32 DampingCoefficient = 0.0f;

		/*! @brief �R�͌W�� [0,+inf]*/
		gu::float32 DragCoefficient = 0.0f;

		/*! @brief �g�͌W�� [0, +inf]*/
		gu::float32 LiftCoefficient = 0.0f;

		/*! @brief ���͌W�� [-inf, +inf]*/
		gu::float32 PressureCoefficient = 0.0f;

		/*! @brief �̐ϕۑ��W�� [0, +inf]*/
		gu::float32 VolumeConversation = 0.0f;

		/*! @brief �����C�W�� [0, 1] */
		gu::float32 DynamicFriction = 0.0f;

		/*! @brief �|�[�Y�}�b�`���O�W�� [0, 1]*/
		gu::float32 PoseMatchingCoefficient = 0.0f;

		/*! @brief ���̐ڐG�̍d�� [0, 1]*/
		gu::float32 RigidBodyContactHardness = 0.0f;

		/*! @brief �^���ڐG�̍d�� [0, 1]*/
		gu::float32 KineticContactHardness = 0.0f;

		/*! @brief �\�t�g�ڐG�̍d�� [0, 1]*/
		gu::float32 SoftContactHardness = 0.0f;

		/*! @brief �A���J�[�d�� [0, 1]*/
		gu::float32 AnchorHardness = 0.0f;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

	};

	/****************************************************************************
	*				  		PMXSoftBodyCluster
	****************************************************************************/
	/*  @brief �\�t�g�{�f�B�̃N���X�^
	*****************************************************************************/
	struct PMXSoftBodyCluster
	{
		/*! @brief �\�t�g�{�f�B vs ���� �d�� [0, 1]*/
		gu::float32 SoftVsRigidBodyHardness = 0.0f;

		/*! @brief �\�t�g�{�f�B vs Kinetic �d�� [0, 1]*/
		gu::float32 SoftVsKineticHardness = 0.0f;

		/*! @brief �\�t�g�{�f�B vs �\�t�g�{�f�B �d�� [0, 1]*/
		gu::float32 SoftVsSoftHardness = 0.0f;

		/*! @brief �\�t�g�{�f�B vs ���� impulse split [0, 1]*/
		gu::float32 SoftVsRigidBodyImpulseSplit = 0.0f;

		/*! @brief �\�t�g�{�f�B vs Kinetic impulse split [0, 1]*/
		gu::float32 SoftVsKineticImpulseSplit = 0.0f;

		/*! @brief �\�t�g�{�f�B vs �\�t�g�{�f�B impulse split [0, 1]*/
		gu::float32 SoftVsSoftImpulseSplit = 0.0f;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMXSoftBodyIteration
	****************************************************************************/
	/*  @brief �\�t�g�{�f�B�v�Z�̊e������
	*****************************************************************************/
	struct PMXSoftBodyIteration
	{
		/*! @brief ���xSolver�̔�����*/
		gu::int32 VelocitySolver = 0;

		/*! @brief �ʒuSolver�̔�����*/
		gu::int32 PositionSolver = 0;

		/*! @brief �h���t�gSolver�̔�����*/
		gu::int32 DriftSolver = 0;

		/*! @brief �N���X�^Solver�̔�����*/
		gu::int32 ClusterSolver = 0;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMXSoftBodyMaterial
	****************************************************************************/
	/*  @brief �\�t�g�{�f�B�̍ގ��ݒ�
	*****************************************************************************/
	struct PMXSoftBodyMaterial
	{
		/*! @brief ���`�����W�� [0, 1]*/
		gu::float32 LinearStiffness = 0.0f;

		/*! @brief �ʐ� / �p�x�����W�� [0,1]*/
		gu::float32 AngularStiffness = 0.0f;

		/*! @brief �̐ύ����W�� [0,1]*/
		gu::float32 VolumeStiffness = 0.0f;

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMXSoftBody
	****************************************************************************/
	/*  @brief �\�t�g�{�f�B
	*****************************************************************************/
	struct PMXSoftBody
	{
		/*! @brief �\�t�g�{�f�B��(���{��)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief �\�t�g�{�f�B��(�p��)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief �`��*/
		PMXSoftBodyShape Shape = PMXSoftBodyShape::TriangleMesh;

		/*! @brief �}�e���A�����w�肷��C���f�b�N�X*/
		gu::int32 MaterialIndex = 0;

		/*! @brief �O���[�v�C���f�b�N�X*/
		gu::uint8 Group = 0;

		/*! @brief ��Փ˃O���[�v�t���O*/
		gu::uint16 NonCollideGroupFlag = 0;

		/*! @brief �\�t�g�{�f�B�t���O*/
		PMXSoftBodyFlags Flag = PMXSoftBodyFlags::None;

		/*! @brief BLink�̍쐬����*/
		gu::int32 BLinkDistance = 0;

		/*! @brief �N���X�^�̍쐬��*/
		gu::int32 ClusterCount = 0;

		/*! @brief ������*/
		gu::float32 TotalMass = 0.0f;

		/*! @brief �Փ˃}�[�W��*/
		gu::float32 CollisionMargin = 0.0f;

		/*! @brief AeroModel*/
		PMXAeroModel AeroModel = PMXAeroModel::VPoint;

		/*! @brief Config*/
		PMXSoftBodyConfig Config = {};

		/*! @brief Cluster*/
		PMXSoftBodyCluster Cluster = {};

		/*! @brief Iteration*/
		PMXSoftBodyIteration Iteration = {};

		/*! @brief Material*/
		PMXSoftBodyMaterial Material = {};

		/*! @brief �A���J�[����*/
		gu::DynamicArray<PMXAnchorRigidBody> AnchorRigidBodies = {};

		/*! @brief �֘A���_Index*/
		gu::DynamicArray<gu::int32> VertexIndices = {};

		/*!**********************************************************************
		*  @brief     pmx�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @param[in] const PMXHeader �w�b�_���
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);

		~PMXSoftBody()
		{
			AnchorRigidBodies.Clear(); AnchorRigidBodies.ShrinkToFit();
			VertexIndices.Clear(); VertexIndices.ShrinkToFit();
		}
	};

}

#endif