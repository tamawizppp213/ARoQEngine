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
				descの登録
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
		windowExtensionStyle = WS_EX_APPWINDOW; // ウィンドウが表示されているときにタスクバーに最上位のウィンドウを強制的に配置する
		windowDefaultStyle   = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION; // タイトルと枠付きのウィンドウ + メニューボックス + タイトルバー
		
		if (desc.IsVanillaWindow)
		{
			if (desc.SupportMaximize) { windowDefaultStyle |= WS_MAXIMIZEBOX; } // 最大化ボタンを持つウィンドウ
			if (desc.SupportMinimize) { windowDefaultStyle |= WS_MINIMIZEBOX; } // 最小化ボタンを持つウィンドウ
			if (desc.SupportResize  ) { windowDefaultStyle |= WS_THICKFRAME;  } // サイズ変更境界を持つウィンドウを作成する
			else                      { windowDefaultStyle |= WS_BORDER;      } // 境界線を持つウィンドウを作成する
		}
		else
		{
			windowDefaultStyle |= WS_POPUP | WS_BORDER;
		}
	}
	else
	{
		windowExtensionStyle = WS_EX_WINDOWEDGE;                             // ウィンドウにはエッジが上がった罫線がある 
		windowDefaultStyle   = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS; // 空ウィンドウ + 親ウィンドウ内で描画される時に子ウィンドウが占有する領域を除外

		if (desc.AppearsInTaskbar)
		{
			windowExtensionStyle |= WS_EX_APPWINDOW;  // ウィンドウが表示されている時に, タスクバーに最上位のウィンドウを強制的に配置する
		}
		else
		{ 
			windowExtensionStyle |= WS_EX_TOOLWINDOW; // ツールバーとして使用
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
*  @brief     ウィンドウを表示します
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
					  アクティブ化を行うか
	-----------------------------------------------------------------*/
	bool shouldActiveWindow = _activationPolicy == core::ActivationPolicy::Always;
	if (_isFirstTimeVisible && _activationPolicy == core::ActivationPolicy::FirstOnlyShow)
	{
		shouldActiveWindow = true;
	}

	/*---------------------------------------------------------------
					  表示コマンドの選択
	-----------------------------------------------------------------*/
	int showWindowCommand = shouldActiveWindow ? SW_SHOW : SW_SHOWNOACTIVATE;
	if (_isFirstTimeVisible)
	{
		_isFirstTimeVisible = false; // 最大化や最小化の時は初めて表示したとは言えない
		if (_initiallyMinimized)
		{
			showWindowCommand = shouldActiveWindow ? SW_MINIMIZE : SW_SHOWMINNOACTIVE;
		}
		else if (_initiallyMaximized)
		{
			showWindowCommand = SW_MAXIMIZE; // アクティブではある
		}
	}

	//// SW_SHOW : ウィンドウをアクティブ化し, 現在のサイズと位置で表示する
	::ShowWindow(_hwnd, showWindowCommand);
	return ErrorLogger::Succeed();
}

/****************************************************************************
*                     Hide
*************************************************************************//**
*  @fn        bool CoreWindow::Hide()
*
*  @brief     Visibleであった場合にウィンドウを隠します
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
*  @brief     ウィンドウを最小化します.
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
*  @brief     ウィンドウを最大化します
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
		// ウィンドウをアクティブにして表示する
		::ShowWindow(_hwnd, SW_MAXIMIZE);
		return ErrorLogger::Succeed();
	}
}

/****************************************************************************
*                     ReStore
*************************************************************************//**
*  @fn        bool CoreWindow::ReStore()
*
*  @brief     ウィンドウをアクティブにして表示する。最小化・最大化されている場合は元のサイズと位置に復元される.
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::ReStore()
{
	// ウィンドウをアクティブにして表示する。最小化・最大化されている場合は元のサイズと位置に復元される.
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
*  @brief     ウィンドウをアクティブにして表示する。最小化・最大化されている場合は元のサイズと位置に復元される.
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