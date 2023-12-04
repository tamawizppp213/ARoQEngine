//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Windows/Include/WindowsWindow.hpp"
#if PLATFORM_OS_WINDOWS
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#include "../../Windows/Include/WindowsError.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
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
#pragma region Constructor and Destructor
CoreWindow::CoreWindow() : core::CoreWindow()
{

}

CoreWindow::~CoreWindow()
{

}
#pragma endregion Constructor and Destructor
#pragma region Main Function
/****************************************************************************
*                     Create
*************************************************************************//**
*  @fn        void CoreWindow::Create(const SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow)
*
*  @brief     ウィンドウを作成します
*
*  @param[in] const SharedPointer<core::PlatformApplication>& application
*  @param[in] const core::CoreWindowDesc& desc
*  @param[in] const gu::SharedPointer<core::CoreWindow>& parentWindow : 親ウィンドウ
*
*  @return    bool
*****************************************************************************/
void CoreWindow::Create(const SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow)
{
	/*-----------------------------------------------------------------
					   Prepare 
	--------------------------------------------------------------------*/
	Checkf(application, "application is nullptr");

	const auto windowsApplication = StaticPointerCast<windows::PlatformApplication>(application);

	/*-----------------------------------------------------------------
				descの登録
	--------------------------------------------------------------------*/
	_windowDesc = desc;

	/*-----------------------------------------------------------------
		               Window Settings
	--------------------------------------------------------------------*/
	int32 windowWidth   = static_cast<int32>(desc.DesiredScreenWidth);
	int32 windowHeight  = static_cast<int32>(desc.DesiredScreenHeight);
	int32 windowCenterX = static_cast<int32>(desc.DesiredScreenPositionX);
	int32 windowCenterY = static_cast<int32>(desc.DesiredScreenPositionY);
	
	// サイズ決定時にDPI scaleも考慮する
	_dpiScaleFactor = windowsApplication->GetDPIScaleFactorAtPixelPoint(static_cast<float>(windowCenterX), static_cast<float>(windowCenterY));

	// https://learn.microsoft.com/ja-jp/windows/win32/winmsg/extended-window-styles
	DWORD windowExtensionStyle = 0;
	DWORD windowDefaultStyle   = 0;
	if (desc.HasWindowOSWindowBorder)
	{
		// OS window border set up
		// ウィンドウが表示されているときにタスクバーに最上位のウィンドウを強制的に配置する
		windowExtensionStyle = WS_EX_APPWINDOW;

		// タイトルと枠付きのウィンドウ + メニューボックス + タイトルバー
		windowDefaultStyle   = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
		
		if (desc.IsVanillaWindow)
		{
			if (desc.SupportMaximize) { windowDefaultStyle |= WS_MAXIMIZEBOX; } // 最大化ボタンを持つウィンドウ
			if (desc.SupportMinimize) { windowDefaultStyle |= WS_MINIMIZEBOX; } // 最小化ボタンを持つウィンドウ
			if (desc.SupportResize  ) { windowDefaultStyle |= WS_THICKFRAME;  } // サイズ変更境界を持つウィンドウを作成する
			else                      { windowDefaultStyle |= WS_BORDER;      } // 境界線を持つウィンドウを作成する
		}
		else
		{
			// 空のウィンドウ + 境界線を持つウィンドウを作成
			// UE5を参考にしているが, vanilla windowであるならこっちの方を選択するのでは?
			windowDefaultStyle |= WS_POPUP | WS_BORDER;
		}

		/*-----------------------------------------------------------------
				Borderによるウィンドウサイズの再調整
		--------------------------------------------------------------------*/
		RECT windowRect = { 0, 0, 0, 0 };
		::AdjustWindowRectEx(&windowRect, windowDefaultStyle, false, windowExtensionStyle);

		windowCenterX += windowRect.left;
		windowCenterY += windowRect.top;
		windowWidth   += windowRect.right - windowRect.left;
		windowHeight  += windowRect.bottom - windowRect.top;
	}
	else
	{
		// ウィンドウにはエッジが上がった罫線がある 
		windowExtensionStyle = WS_EX_WINDOWEDGE; 

		// 半透明ウィンドウを出来るようにする
		if (_windowDesc.TransparencyType == core::WindowTransparency::PerWindow)
		{
			windowExtensionStyle |= WS_EX_LAYERED;
		}
		if (_windowDesc.TransparencyType == core::WindowTransparency::PerPixel)
		{
			windowExtensionStyle |= WS_EX_COMPOSITED;
		}

		// 空ウィンドウ + 親ウィンドウ内で描画される時に子ウィンドウが占有する領域を除外
		windowDefaultStyle   = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS; 
		
		// タスクバー対応 
		if (desc.AppearsInTaskbar)
		{
			windowExtensionStyle |= WS_EX_APPWINDOW;  // ウィンドウが表示されている時に, タスクバーに最上位のウィンドウを強制的に配置する
		}
		else
		{ 
			windowExtensionStyle |= WS_EX_TOOLWINDOW; // ツールバーとして使用
		}

		// 説明文を加えるときにtrueになります. 
		if (desc.IsTopmostWindow)
		{
			windowExtensionStyle |= WS_EX_TOPMOST;
		}

		// non Interactiveなウィンドウを作成したいとき
		if (!desc.AcceptInput)
		{
			windowExtensionStyle |= WS_EX_TRANSPARENT;
		}
	}

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
		parentWindow ? static_cast<HWND>(parentWindow->GetWindowHandle()) : nullptr, // parent window
		nullptr      , // menu handler 
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
	if (!RegisterTouchWindow(_hwnd, NULL))
	{
		ErrorLogger::Log(_hwnd);
	}

	/*-----------------------------------------------------------------
				強制的にtouch Feedbackをoffにする
	--------------------------------------------------------------------*/
	if (!desc.UseTouchFeedback)
	{
		DisableTouchFeedback();
	}

	/*-----------------------------------------------------------------
				最適化用のvirtual window sizeを取得する
	--------------------------------------------------------------------*/
	const auto clientWindowWidth  = static_cast<int32>(desc.DesiredScreenWidth);
	const auto clientWindowHeight = static_cast<int32>(desc.DesiredScreenHeight);
	_virtualWindowWidth  = clientWindowWidth;
	_virtualWindowHeight = clientWindowHeight;

	Resize(desc.DesiredScreenPositionX, desc.DesiredScreenPositionY, clientWindowWidth, clientWindowHeight);

	/*-----------------------------------------------------------------
				透過度の設定
	--------------------------------------------------------------------*/
	if (_windowDesc.TransparencyType == core::WindowTransparency::PerWindow)
	{
		SetOpacity(_windowDesc.Opacity);
	}

	/*-----------------------------------------------------------------
				非クライアント領域の描画無効か
	--------------------------------------------------------------------*/
	// osウィンドウの境界線を表示しない場合は、DWMレンダリングと非クライアント領域の描画を無効にする 
	// これにより、標準ウィンドウのフレームが描画されることがなくなる
	if (!_windowDesc.HasWindowOSWindowBorder)
	{
		// 非クライアント領域の描画を無効にする
		const auto renderingPolicy = DWMNCRP_DISABLED;
		Confirm(SUCCEEDED(DwmSetWindowAttribute(_hwnd, DWMWA_NCRENDERING_POLICY, &renderingPolicy, sizeof(renderingPolicy))));

		// 非クライアント領域でレンダリングされたコンテンツがDWMで描画されたフレームに表示されるようにする。
		const auto enableNCPaint = false;
		Confirm(SUCCEEDED(DwmSetWindowAttribute(_hwnd, DWMWA_ALLOW_NCPAINT, &enableNCPaint, sizeof(enableNCPaint))));

		// 後ほど修正
		if (_windowDesc.TransparencyType == core::WindowTransparency::PerPixel)
		{
			MARGINS margins = { -1 };
			Confirm(SUCCEEDED(::DwmExtendFrameIntoClientArea(_hwnd, &margins)));
		}
	}

	/*-----------------------------------------------------------------
	    非正規ウィンドウまたはosウィンドウの境界線を表示するウィンドウの領域をなくす
	--------------------------------------------------------------------*/
	if (_windowDesc.IsVanillaWindow && !_windowDesc.HasWindowOSWindowBorder)
	{
		windowDefaultStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
		if (desc.SupportMaximize) { windowDefaultStyle |= WS_MAXIMIZEBOX; } // 最大化ボタンを持つウィンドウ
		if (desc.SupportMinimize) { windowDefaultStyle |= WS_MINIMIZEBOX; } // 最小化ボタンを持つウィンドウ
		if (desc.SupportResize  ) { windowDefaultStyle |= WS_THICKFRAME;  } // サイズ変更境界を持つウィンドウを作成する

		Confirm(SetWindowLong(_hwnd, GWL_STYLE, windowDefaultStyle));

		uint32 setWindowPositionFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;
		if (_windowDesc.ActivationPolicy == core::ActivationPolicy::Never)
		{
			setWindowPositionFlags |= SWP_NOACTIVATE;
		}

		::SetWindowPos(_hwnd, nullptr, 0, 0, 0, 0, setWindowPositionFlags);

		//ゲーム以外のウィンドウでは、デフォルトのシステムメニューから閉じるメニューを削除する
		//誤ってアプリケーションアイコンをダブルクリックしてwin32アプリを閉じてしまうことを防げる
		::DeleteMenu(GetSystemMenu(_hwnd, false), SC_CLOSE, MF_BYCOMMAND);

		AdjustWindowRegion(clientWindowWidth, clientWindowHeight);
	}
	else if(_windowDesc.HasWindowOSWindowBorder && !_windowDesc.HasCloseButton)
	{
		// 持っていなければ
		EnableMenuItem(GetSystemMenu(_hwnd, false), SC_CLOSE, MF_GRAYED);
	}

	// このあと出す
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
	bool shouldActiveWindow =  _windowDesc.ActivationPolicy == core::ActivationPolicy::Always;
	if (_isFirstTimeVisible && _windowDesc.ActivationPolicy == core::ActivationPolicy::FirstOnlyShow)
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
			showWindowCommand = shouldActiveWindow ? SW_SHOWMAXIMIZED : SW_MAXIMIZE; // アクティブではある
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

/****************************************************************************
*                     Resize
*************************************************************************//**
*  @fn        void CoreWindow::Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height)
*
*  @brief     ウィンドウのサイズを変更します
*
*  @param[in] const int32 window center position x
*  @param[in] const int32 window center position y
*  @param[in] const int32 window width
*  @param[in] const int32 window height
*
*  @return    bool
*****************************************************************************/
void CoreWindow::Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height)
{
	/*---------------------------------------------------------------
					  WindowInfoの取得
	-----------------------------------------------------------------*/
	// 初期化されてないので, 無理やりゼロで初期化
	WINDOWINFO windowInfo = {};
	Memory::Zero(&windowInfo, sizeof(windowInfo));

	// window infoの取得
	windowInfo.cbSize = sizeof(windowInfo);
	::GetWindowInfo(_hwnd, &windowInfo);

	// aspect ratioの取得
	_aspectRatio = (float)width / (float)height;

	/*---------------------------------------------------------------
					  事前準備
	-----------------------------------------------------------------*/
	int32 newX      = x;
	int32 newY      = y;
	int32 newWidth  = width;
	int32 newHeight = height;

	/*---------------------------------------------------------------
					 境界があった場合のウィンドウサイズ調整
	-----------------------------------------------------------------*/
	if (_windowDesc.HasWindowOSWindowBorder)
	{
		RECT borderRectangle = { 0,0,0,0 };
		::AdjustWindowRectEx(&borderRectangle, windowInfo.dwStyle, false, windowInfo.dwExStyle);

		newX += borderRectangle.left;
		newY += borderRectangle.top;
		newWidth  += borderRectangle.right - borderRectangle.left;
		newHeight += borderRectangle.bottom - borderRectangle.top;
	}

	/*---------------------------------------------------------------
					 ウィンドウサイズの取得
	-----------------------------------------------------------------*/
	// ウィンドウサイズが頻繁に変更される場合は、ウィンドウを大きくするだけで、決して小さくしない。
	const auto virtualSizeChanged = newWidth != _virtualWindowWidth || newHeight != _virtualWindowHeight;
	_virtualWindowWidth  = newWidth;
	_virtualWindowHeight = newHeight;

	/*---------------------------------------------------------------
				   頻繁にサイズ変更がかかる場合の最適化
	-----------------------------------------------------------------*/
	// SizeWillChangeOftenが設定されている場合、幅と高さの最大ウィンドウサイズを設定します。
    // 要求された実際のウィンドウ サイズが小さくなっても、確保し続けます。 これは、ウィンドウと関連する GPU バッファのサイズを常に変更することを避けるためです。
    // プラットフォームによっては非常に遅くなります。
	if (_windowDesc.SizeWillChangeOften)
	{
		const auto oldWindowRectangle = windowInfo.rcWindow;
		const auto oldWidth           = oldWindowRectangle.right - oldWindowRectangle.left;
		const auto oldHeight          = oldWindowRectangle.bottom - oldWindowRectangle.top;

		const auto minRetainedWidth   = _windowDesc.ExpectedMaxWidth  < 0.0f ? _windowDesc.ExpectedMaxWidth : oldWidth;
		const auto minRetainedHeight = _windowDesc.ExpectedMaxHeight < 0.0f ? _windowDesc.ExpectedMaxHeight : oldHeight;

		newWidth  = max(newWidth , min(oldWidth, minRetainedWidth));
		newHeight = max(newHeight, min(oldHeight, minRetainedHeight));
	}

	if (IsMaximized()) { ReStore(); }

	/*---------------------------------------------------------------
				    ウィンドウサイズの変更
	-----------------------------------------------------------------*/
	// フルスクリーンモードではSWP_NOSENDCHANGINGを使用し、Windowsがウィンドウサイズを現在の解像度に制限するのを防ぎます。
	::SetWindowPos(_hwnd, nullptr, newX, newY, newWidth, newHeight,
		SWP_NOZORDER | SWP_NOACTIVATE | ((_windowDesc.WindowMode == core::WindowMode::FullScreen) ? SWP_NOSENDCHANGING : 0));

	const auto adjustedSizeChange = _windowDesc.SizeWillChangeOften || virtualSizeChanged;
	const auto adjustedCorners    = _windowDesc.EngineWindowType != core::EngineWindowType::Menu && _windowDesc.CornerRadius > 0;

	// まだ未実装
	if (!_windowDesc.HasWindowOSWindowBorder && (adjustedSizeChange || adjustedCorners))
	{
		AdjustWindowRegion(_virtualWindowWidth, _virtualWindowHeight);
	}
}
/****************************************************************************
*                     DisableTouchFeedback
*************************************************************************//**
*  @fn        void CoreWindow::DisableTouchFeedback()
*
*  @brief     タッチ入力を無効化します. 
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void CoreWindow::DisableTouchFeedback()
{
#if WINVER < 0x0602
	const auto userDLL = GetModuleHandle(L"user32.dll");

	/*---------------------------------------------------------------
					User32DLLが読み込まれている
	-----------------------------------------------------------------*/
	if (!userDLL) { return; }

	/*---------------------------------------------------------------
					無効化するためのフィードバックの種類
	-----------------------------------------------------------------*/
	enum FeedBackType
	{
		FEEDBACK_TOUCH_CONTACTVISUALIZATION = 1,
		FEEDBACK_PEN_BARRELVISUALIZATION = 2,
		FEEDBACK_PEN_TAP = 3,
		FEEDBACK_PEN_DOUBLETAP = 4,
		FEEDBACK_PEN_PRESSANDHOLD = 5,
		FEEDBACK_PEN_RIGHTTAP = 6,
		FEEDBACK_TOUCH_TAP = 7,
		FEEDBACK_TOUCH_DOUBLETAP = 8,
		FEEDBACK_TOUCH_PRESSANDHOLD = 9,
		FEEDBACK_TOUCH_RIGHTTAP = 10,
		FEEDBACK_GESTURE_PRESSANDTAP = 11,
		FEEDBACK_MAX = 0xFFFFFFFF
	};

	/*---------------------------------------------------------------
					関数ポインタの設定
	-----------------------------------------------------------------*/
	using SetWindowFeedbackSettingProcedure = BOOL(WINAPI*)
		(_In_ HWND hwnd,
			_In_ FeedBackType feedback,
			_In_ DWORD flags,
			_In_ UINT32 size,
			_In_reads_bytes_opt_(size) CONST VOID* configuration);

	auto SetWindowFeedbackSetting = (SetWindowFeedbackSettingProcedure)::GetProcAddress(userDLL, "SetWindowFeedbackSetting");

	/*---------------------------------------------------------------
					無効化するフィードバックの実装
	-----------------------------------------------------------------*/
	if (SetWindowFeedbackSetting)
	{
		BOOL enabled = 0;
		SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_CONTACTVISUALIZATION, 0, sizeof(enabled), &enabled);
		SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_TAP, 0         , sizeof(enabled), &enabled);
		SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_PRESSANDHOLD, 0, sizeof(enabled), &enabled);
	}
