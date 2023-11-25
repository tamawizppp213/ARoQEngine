//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_WINDOW_HPP
#define WINDOWS_WINDOW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CoreWindow.hpp"
#include <Windows.h>
#include "GameUtility/Base/Include/GUMemory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			    CoreWindow
	*************************************************************************//**
	*  @class     CoreWindow
	*  @brief     �`��p�̃E�B���h�E���쐬����N���X
	*****************************************************************************/
	class CoreWindow : public platform::core::CoreWindow
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Create(const gu::SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc);

		/* @brief : Visible�łȂ������ꍇ�ɃE�B���h�E��\�����܂�.*/
		bool Show() override;

		/* @brief : Visible�o������ꍇ�ɃE�B���h�E���B���܂�*/
		bool Hide() override;

		/* @brief : �E�B���h�E���ŏ������܂�*/
		bool Minimize() override;

		/* @brief : �E�B���h�E���ő剻���܂�.*/
		bool Maximize() override;

		/* @brief : �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������B�ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.*/
		bool ReStore() override;

		/* @brief : �E�B���h�E��j�����܂�*/
		bool Destroy() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void* GetWindowHandle() const noexcept override { return _hwnd; }

		float GetAspectRatio() const override { return _aspectRatio;  }

		HWND GetHWND() const { return _hwnd; }

		void SetHWND(const HWND hwnd) { _hwnd = hwnd; }

		/* @brief : �L���v�V�����Ƀe�L�X�g��ǉ����܂�. */
		void SetText(const wchar_t* const text) override { SetWindowText(_hwnd, text); }

		/* @brief : ���ݍ�ƒ��̃E�B���h�E�ł��邩�𒲂ׂ܂�. */
		bool IsForegroundWindow() const override;

		/* @brief : �t���X�N���[�����T�|�[�g���Ă��邩�𒲂ׂ܂�. */
		bool IsFullscreenSupported() const override;

		// @brief : �ő剻����Ă��邩�𒲂ׂ܂�
		bool IsMaximized() const override { return !!::IsZoomed(_hwnd); }

		// @brief : �ŏ�������Ă��邩�𒲂ׂ܂� (�A�C�R����ԂɂȂ��Ă��邩)
		bool IsMinimized() const override { return !!::IsIconic(_hwnd); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CoreWindow();

		~CoreWindow();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : Window handle*/
		HWND _hwnd = NULL;

		float _aspectRatio = 0.0f;

		bool _isFirstTimeVisible = true;
		bool _initiallyMinimized = false;
		bool _initiallyMaximized = false;
	};
}
#endif _WIN32
#endif 