//////////////////////////////////////////////////////////////////////////////////
///  @file   VMDDefines.hpp
///  @brief  VMD�Ŏg�p�����`���܂Ƃ߂��w�b�_�t�@�C��.
///  @author toide
///  @date   2024/06/05 23:05:33
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VMD_DEFINES_HPP
#define VMD_DEFINES_HPP

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
namespace gc::file::vmd
{
	/****************************************************************************
	*				  		VMDHeader
	*************************************************************************//**
	/*  @brief  VMD�̃w�b�_�����i�[����\���̂ł�.
	*****************************************************************************/
	struct VMDHeader
	{
		/*! @brief �V�O�l�`�� Vocaloid Motion Data 0002 (30 byte)*/
		gu::tstring Header = SP("");

		/*! @brief ���f���� (20 byte)*/
		gu::tstring ModelName = SP("");

		/*!**********************************************************************
		*  @brief     �L����VMD�t�@�C����
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool IsValid() const; 

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^��������
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDBoneKeyFrame
	*************************************************************************//**
	/*  @brief  �{�[���̃L�[�t���[�������i�[����\���̂ł�.
	*****************************************************************************/
	struct VMDBoneKeyFrame
	{
		/*! @brief �{�[����*/
		gu::tstring Name = SP("");

		/*! @brief �t���[���ԍ�*/
		gu::uint32 Frame = 0;

		/*! @brief �ʒu*/
		gm::Float3 Translation = {};

		/*! @brief ��]*/
		gm::Float4 Quaternion = {};

		/*! @brief �x�W�G�Ȑ��̕�ԃp�����[�^. 0�`127*/
		gu::uint8 BazierInterpolation[64] = {};

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^��������
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDFaceKeyFrame
	*************************************************************************//**
	/*  @brief  �\��̃L�[�t���[�������i�[����\���̂ł�.
	*****************************************************************************/
	struct VMDFaceKeyFrame
	{
		/*! @brief �\�*/
		gu::tstring Name = SP("");

		/*! @brief �t���[���ԍ�*/
		gu::uint32 Frame = 0;

		/*! @brief �E�F�C�g*/
		gu::float32 Weight = 0.0f;

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^��������
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDCameraKeyFrame
	*************************************************************************//**
	/*  @brief  �J�����̃L�[�t���[�������i�[����\���̂ł�.
	*****************************************************************************/
	struct VMDCameraKeyFrame
	{
		/*! @brief �t���[���ԍ�*/
		gu::uint32 Frame = 0;

		/*! @brief ����*/
		gu::float32 Distance = 0.0f;

		/*! @brief �ʒu*/
		gm::Float3 Position = {};

		/*! @brief ��]*/
		gm::Float3 Rotation = {};

		/*! @brief ��ԃp�����[�^*/
		gu::uint8 Interpolation[24] = {};

		/*! @brief ����p*/
		gu::uint32 ViewAngle = 0;

		/*! @brief �������e���ǂ���*/
		bool IsPerspective = 0;

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^��������
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDLightKeyFrame
	*************************************************************************//**
	/*  @brief  ���C�g�̃L�[�t���[�������i�[����\���̂ł�.
	*****************************************************************************/
	struct VMDLightKeyFrame
	{
		/*! @brief �t���[���ԍ�*/
		gu::uint32 Frame = 0;

		/*! @brief �F*/
		gm::Float3 Color = {};

		/*! @brief �ʒu*/
		gm::Float3 Position = {};

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^��������
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDShadowType
	*************************************************************************//**
	/*  @brief  �Z���t�V���h�E�̂�����\���񋓑̂ł�.
	*****************************************************************************/
	enum class VMDShadowType : gu::uint8
	{
		Off,    //!< �I�t
		Mode1,  //!< ���[�h1
		Mode2   //!< ���[�h2
	};

	/****************************************************************************
	*				  		VMDSelfShadowKeyFrame
	*************************************************************************//**
	/*  @brief  �Z���t�V���h�E�̃L�[�t���[�������i�[����\���̂ł�.
	*****************************************************************************/
	struct VMDSelfShadowKeyFrame
	{
		/*! @brief �t���[���ԍ�*/
		gu::uint32 Frame = 0;

		/*! @brief �V���h�E�̎��*/
		VMDShadowType ShadowType = VMDShadowType::Off;

		/*! @brief �V���h�E����*/
		gu::float32 Distance = 0.0f;

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^��������
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDIKEnable
	*************************************************************************//**
	/*  @brief  ���f���\���̃L�[�t���[�������i�[����\���̂ł�.
	*****************************************************************************/
	struct VMDIKEnable
	{
		/*! @brief IK��*/
		gu::tstring Name = SP("");

		/*! �L�������邩*/
		bool IsEnable = false;

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^��������
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDIKKeyFrame
	*************************************************************************//**
	/*  @brief  IK�̃L�[�t���[�������i�[����\���̂ł�.
	*****************************************************************************/
	struct VMDIKKeyFrame
	{
		/*! @brief �t���[���ԍ�*/
		gu::uint32 Frame = 0;

		/*! @brief �\�����邩*/
		bool IsDisplay = false;

		/*! @brief IK�̗L�������*/
		gu::DynamicArray<VMDIKEnable> IKEnables = {};

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^�ǂݍ���
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMD�t�@�C�������ƂɃf�[�^��������
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;

		~VMDIKKeyFrame()
		{
			IKEnables.Clear(); IKEnables.ShrinkToFit();
		}
	};
}

#endif