#else
	BOOL enabled = 0;
	SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_CONTACTVISUALIZATION, 0, sizeof(enabled), &enabled);
	SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_TAP, 0         , sizeof(enabled), &enabled);
	SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_PRESSANDHOLD, 0, sizeof(enabled), &enabled);
#endif
}

/****************************************************************************
*                     AdjustWindowRegion
*************************************************************************//**
*  @fn        void CoreWindow::AdjustWindowRegion(const int32 width, const int32 height)
*
*  @brief     ウィンドウ領域の調整
*
*  @param[in] const int32 width
*  @param[in] const int32 height
*
*  @return    void
*****************************************************************************/
void CoreWindow::AdjustWindowRegion(const int32 width, const int32 height)
{
	_regionWidth  = width;
	_regionHeight = height;

	HRGN regionHandle = CreateWindowRegionObject(true);
	Confirm(SetWindowRgn(_hwnd, regionHandle, false))
}

/****************************************************************************
*                     CreateWindowRegionObject
*************************************************************************//**
*  @fn        HRGN CoreWindow::CreateWindowRegionObject(const bool useBorderWhenMaximized) const
*
*  @brief     Regionオブジェクトの作成
*
*  @param[in] const bool useBorderWhenMaximizedは最大化しているときにも境界部分を使うか.
*
*  @return    HRGN 
*****************************************************************************/
HRGN CoreWindow::CreateWindowRegionObject(const bool useBorderWhenMaximized) const
{
	HRGN region = nullptr;

	/*---------------------------------------------------------------
				window regionが設定されている場合の対応
	-----------------------------------------------------------------*/
	if (_regionWidth != -1 && _regionHeight != -1)
	{
		const bool isBorderlessGameWindow = _windowDesc.EngineWindowType == core::EngineWindowType::Game 
			                             && !_windowDesc.HasWindowOSWindowBorder;
		/*---------------------------------------------------------------
					ウィンドウが最大化されている時
		-----------------------------------------------------------------*/
		if (IsMaximized())
		{
			if (isBorderlessGameWindow)
			{
				// Windowsは、ウィンドウ作成時にcxWindowBordersサイズをキャッシュする. 
				// ボーダーが削除されたり、サイズが変更されたりしても、Windowsは領域を評価したり、
				// ウィンドウのサイズを調整したりするときに、この値を使い続けます。
				// 最大化すると、ウィンドウの位置は画面の原点から (-cxWindowBorders,-cxWindowBorders) だけオフセットされます。
				// 最大化された画面領域内の領域だけを表示したいので、左上と右下を cxWindowBorders だけオフセットします。
				WINDOWINFO windowInfo;
				Memory::Zero(&windowInfo, sizeof(windowInfo));
				windowInfo.cbSize = sizeof(windowInfo);
				::GetWindowInfo(_hwnd, &windowInfo);

				const auto windowBorderSize = useBorderWhenMaximized ? windowInfo.cxWindowBorders : 0;
				region = CreateRectRgn(windowBorderSize, windowBorderSize, _regionWidth + windowBorderSize, _regionHeight + windowBorderSize);
			}
			else
			{
				const auto windowBorderSize = useBorderWhenMaximized ? GetWindowBorderSize() : 0;
				region = CreateRectRgn(windowBorderSize, windowBorderSize, _regionWidth - windowBorderSize, _regionHeight - windowBorderSize);
			}
		}
		/*---------------------------------------------------------------
					ウィンドウ平常時
		-----------------------------------------------------------------*/
		else
		{
			const bool useCornerRadius = _windowDesc.WindowMode == core::WindowMode::Windowed && !isBorderlessGameWindow &&
				                   _windowDesc.TransparencyType != core::WindowTransparency::PerPixel
				                   && _windowDesc.CornerRadius > 0;

			if (useCornerRadius)
			{
				region = CreateRoundRectRgn(0, 0, _regionWidth + 1, _regionHeight + 1, _windowDesc.CornerRadius, _windowDesc.CornerRadius);
			}
			else
			{
				region = CreateRectRgn(0, 0, _regionWidth, _regionHeight);
			}
		}
	}
	/*---------------------------------------------------------------
				window regionが未設定の場合の対応
	-----------------------------------------------------------------*/
	else
	{
		RECT windowRectangle = {};
		GetWindowRect(_hwnd, &windowRectangle);
		region = CreateRectRgn(0, 0, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top);
	}

	return region;
}

