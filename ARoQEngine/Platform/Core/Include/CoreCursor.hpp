//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreCursor.hpp
///  @brief  �J�[�\���̃A�v���P�[�V�����ɑ΂���\��, ��\�����s���܂�
///  @author Toide Yutaro
///  @date   2023_09_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_CURSOR_HPP
#define CORE_CURSOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CoreCommonState.hpp"
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct RGBA;
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	/****************************************************************************
	*				  			    ICursor
	****************************************************************************/
	/*  @brief �J�[�\���̃A�v���P�[�V�����ɑ΂���\��, ��\�����s���܂�
	*****************************************************************************/
	class ICursor : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      �J�[�\����\�����܂�
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void Show() = 0;

		/*!***********************************************************************
		*  @brief      �J�[�\�����\���ɂ��܂�
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void Hide() = 0;

		/*!***********************************************************************
		*  @brief      �w�肵���s�N�Z���̈���ł̂݃J�[�\���𓮂�����悤�ɂ��܂�
		*  @param[in]  Rectangle* �̈�
		*  @return     bool
		**************************************************************************/
		virtual bool Lock(Rectangle* rectangle) = 0;

		/*!***********************************************************************
		*  @brief      .cur�̂݌��ݎg�p�\�ł���, �I���W�i���̃J�[�\�����쐬���܂�.�R���X�g���N�^�ł��ݒ�\�Ȃ̂ł��D�݂Ŏg�p���Ă�������. 
		*  @param[in]  const char* filePath �t�@�C���p�X
		*  @return     void*
		**************************************************************************/
		virtual void* CreateCursorFromFile([[maybe_unused]] const char* filePath) = 0;

		/*!***********************************************************************
		*  @brief      RGBA�̃o�b�t�@�Ɋ�Â��Ē��ڃJ�[�\�����쐬���܂�
		*  @param[in]  const gm::RGBA*  �s�N�Z��
		*  @param[in]  const gu::int32  ��
		*  @param[in]  const gu::int32  ����
		*  @return     void*
		**************************************************************************/
		virtual void* CreateCursorFromRGBABuffer(const gm::RGBA* pixels, const gu::int32 width, const gu::int32 height) = 0;

		#pragma endregion

		#pragma region Public Property

		/*!***********************************************************************
		*  @brief      (x, y)�s�N�Z������J�[�\���ʒu���擾���܂�
		*  @param[in]  void
		*  @return     gm::Float2
		**************************************************************************/
		virtual gm::Float2 GetPosition() const = 0;
		
		/*!***********************************************************************
		*  @brief      �J�[�\���̎�ނ�Ԃ��܂�
		*  @param[in]  void
		*  @return     CursorType
		**************************************************************************/
		virtual CursorType GetType() const = 0;

		/*!***********************************************************************
		*  @brief      �J�[�\���̃T�C�Y��Ԃ��܂�
		*  @param[out] gu::int32& ��
		*  @param[out] gu::int32& ����
		*  @return     void
		**************************************************************************/
		virtual void GetSize(gu::int32& width, gu::int32& height) const = 0;

		/*!***********************************************************************
		*  @brief      (x, y)�s�N�Z������J�[�\���ʒu��ݒ肵�܂�
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @return     void
		**************************************************************************/
		virtual void SetPosition(const gu::int32 x, const gu::int32 y) const = 0;

		/*!***********************************************************************
		*  @brief      �J�[�\���̎�ނ�ݒ肵�܂�
		*  @param[in]  const CursorType type
		*  @return     void
		**************************************************************************/
		virtual void SetType(const CursorType type) = 0;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		
		/*! @brief �J�[�\����ނŏ�����*/
		explicit ICursor(const CursorType type) : _type(type) {};

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion

		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property

		/*! @brief �J�[�\���̎��*/
		CursorType _type = CursorType::Default;

		#pragma endregion
	};
}
#endif
