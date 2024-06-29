//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_CURSOR_HPP
#define WINDOWS_CURSOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CoreCursor.hpp"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace platform::windows
{
	/****************************************************************************
	*				  			    ICursor
	****************************************************************************/
	/* @class     ICursor
	*  @brief     Cursor interface
	*****************************************************************************/
	class ICursor : public core::ICursor
	{
	public:
#pragma region Public Function
		/*!***********************************************************************
		*  @brief      �J�[�\����\�����܂�
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void Show() override;

		/*!***********************************************************************
		*  @brief      �J�[�\�����\���ɂ��܂�
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void Hide() override;

		/*!***********************************************************************
		*  @brief      �w�肵���s�N�Z���̈���ł̂݃J�[�\���𓮂�����悤�ɂ��܂�
		*  @param[in]  Rectangle* �̈�
		*  @return     bool
		**************************************************************************/
		virtual bool Lock(core::Rectangle* rectangle) override;

		/*!***********************************************************************
		*  @brief      .cur�̂݌��ݎg�p�\�ł���, �I���W�i���̃J�[�\�����쐬���܂�.�R���X�g���N�^�ł��ݒ�\�Ȃ̂ł��D�݂Ŏg�p���Ă�������.
		*  @param[in]  const char* filePath �t�@�C���p�X
		*  @return     void*
		**************************************************************************/
		virtual void* CreateCursorFromFile(const char* filePath) override;

		/*!***********************************************************************
		*  @brief      RGBA�̃o�b�t�@�Ɋ�Â��Ē��ڃJ�[�\�����쐬���܂�
		*  @param[in]  const gm::RGBA*  �s�N�Z��
		*  @param[in]  const gu::int32  ��
		*  @param[in]  const gu::int32  ����
		*  @return     void*
		**************************************************************************/
		virtual void* CreateCursorFromRGBABuffer(const gm::RGBA* pixels, const gu::int32 width, const gu::int32 height) override;
		
		#pragma endregion

		#pragma region Public Property

		/*!***********************************************************************
		*  @brief      (x, y)�s�N�Z������J�[�\���ʒu���擾���܂�
		*  @param[in]  void
		*  @return     gm::Float2
		**************************************************************************/
		virtual gm::Float2  GetPosition() const override;

		/*!***********************************************************************
		*  @brief      �J�[�\���̎�ނ�Ԃ��܂�
		*  @param[in]  void
		*  @return     CursorType
		**************************************************************************/
		virtual core::CursorType GetType() const override;

		/*!***********************************************************************
		*  @brief      �J�[�\���̃T�C�Y��Ԃ��܂�
		*  @param[out] gu::int32& ��
		*  @param[out] gu::int32& ����
		*  @return     void
		**************************************************************************/
		virtual void GetSize(gu::int32& width, gu::int32& height) const override;

		/*!***********************************************************************
		*  @brief      (x, y)�s�N�Z������J�[�\���ʒu��ݒ肵�܂�
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @return     void
		**************************************************************************/
		virtual void SetPosition(const gu::int32 x, const gu::int32 y) const override;

		/*!***********************************************************************
		*  @brief      �J�[�\���̎�ނ�ݒ肵�܂�
		*  @param[in]  const CursorType type
		*  @return     void
		**************************************************************************/
		virtual void SetType(const core::CursorType type) override;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		explicit ICursor(const core::CursorType type);

		explicit ICursor(const char* filePath);

		~ICursor();


	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		HCURSOR _cursor = NULL;
	};
}
#endif
#endif