/****************************************************************************
*                     GetFullScreenInfo
*************************************************************************//**
*  @fn        bool CoreWindow::GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const
*
*  @brief     現在指定したウィンドウに最も交差しているモニターのサイズを取得します. 
*
*  @return    bool 
*****************************************************************************/
bool CoreWindow::GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const
{
	const bool isFullScreen = _windowDesc.WindowMode == core::WindowMode::FullScreen;

	/*---------------------------------------------------------------
				Monitorの情報を取得する
	-----------------------------------------------------------------*/
	// 指定したウィンドウの概説する四角形との交差領域が最も大きいディスプレイモニターへのハンドルを取得する
	// isFullScreenでは交差しない場合の戻り値を設定している.
	HMONITOR monitor = MonitorFromWindow(_hwnd, isFullScreen ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST);

	// Monitor infoを取得する
	MONITORINFO monitorInfo = {};

	Memory::Zero(&monitorInfo, sizeof(monitorInfo));
	monitorInfo.cbSize = sizeof(MONITORINFO);
	
	GetMonitorInfo(monitor, &monitorInfo);

	/*---------------------------------------------------------------
				各引数に設定を反映する
	-----------------------------------------------------------------*/
	x      = monitorInfo.rcMonitor.left;
	y      = monitorInfo.rcMonitor.top;
	width  = monitorInfo.rcMonitor.right - x;
	height = monitorInfo.rcMonitor.bottom - y;

	return true;
}

