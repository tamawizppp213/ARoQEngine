//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "../../Windows/Include/WindowsWindow.hpp"
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#include "../../Windows/Include/WindowsError.hpp"
#include <cassert>
#include <stdexcept>
#include <dwmapi.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "Dwmapi.lib")

using namespace platform;
using namespace platform::windows;
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Main Function
void CoreWindow::Create(const SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc)
{
	/*-----------------------------------------------------------------
					   Prepare 
	--------------------------------------------------------------------*/
	assert(("application is nullptr", application));

	const auto windowsApplication = StaticPointerCast<windows::PlatformApplication>(application);

	/*-----------------------------------------------------------------
				desc�̓o�^
	--------------------------------------------------------------------*/
	_windowMode       = desc.WindowMode;
	_activationPolicy = desc.ActivationPolicy;

	/*-----------------------------------------------------------------
		               Window Settings
	--------------------------------------------------------------------*/
	int windowWidth   = static_cast<int>(desc.DesiredScreenWidth);
	int windowHeight  = static_cast<int>(desc.DesiredScreenHeight);
	int windowCenterX = static_cast<int>(desc.DesiredScreenPositionX);
	int windowCenterY = static_cast<int>(desc.DesiredScreenPositionY);
	
	// https://learn.microsoft.com/ja-jp/windows/win32/winmsg/extended-window-styles
	DWORD windowExtensionStyle = 0;
	DWORD windowDefaultStyle   = 0;
	if (desc.HasWindowOSWindowBorder)
	{
		// OS window border set up
		windowExtensionStyle = WS_EX_APPWINDOW; // �E�B���h�E���\������Ă���Ƃ��Ƀ^�X�N�o�[�ɍŏ�ʂ̃E�B���h�E�������I�ɔz�u����
		windowDefaultStyle   = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION; // �^�C�g���Ƙg�t���̃E�B���h�E + ���j���[�{�b�N�X + �^�C�g���o�[
		
		if (desc.IsVanillaWindow)
		{
			if (desc.SupportMaximize) { windowDefaultStyle |= WS_MAXIMIZEBOX; } // �ő剻�{�^�������E�B���h�E
			if (desc.SupportMinimize) { windowDefaultStyle |= WS_MINIMIZEBOX; } // �ŏ����{�^�������E�B���h�E
			if (desc.SupportResize  ) { windowDefaultStyle |= WS_THICKFRAME;  } // �T�C�Y�ύX���E�����E�B���h�E���쐬����
			else                      { windowDefaultStyle |= WS_BORDER;      } // ���E�������E�B���h�E���쐬����
		}
		else
		{
			windowDefaultStyle |= WS_POPUP | WS_BORDER;
		}
	}
	else
	{
		windowExtensionStyle = WS_EX_WINDOWEDGE;                             // �E�B���h�E�ɂ̓G�b�W���オ�����r�������� 
		windowDefaultStyle   = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS; // ��E�B���h�E + �e�E�B���h�E���ŕ`�悳��鎞�Ɏq�E�B���h�E����L����̈�����O

		if (desc.AppearsInTaskbar)
		{
			windowExtensionStyle |= WS_EX_APPWINDOW;  // �E�B���h�E���\������Ă��鎞��, �^�X�N�o�[�ɍŏ�ʂ̃E�B���h�E�������I�ɔz�u����
		}
		else
		{ 
			windowExtensionStyle |= WS_EX_TOOLWINDOW; // �c�[���o�[�Ƃ��Ďg�p
		}
	}

	/*-----------------------------------------------------------------
						Create window
	--------------------------------------------------------------------*/
	RECT windowRect = { 0, 0, 0, 0 };
	::AdjustWindowRectEx(&windowRect, windowDefaultStyle, false, windowExtensionStyle);

	windowCenterX += windowRect.left;
	windowCenterY += windowRect.top;
	windowWidth   += windowRect.right - windowRect.left;
	windowHeight  += windowRect.bottom - windowRect.top;

	/*-----------------------------------------------------------------
	                	Create window
	--------------------------------------------------------------------*/
	_hwnd = CreateWindowEx
	(
		windowExtensionStyle, // Window style
		core::PlatformApplication::APPLICATION_NAME, // Name to register with Windows
		desc.Title,
		windowDefaultStyle,
		windowCenterX, // Center position_X
		windowCenterY, // Center position_Y
		windowWidth,   // window width
		windowHeight,  // window height
		NULL, // parent window
		NULL, // menu handler 
		windowsApplication->GetWindowsInstanceHandle(), // instance handler 
		windowsApplication.Get()  // additional expansion area
	);

	if (_hwnd == NULL)
	{
		ErrorLogger::Log(NULL);
		return;
	}

	/*-----------------------------------------------------------------
				Enable to touch the ui button in the window  
	--------------------------------------------------------------------*/
	if (desc.UseTouchFeedback)
	{
		if (!RegisterTouchWindow(_hwnd, NULL))
		{
			ErrorLogger::Log(_hwnd);
		}
	}
}

