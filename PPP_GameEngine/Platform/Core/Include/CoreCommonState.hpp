//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_COMMON_STATE_HPP
#define CORE_COMMON_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	/****************************************************************************
	*				  			    PlatformType
	*************************************************************************//**
	*  @class     PlatformType
	*  @brief     �v���b�g�t�H�[���̎�ނ��`���Ă��܂�
	*****************************************************************************/
	enum class PlatformType
	{
		Windows,

	};


	/****************************************************************************
	*				  			    CursorType
	*************************************************************************//**
	*  @class     CursorType
	*  @brief     cursor look 
	*****************************************************************************/
	enum class CursorType
	{
		None, 
		Default,
		TextEditBeam,
		ResizeLeftRight,
		ResizeUpDown,
		ResizeSouthEast,
		ResizeSouthWest,
		CardinalCross,
		Crosshairs,
		Hand,
		Prohibit,
		CountOf,
	};

	/****************************************************************************
	*				  			  ActivationType
	*************************************************************************//**
	*  @class     ActivationType
	*  @brief     �E�B���h�E�̋N����, ��N�����̏u�ԂɌĂ΂��enum class�ł�.
	*****************************************************************************/
	enum class ActivationType
	{
		InActive,      // ��A�N�e�B�u
		Activate,      // �}�E�X�N���b�N�ȊO�̃��\�b�h�ɂ���ăA�N�e�B�u��
		ClickActivate, // �}�E�X�N���b�N�ŃA�N�e�B�u��
	};

	/****************************************************************************
	*				  			  ActivationPolicy
	*************************************************************************//**
	*  @class     ActivationPolicy
	*  @brief     �E�B���h�E�̕\�����̃A�N�e�B�u���̕��j�����߂���
	*****************************************************************************/
	enum class ActivationPolicy
	{
		Never,        // ������Active�����Ȃ�
		Always,       // �\�������Ƃ��ɕK��window��Active�ɂ���
		FirstOnlyShow // �ŏ��̈�񂾂�
	};

	/****************************************************************************
	*				  			    WindowMode
	*************************************************************************//**
	*  @class     WindowMode
	*  @brief     window mode
	*****************************************************************************/
	enum class WindowMode
	{
		FullScreen,
		BorderLess,
		Windowed,
		CountOf
	};

	/****************************************************************************
	*				  			  CoreWindowDesc
	*************************************************************************//**
	*  @struct     CoreWindowDesc
	*  @brief     �E�B���h�E�p�̃f�B�X�N���v�^�ł�
	*****************************************************************************/
	struct CoreWindowDesc
	{
		float DesiredScreenPositionX  = 0.0f;  // �ŏ��ɖ]�܂����Ƃ����X�N���[���̉��̈ʒu
		float DesiredScreenPositionY  = 0.0f;  // �ŏ��ɖ]�܂����Ƃ����X�N���[���̏c�̈ʒu
		float DesiredScreenWidth      = 0.0f;  // �ŏ��ɖ]�܂����Ƃ���镝
		float DesiredScreenHeight     = 0.0f;  // �ŏ��ɖ]�܂����Ƃ���鍂��
		wchar_t* Title                = L"PPP Game Window";
		bool  HasWindowOSWindowBorder = false; // OS�̃E�B���h�E�̋��E�����g�p�������ꍇ��true
		bool  IsVanillaWindow         = false;
		bool  SupportMaximize         = true;  // �E�B���h�E�̍ő剻�����邩  
		bool  SupportMinimize         = true;  // �E�B���h�E�̍ŏ��������邩
		bool  SupportResize           = true;  // �E�B���h�E�̃��T�C�Y���\�ɂ��邩
		bool  AppearsInTaskbar        = false; // �^�X�N�o�[�ɃE�B���h�E��\�����邩
		bool  UseTouchFeedback        = false; // ui�{�^�����̃^�b�`���͂��\�ɂ��邩
		ActivationPolicy ActivationPolicy = ActivationPolicy::FirstOnlyShow;
		WindowMode       WindowMode       = WindowMode::Windowed;
	};
}

#endif
