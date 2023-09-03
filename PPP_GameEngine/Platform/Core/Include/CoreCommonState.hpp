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
		float DesiredScreenPositionX = 0.0f;
		float DesiredScreenPositionY = 0.0f;
		float DesiredScreenWidth     = 0.0f;
		float DesiredScreenHeight    = 0.0f;
		wchar_t* Title               = L"PPP Game Window";
	};
}

#endif