/****************************************************************************
*                     Show
*************************************************************************//**
*  @fn        bool CoreWindow::Show()
*
*  @brief     �E�B���h�E��\�����܂�
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Show()
{
	if (_isVisible) { return true; }
	
	_isVisible = true;
	
	/*---------------------------------------------------------------
					  �A�N�e�B�u�����s����
	-----------------------------------------------------------------*/
	bool shouldActiveWindow = _activationPolicy == core::ActivationPolicy::Always;
	if (_isFirstTimeVisible && _activationPolicy == core::ActivationPolicy::FirstOnlyShow)
	{
		shouldActiveWindow = true;
	}

	/*---------------------------------------------------------------
					  �\���R�}���h�̑I��
	-----------------------------------------------------------------*/
	int showWindowCommand = shouldActiveWindow ? SW_SHOW : SW_SHOWNOACTIVATE;
	if (_isFirstTimeVisible)
	{
		_isFirstTimeVisible = false; // �ő剻��ŏ����̎��͏��߂ĕ\�������Ƃ͌����Ȃ�
		if (_initiallyMinimized)
		{
			showWindowCommand = shouldActiveWindow ? SW_MINIMIZE : SW_SHOWMINNOACTIVE;
		}
		else if (_initiallyMaximized)
		{
			showWindowCommand = SW_MAXIMIZE; // �A�N�e�B�u�ł͂���
		}
	}

	//// SW_SHOW : �E�B���h�E���A�N�e�B�u����, ���݂̃T�C�Y�ƈʒu�ŕ\������
	::ShowWindow(_hwnd, showWindowCommand);
	return ErrorLogger::Succeed();
}

/****************************************************************************
*                     Hide
*************************************************************************//**
*  @fn        bool CoreWindow::Hide()
*
*  @brief     Visible�ł������ꍇ�ɃE�B���h�E���B���܂�
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Hide()
{
	if (_isVisible)
	{
		_isVisible = false;
		::ShowWindow(_hwnd, SW_HIDE);
		return ErrorLogger::Succeed();
	}
	return true;
}

/****************************************************************************
*                     Minimize
*************************************************************************//**
*  @fn        bool CoreWindow::Minimize()
*
*  @brief     �E�B���h�E���ŏ������܂�.
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Minimize()
{
	if (_isFirstTimeVisible)
	{
		_initiallyMaximized = false;
		_initiallyMinimized = true;
		return Show();
	}
	else
	{
		::ShowWindow(_hwnd, SW_MINIMIZE);
		return ErrorLogger::Succeed();
	}
}

/****************************************************************************
*                     Maximize
*************************************************************************//**
*  @fn        bool CoreWindow::Maximize()
*
*  @brief     �E�B���h�E���ő剻���܂�
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Maximize()
{
	if (_isFirstTimeVisible)
	{
		_initiallyMaximized = true;
		_initiallyMinimized = false;
		return Show();
	}
	else
	{
		// �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������
		::ShowWindow(_hwnd, SW_MAXIMIZE);
		return ErrorLogger::Succeed();
	}
}

/****************************************************************************
*                     ReStore
*************************************************************************//**
*  @fn        bool CoreWindow::ReStore()
*
*  @brief     �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������B�ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::ReStore()
{
	// �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������B�ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.
	if (_isFirstTimeVisible)
	{
		_initiallyMaximized = false;
		_initiallyMinimized = false;
		return true;
	}
	else
	{
		::ShowWindow(_hwnd, SW_RESTORE);
		return ErrorLogger::Succeed();
	}
}

/****************************************************************************
*                     Destroy
*************************************************************************//**
*  @fn        bool CoreWindow::Destroy()
*
*  @brief     �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������B�ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Destroy()
{
	return ::DestroyWindow(_hwnd);
}
#pragma endregion Main Function

#pragma region Supported Check Function
bool CoreWindow::IsForegroundWindow() const
{
	return ::GetForegroundWindow() == _hwnd;
}

bool CoreWindow::IsFullscreenSupported() const 
{
	// fullscreen not supported when using remote desktop
	return !::GetSystemMetrics(SM_REMOTESESSION);
}
#pragma endregion Supported Check Function
#endif