/****************************************************************************
*                     BringToFront
*************************************************************************//**
*  @fn        void BringToFront(const bool forceFront)
*
*  @brief     既に設定されたウィンドウを最前面に表示する
* 
*  @param[in] bool force front 
　　　　　　　　　 他のウィンドウからフォーカスを奪うことになっても、ウィンドウを強制的にZ順の先頭にする
		     これはゲーム・ウィンドウのように、強制的に最前面に移動させないとマウス・キャプチャやマウス・ロックが起こるが、
			 ウィンドウが表示されないようなウィンドウにのみ有効である。
*
*  @return    void
*****************************************************************************/
void CoreWindow::BringToFront(const bool forceFront)
{
	if (_windowDesc.IsVanillaWindow)
	{
		// アイコン状態になっていれば表示を行い, そうでなければアクティブ化を行う
		if (::IsIconic(_hwnd))
		{
			::ShowWindow(_hwnd, SW_RESTORE);
		}
		else
		{
			::SetActiveWindow(_hwnd);
		}
	}
	else
	{
		//http://www.freak.ne.jp/HEPOPO/lib/prog_win32/win32009.html
		// ウィンドウをZオーダーの上部に配置する
		// TOPの方は最前面ウィンドウの一つ後ろではあるが, 基本的に一番前です. TOPMOSTは完全に最前面ウィンドウとなる
		HWND   hwndInsertAfter = _windowDesc.IsTopmostWindow ? HWND_TOPMOST : HWND_TOP;
		
		// 現在の位置 + サイズ + zOrderを保持する. (zOrderはinsertAfter以外)
		uint32 flags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER;
		if (!forceFront) { flags |= SWP_NOACTIVATE; }


		::SetWindowPos(_hwnd, hwndInsertAfter, 0, 0, 0, 0, flags);
	}
}

