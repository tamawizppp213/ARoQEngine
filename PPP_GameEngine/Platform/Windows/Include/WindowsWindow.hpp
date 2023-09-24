//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_WINDOW_HPP
#define WINDOWS_WINDOW_HPP

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CoreWindow.hpp"
#include <Windows.h>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class CoreWindow : public platform::core::CoreWindow
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Create(const std::shared_ptr<core::PlatformApplication>& application, const core::CoreWindowDesc& desc);

		/* @brief : Visible�łȂ������ꍇ�ɃE�B���h�E��\�����܂�.*/
		bool Show() override;

		/* @brief : Visible�o������ꍇ�ɃE�B���h�E���B���܂�*/
		bool Hide() override;

		bool Minimize() override;

		bool Maximize() override;

		/* @brief : �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������B�ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.*/
		bool ReStore() override;

		/* @brief : �E�B���h�E��j�����܂�*/
		bool Destroy() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		HWND GetHWND() const { return _hwnd; }

		void SetHWND(const HWND hwnd) { _hwnd = hwnd; }

		/* @brief : ���ݍ�ƒ��̃E�B���h�E�ł��邩�𒲂ׂ܂�. */
		bool IsForegroundWindow() const override;

		/* @brief : �t���X�N���[�����T�|�[�g���Ă��邩�𒲂ׂ܂�. */
		bool IsFullscreenSupported() const override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : Window handle*/
		HWND _hwnd = NULL;

		bool _isFirstTimeVisible = true;
		bool _initiallyMinimized = false;
		bool _initiallyMaximized = false;
	};
}
#endif _WIN32
#endif 