/****************************************************************************
*                     Move
*************************************************************************//**
*  @fn        void CoreWindow::Move(const int32 x, const int32 y)
*
*  @brief     指定したx, y座標にウィンドウを移動させます
*
*  @param[in] int32 x
*  @param[in] int32 y
*
*  @return    void
*****************************************************************************/
void CoreWindow::Move(const int32 x, const int32 y)
{
	int32 correctedX = x;
	int32 correctedY = y;

	/*---------------------------------------------------------------
		　　境界があった場合の補正処理　
	-----------------------------------------------------------------*/
	if (_windowDesc.HasWindowOSWindowBorder)
	{
		const auto windowDefaultStyle   = ::GetWindowLong(_hwnd, GWL_STYLE);
		const auto windowExtensionStyle = ::GetWindowLong(_hwnd, GWL_EXSTYLE);

		// ボーダーサイズの取得
		RECT borderRectangle = { 0,0,0,0 };
		::AdjustWindowRectEx(&borderRectangle, windowDefaultStyle, false, windowExtensionStyle);

		// 実際のボーダーサイズの補正
		correctedX += borderRectangle.left;
		correctedY += borderRectangle.top;
	}

	/*---------------------------------------------------------------
		　　ウィンドウの配置
	-----------------------------------------------------------------*/
	::SetWindowPos(_hwnd, nullptr, correctedX, correctedY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}

/****************************************************************************
*                     SetWindowMode
*************************************************************************//**
*  @fn        void CoreWindow::SetWindowMode(const core::WindowMode windowMode)
*
*  @brief     ウィンドウモードの設定
*
*  @param[in] const core::WindowMode windowMode
*
*  @return    void
*****************************************************************************/
void CoreWindow::SetWindowMode(const core::WindowMode windowMode)
{
	if (_windowDesc.WindowMode == windowMode) { return; }

	/*---------------------------------------------------------------
				ウィンドウの設定
	-----------------------------------------------------------------*/
	const auto previousWindowMode = _windowDesc.WindowMode;
	_windowDesc.WindowMode        = windowMode;

	auto windowDefaultStyle  = GetWindowLong(_hwnd, GWL_STYLE);
	auto windowedModeStyle   = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
	auto fullScreenModeStyle = WS_POPUP;
	const auto isTrueFullScreen = windowMode == core::WindowMode::FullScreen;

	if (_windowDesc.IsVanillaWindow)
	{
		if (_windowDesc.SupportMaximize) { windowDefaultStyle |= WS_MAXIMIZEBOX; }
		if (_windowDesc.SupportMinimize) { windowDefaultStyle |= WS_MINIMIZEBOX; }
		if (_windowDesc.SupportResize  ) { windowDefaultStyle |= WS_THICKFRAME;  } // サイズ変更境界を持つウィンドウを作成する
		else                             { windowDefaultStyle |= WS_BORDER;      } // 境界線を持つウィンドウを作成する
	}
	else
	{
		windowDefaultStyle |= WS_POPUP | WS_BORDER;
	}

	/*---------------------------------------------------------------
				フルスクリーン
	-----------------------------------------------------------------*/
	if (windowMode == core::WindowMode::BorderLess || windowMode == core::WindowMode::FullScreen)
	{
		/*---------------------------------------------------------------
				フルスクリーンになる前にウィンドウの配置を記憶しておく
		-----------------------------------------------------------------*/
		if (previousWindowMode == core::WindowMode::Windowed)
		{
			_previousFullScreenWindowPlacement.length = sizeof(WINDOWPLACEMENT);
			::GetWindowPlacement(_hwnd, &_previousFullScreenWindowPlacement);
		}

		/*---------------------------------------------------------------
				ウィンドウのスタイルを変更する
		-----------------------------------------------------------------*/
		windowDefaultStyle &= ~windowedModeStyle;
		windowDefaultStyle |= fullScreenModeStyle;

		SetWindowLong(_hwnd, GWL_STYLE, windowDefaultStyle);
		::SetWindowPos(_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		if (windowMode == core::WindowMode::BorderLess)
		{
			// ボーダーレスの場合は, ウィンドウを復元できるようにしておく
			::ShowWindow(_hwnd, SW_RESTORE);
		}

		/*---------------------------------------------------------------
				モニター情報の取得
		-----------------------------------------------------------------*/
		// 現在のウィンドウの位置を取得する
		RECT clientRectangle;
		GetClientRect(_hwnd, &clientRectangle);

		// モニター情報の取得
		HMONITOR    monitor = MonitorFromWindow(_hwnd, isTrueFullScreen? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST);
		
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &monitorInfo);

		/*---------------------------------------------------------------
				モニターサイズの決定
		-----------------------------------------------------------------*/
		const auto monitorWidth       = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
		const auto monitorHeight      = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
		const auto targetClientWidth  = isTrueFullScreen ? min(monitorWidth, clientRectangle.right - clientRectangle.left) : monitorWidth;
		const auto targetClientHeight = isTrueFullScreen ? min(monitorHeight, clientRectangle.bottom - clientRectangle.top) : monitorHeight;

		/*---------------------------------------------------------------
				サイズを変更する
		-----------------------------------------------------------------*/
		Resize(monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, targetClientWidth, targetClientHeight);
	}
	/*---------------------------------------------------------------
				Windowed
	-----------------------------------------------------------------*/
	else
	{
		windowDefaultStyle &= ~fullScreenModeStyle;
		windowDefaultStyle |= windowedModeStyle;

		SetWindowLong(_hwnd, GWL_STYLE, windowDefaultStyle);
		::SetWindowPos(_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		if (_previousFullScreenWindowPlacement.length)
		{
			::SetWindowPlacement(_hwnd, &_previousFullScreenWindowPlacement);
		}

		HICON icon = (HICON)::GetClassLong(_hwnd, GCLP_HICON);
		if (icon != nullptr)
		{
			::SendMessageW(_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
		}
	}
}

/****************************************************************************
*                     GetWindowBorderSize
*************************************************************************//**
*  @fn        int32 CoreWindow::GetWindowBorderSize() const
*
*  @brief     ウィンドウの境界部分のサイズを返す
*
*  @param[in] void
*
*  @return    int32 border size
*****************************************************************************/
int32 CoreWindow::GetWindowBorderSize() const
{
	/*---------------------------------------------------------------
		　　　　ボーダーレスウィンドウであれば0を返す
	-----------------------------------------------------------------*/
	if ( _windowDesc.EngineWindowType == core::EngineWindowType::Game && 
		!_windowDesc.HasWindowOSWindowBorder)
	{
		return 0;
	}

	/*---------------------------------------------------------------
		　　　　ボーダーレスウィンドウ以外は境界を返す
	-----------------------------------------------------------------*/
	WINDOWINFO windowInfo = {};
	Memory::Zero(&windowInfo, sizeof(windowInfo));
	windowInfo.cbSize = sizeof(windowInfo);
	::GetWindowInfo(_hwnd, &windowInfo);
	
	return windowInfo.cxWindowBorders;
}

/****************************************************************************
*                     GetWindowTitleBarSize
*************************************************************************//**
*  @fn        int32 CoreWindow::GetWindowTitleBarSize() const
*
*  @brief     ウィンドウのタイトルバーのサイズを返す
*
*  @param[in] void
*
*  @return    int32 size
*****************************************************************************/
int32 CoreWindow::GetWindowTitleBarSize() const
{
	return ::GetSystemMetrics(SM_CYCAPTION);
}
/****************************************************************************
*                     GetRestoredDimensions
*************************************************************************//**
*  @fn        bool CoreWindow::GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height)
*
*  @brief     サイズとウィンドウの位置を取得します
*
*  @return   bool
*****************************************************************************/
bool CoreWindow::GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height)
{
	/*---------------------------------------------------------------
		　　　　　Windowの配置を取得する
	-----------------------------------------------------------------*/
	WINDOWPLACEMENT windowPlacement;
	Memory::Zero(&windowPlacement, sizeof(windowPlacement));
	windowPlacement.length = sizeof(WINDOWPLACEMENT);

	if (::GetWindowPlacement(_hwnd, &windowPlacement) == 0) { return false; }

	/*---------------------------------------------------------------
		　　このウィンドウは最小化されている可能性があります。  元に戻したときの位置を取得します。
	-----------------------------------------------------------------*/
	const auto restoredWindow = windowPlacement.rcNormalPosition;

	x      = restoredWindow.left;
	y      = restoredWindow.top;
	width  = restoredWindow.right  - restoredWindow.left;
	height = restoredWindow.bottom - restoredWindow.top;

	// WS_EX_TOOLWINDOWウィンドウスタイルを持たないウィンドウの場合 
	// WindowPlacement.rcNormalPositionはワークスペース座標なので、スクリーン座標に変換する必要がある。
	const auto windowExtensionStyle = ::GetWindowLong(_hwnd, GWL_EXSTYLE);
	if ((windowExtensionStyle & WS_EX_TOOLWINDOW) == 0)
	{
		const bool isTrueFullscreen = (_windowDesc.WindowMode == core::WindowMode::FullScreen);
		const auto monitor = MonitorFromWindow(_hwnd, isTrueFullscreen ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST);
		
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &monitorInfo);

		x += monitorInfo.rcWork.left - monitorInfo.rcMonitor.left;
		y += monitorInfo.rcWork.top  - monitorInfo.rcMonitor.top;
	}

	return true;
}

/****************************************************************************
*                     ExistPointInWindow
*************************************************************************//**
*  @fn        bool CoreWindow::ExistPointInWindow(const gu::int32 x, const gu::int32 y) const
*
*  @brief     指定した位置がウィンドウ中に存在するかを判定します.
*
*  @param[in] const gu::int32 x
*  @param[in] const gu::int32 y
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::ExistPointInWindow(const gu::int32 x, const gu::int32 y) const
{
	const auto region = CreateWindowRegionObject(false);
	const auto result = !!PtInRegion(region, x, y);
	DeleteObject(region);
	
	return result;
}

/****************************************************************************
*                     SetOpacity
*************************************************************************//**
*  @fn        void CoreWindow::SetOpacity(const float opacity) const
*
*  @brief     不透明度を0～1の範囲で設定します
*
*  @param[in] const float opacity (不透明度0～1)
* 
*  @return    void
*****************************************************************************/
void CoreWindow::SetOpacity(const float opacity) const
{
	const auto adjustOpacity = max(0, min(opacity, 255));
	SetLayeredWindowAttributes(_hwnd, 0, adjustOpacity, LWA_ALPHA);
}

/****************************************************************************
*                     SetKeyboardFocus
*************************************************************************//**
*  @fn        void CoreWindow::SetKeyboardFocus()
*
*  @brief      KeyboardFocusにこのウィンドウを設定する
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void CoreWindow::SetKeyboardFocus()
{
	if (GetFocus() != _hwnd) { ::SetFocus(_hwnd); }
}

/****************************************************************************
*                     SetKeyboardFocus
*************************************************************************//**
*  @fn        void CoreWindow::SetKeyboardFocus()
*
*  @brief     親ウィンドウが最小化されたときに呼び出される関数
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::OnParentWindowMinimized()
{
	return !!::GetWindowPlacement(_hwnd, &_previousParentMinimizedWindowPlacement);
}

/****************************************************************************
*                     SetKeyboardFocus
*************************************************************************//**
*  @fn        void CoreWindow::SetKeyboardFocus()
*
*  @brief     親ウィンドウが最小化されたときに呼び出される関数
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::OnParentWindowRestored()
{
	return !!::SetWindowPlacement(_hwnd, &_previousParentMinimizedWindowPlacement);
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

/****************************************************************************
*                     OnTransparenySupportChanged
*************************************************************************//**
*  @fn        void CoreWindow::OnTransparenySupportChanged(const core::WindowTransparency transparency)
*
*  @brief     透過度の設定
*
*  @param[in] const core::WindowTransparency 透過度の設定
*
*  @return    void
*****************************************************************************/
void CoreWindow::OnTransparenySupportChanged(const core::WindowTransparency transparency)
{
	if (transparency != core::WindowTransparency::PerPixel) { return; }

	/*---------------------------------------------------------------
		　　　　　ピクセル毎に透過度を設定できた場合の処理
	-----------------------------------------------------------------*/
	const auto windowExtensionStyle = GetWindowLong(_hwnd, GWL_EXSTYLE);

	if (transparency == core::WindowTransparency::PerPixel)
	{
		SetWindowLong(_hwnd, GWL_EXSTYLE, windowExtensionStyle | WS_EX_COMPOSITED);

		MARGINS margins = { -1 };
		Confirm(SUCCEEDED(::DwmExtendFrameIntoClientArea(_hwnd, &margins)));
	}
	else
	{
		SetWindowLong(_hwnd, GWL_EXSTYLE, windowExtensionStyle & ~WS_EX_COMPOSITED);
	}

	//内部キャッシュを更新するため、ウィンドウのスタイル属性を更新するときにSWP_FRAMECHANGEDを呼び出す必要がある（MSDNによる）
	SetWindowPos(_hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOSENDCHANGING | SWP_NOZORDER);
}
#pragma endregion Supported Check Function

#endif