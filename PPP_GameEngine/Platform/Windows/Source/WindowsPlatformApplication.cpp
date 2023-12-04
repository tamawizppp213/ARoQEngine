//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#if PLATFORM_OS_WINDOWS
#include "../../Windows/Include/WindowsWindowsMessageHandler.hpp"
#include "../../Windows/Include/WindowsWindow.hpp"
#include "../../Windows/Include/WindowsPlatformCommand.hpp"
#include "../../Core/Include/CoreCommonState.hpp"
#include "../../Windows/Include/WindowsError.hpp"
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;
using namespace gu;

namespace
{
	using GetDPIForMonitorProcedure = HRESULT(WINAPI*)(HMONITOR monitor, int32 dpiType, uint32* dpiX, uint32* dpiY);
	GetDPIForMonitorProcedure GetDPIForMonitor = nullptr;

	HMODULE SHCoreDLL = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
static constexpr LPCWSTR g_ApplicationName = L"Game Window";

namespace
{
	gu::SharedPointer<windows::CoreWindow> FindWindowByHWND(const std::vector<gu::SharedPointer<windows::CoreWindow>>& searchWindows, HWND handle)
	{
		for (int i = 0; i < searchWindows.size(); ++i)
		{
			if (searchWindows[i]->GetHWND() == handle)
			{
				return searchWindows[i];
			}
		}

		return nullptr;
	}
}

#pragma region Constructor and Destructor
PlatformApplication::PlatformApplication() : core::PlatformApplication()
{
	/*---------------------------------------------------------------
					Create instance handle
	-----------------------------------------------------------------*/
	_instanceHandle = GetModuleHandleA(NULL);
	assert(("_instanceHandle is nullptr.", _instanceHandle));

	/*---------------------------------------------------------------
					Create window class
	-----------------------------------------------------------------*/
	RegisterWindowClass();

	printf_s("Windows application is created\n");

	/*---------------------------------------------------------------
					Create message handler
	-----------------------------------------------------------------*/
	_messageHandler = gu::MakeShared<windows::CoreWindowMessageHandler>();

	/*-----------------------------------------------------------------
					  高DPIが設定可能かどうか
	--------------------------------------------------------------------*/
	_enableHighDPIMode = SetHighDPIMode();
}

PlatformApplication::~PlatformApplication()
{
	if (SHCoreDLL) { ::FreeLibrary(SHCoreDLL); }
	_windows.clear();
	_windows.shrink_to_fit();
	_messageList.clear();
	_messageList.shrink_to_fit();

	::CoUninitialize();

	printf_s("Windows application is destroyed\n");
}

#pragma endregion Contructor and Destructor

#pragma region Main Function

LRESULT CALLBACK PlatformApplication::StaticWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PlatformApplication* application = (windows::PlatformApplication*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
	if (application)
	{
		application->ApplicationWindowMessageProcedure(hwnd, message, wParam, lParam);
	}
	else
	{
		if (message == WM_CREATE)
		{ 
			const auto createStruct = (LPCREATESTRUCT)lParam;
			application = (PlatformApplication*)(createStruct)->lpCreateParams;
		}
		if (application)
		{
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)application);
			application->ApplicationWindowMessageProcedure(hwnd, message, wParam, lParam);
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

bool PlatformApplication::RegisterWindowClass()
{
	/*---------------------------------------------------------------
						Register Window Class
			http://kaitei.net/winapi/creating-windows/
	-----------------------------------------------------------------*/
	// Settings
	WNDCLASS windowClass      = {};
	windowClass.style         = CS_DBLCLKS;       // enable to use double click
	windowClass.lpfnWndProc   = StaticWindowProcedure;  // window procedure. event driven 
	windowClass.cbClsExtra    = 0;                // window class構造体の後に確保する領域のバイト数
	windowClass.cbWndExtra    = 0;                // windowInstanceの後に確保する領域のバイト数
	windowClass.hInstance     = _instanceHandle;  // lpfnWndProcで指定した関数が含まれるインスタンスへのハンドルを指定
	windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION); // デフォルトアイコン
	windowClass.hCursor       = NULL;             // 別途管理する
	windowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); // 背景は透明にする
	windowClass.lpszMenuName  = NULL;
	windowClass.lpszClassName = APPLICATION_NAME;

	// Register
	if (!::RegisterClass(&windowClass))
	{
		MessageBox(NULL, L"Failed to register class.", NULL, NULL);
		return false;
	}

	return true;
}

/****************************************************************************
*                     MakeWindow
*************************************************************************//**
*  @fn        gu::SharedPointer<core::CoreWindow> PlatformApplication::MakeWindow()
*
*  @brief     新規のウィンドウインスタンスを作成します. ここではセットアップは行いません
*
*  @param[in] void
*
*  @return    gu::SharedPointer<core::CoreWindow>
*****************************************************************************/
SharedPointer<core::CoreWindow> PlatformApplication::MakeWindow()
{
	return StaticPointerCast<core::CoreWindow>(MakeShared<windows::CoreWindow>());
}

/****************************************************************************
*                     MakeCommand
*************************************************************************//**
*  @fn        gu::SharedPointer<core::PlatformCommand> PlatformApplication::MakeCommand()
*
*  @brief     新規でコマンドをまとめたクラスのインスタンスを作成します.
*
*  @param[in] void
*
*  @return    gu::SharedPointer<core::PlatformCommand>
*****************************************************************************/
SharedPointer<core::PlatformCommand> PlatformApplication::MakeCommand()
{
	return StaticPointerCast<core::PlatformCommand>(MakeShared<windows::PlatformCommand>());
}

/****************************************************************************
*                     SetUpWindow
*************************************************************************//**
*  @fn        void PlatformApplication::SetUpWindow(const gu::SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc)
*
*  @brief     指定のウィンドウを実際に作成するまで行います. セットアップも行います
*
*  @param[in] const gu::SharedPointer<core::CoreWindow>& window, 特定のウィンドウ
*  @param[in] const core:CoreWidowDesc& desc, 設定
*
*  @return    void
*****************************************************************************/
void PlatformApplication::SetUpWindow(const SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc)
{
	assert(("window is nullptr", window));

	_windows.push_back(StaticPointerCast<windows::CoreWindow>(window));
	
	const auto windowsCoreWindow = StaticPointerCast<windows::CoreWindow>(window);
	windowsCoreWindow->Create(StaticPointerCast<core::PlatformApplication>(SharedFromThis()), desc);
}

/****************************************************************************
*                     PumpMessage
*************************************************************************//**
*  @fn        bool PlatformApplication::PumpMessage()
*
*  @brief     メッセージを出します. この関数は仮想キーメッセージが受け取られ, それをメッセージ形式に変換した時にtrueを返します
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool PlatformApplication::PumpMessage()
{
	if (PeekMessage(&_windowMessage, NULL, 0, 0, PM_REMOVE))
	{
		/*-----------------------------------------------------------------
				メッセージの変換と割り当て
		--------------------------------------------------------------------*/
		TranslateMessage(&_windowMessage);
		DispatchMessage(&_windowMessage);

		/*-----------------------------------------------------------------
		        アプリケーションの終了を検知する
		--------------------------------------------------------------------*/
		if(!_isApplicationQuited && _windowMessage.message == WM_QUIT)
		{
			_isApplicationQuited = true;
		}
		return true;
	}
	else
	{
		return false;
	}
}

/****************************************************************************
*                     ApplicationWindowMessageProcedure
*************************************************************************//**
*  @fn        LRESULT PlatformApplication::ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
*
*  @brief     メンバ関数で呼び出せるようにした (StaticWindowProcedureの代わり)
*
*  @param[in] HWND windowHandle
*  @param[in] UINT messageCode
*  @param[in] WPARAM wParam
*  @param[in] LPARAM lParam
*
*  @return    LRESULT
*****************************************************************************/
LRESULT PlatformApplication::ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_allowedToDeferredMessageProcessing)
	{
		_messageList.push_back(DeferredMessage(hwnd, message, wParam, lParam));
	}
	else
	{
		return ProcessDeferredWindowsMessage(DeferredMessage(hwnd, message, wParam, lParam));
	}

	return DefWindowProc(hwnd, message, wParam, lParam); // default window procedure
}

LRESULT PlatformApplication::ProcessDeferredWindowsMessage(const DeferredMessage& message)
{
	const auto window      = FindWindowByHWND(_windows, message.WindowHandle);
	const auto messageCode = message.MessageCode;

	if (!window) { return 0; }

	switch (messageCode)
	{
		/*-----------------------------------------------------------------
		  Activated or deactivatedの時に送られる
		--------------------------------------------------------------------*/
		case WM_ACTIVATE:
		{
			break;
		}
		/*-----------------------------------------------------------------
		  アクティブなウィンドウとは異なるアプリケーションに属するウィンドウがアクティブ化される時に送られる
		--------------------------------------------------------------------*/
		case WM_ACTIVATEAPP:
		{
			break;
		}
		/*-----------------------------------------------------------------
		  ウィンドウサイズが変わったときに送られる
		--------------------------------------------------------------------*/
		case WM_SIZE:
		{
			/*-----------------------------------------------------------------
			   fullscreen window rendering size should be determined by the application. 
			   Do not adjust based on WM_SIZE message.
			--------------------------------------------------------------------*/
			if (window->GetWindowMode() == core::WindowMode::FullScreen)
			{
				return 0;
			}

			_messageHandler->OnSizeChanged(LOWORD(message.LParam), HIWORD(message.LParam));

			switch (message.WParam)
			{
				case SIZE_RESTORED:
				{

					break;
				}
				case SIZE_MINIMIZED:
				{
					break;
				}
				case SIZE_MAXSHOW:
				{
					break;
				}
				case SIZE_MAXIMIZED:
				{
					break;
				}
				case SIZE_MAXHIDE:
				{
					break;
				}
				default:
				{
					break;
				}
			}
			return 0;
		}
		/*-----------------------------------------------------------------
		  ウィンドウサイズを変更しているウィンドウに送信
		--------------------------------------------------------------------*/
		case WM_SIZING:
		{
			return 0;
		}
		/*-----------------------------------------------------------------
		  DPIが変化した時に送信
		--------------------------------------------------------------------*/
		case WM_DPICHANGED:
		{
			return 0;
		}
		/*-----------------------------------------------------------------
			WM_CLOSE is sent when the window is closed
		--------------------------------------------------------------------*/
		case WM_CLOSE:
		{
			_messageHandler->OnWindowClosed(gu::StaticPointerCast<core::CoreWindow>(window));
			return 0;
		}
		/*-----------------------------------------------------------------
			WM_DESTROY is sent when the window is deleted
		--------------------------------------------------------------------*/
		case WM_DESTROY:
		{
			std::erase(_windows, window);
			if (_windows.empty())
			{
				PostQuitMessage(0);
			}
			return 0;
		}
	}

	return DefWindowProc(message.WindowHandle, message.MessageCode, message.WParam, message.LParam); // default window procedure
}

/****************************************************************************
*                     ProcessDeferredEvents
*************************************************************************//**
*  @fn        void PlatformApplication::ProcessDeferredEvents()
*
*  @brief     まとめて一括でためたイベントを発行する
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void PlatformApplication::ProcessDeferredEvents()
{
	for (int i = 0; i < _messageList.size(); ++i)
	{
		ProcessDeferredWindowsMessage(_messageList[i]);
	}

	_messageList.clear();
	_messageList.shrink_to_fit();
}

#pragma endregion Main Function

#pragma region Monitor
/****************************************************************************
*                     GetMonitorDPI
*************************************************************************//**
*  @fn        gu::int32 PlatformApplication::GetMonitorDPI(const core::MonitorInfo& monitorInfo) const
*
*  @brief     モニターのDPIを取得します
*             https://zenn.dev/tenka/articles/windows_display_monitor_dpi
*
*  @param[in] const core::MonitorInfo
*
*  @return    gu::int32 DPI
*****************************************************************************/
gu::int32 PlatformApplication::GetMonitorDPI(const core::MonitorInfo& monitorInfo) const
{	
	if (!EnableHighDPIAwareness()) { return USER_DEFAULT_SCREEN_DPI; }

	int32 displayDPI = USER_DEFAULT_SCREEN_DPI;

	if (GetDPIForMonitor)
	{
		const RECT monitorDimenstion =
		{
			.left   = monitorInfo.ActualRectangle.Left,
			.top    = monitorInfo.ActualRectangle.Top,
			.right  = monitorInfo.ActualRectangle.Right,
			.bottom = monitorInfo.ActualRectangle.Bottom
		};
		
		uint32 dpiX = 0, dpiY = 0;
		HMONITOR monitor = MonitorFromRect(&monitorDimenstion, MONITOR_DEFAULTTONEAREST);
		if(monitor && SUCCEEDED(GetDPIForMonitor(monitor, 0, &dpiX, &dpiY)))
		{
			displayDPI = dpiX;
		}
	}
	else
	{
		auto displayContext = GetDC(nullptr);
		displayDPI = GetDeviceCaps(displayContext, LOGPIXELSX);
		ReleaseDC(nullptr, displayContext);
	}

	return displayDPI;
}

/****************************************************************************
*                     SetHighDPIMode
*************************************************************************//**
*  @fn        void CoreApplication::SetHighDPIMode()
*
*  @brief     高DPIの設定に書き換えます.
*             https://zenn.dev/tenka/articles/windows_display_monitor_dpi
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool PlatformApplication::SetHighDPIMode()
{
	/*---------------------------------------------------------------
					  DLLの読み込み
	-----------------------------------------------------------------*/
	const auto userDLL   = LoadLibrary(L"user32.dll");
	SHCoreDLL = LoadLibrary(L"shcore.dll");

	/*---------------------------------------------------------------
					  DPIモードの種類設定
	-----------------------------------------------------------------*/
	enum Process_DPI_Awareness
	{
		Process_DPI_UnAware           = 0,
		Process_System_DPI_Aware      = 1,
		Process_Per_Monitor_DPI_Aware = 2
	};

	/*---------------------------------------------------------------
			      DPIモードの取得 : Win 8.1 and later
	-----------------------------------------------------------------*/
	if (SHCoreDLL)
	{
		// 関数ポインタの登録, 返り値(ポインタ)(引数)で取得できる
		using GetProcessDPIAwarenessProcedure = HRESULT(WINAPI*)(HANDLE process, Process_DPI_Awareness* awareness);
		using SetProcessDPIAwarenessProcedure = HRESULT(WINAPI*)(Process_DPI_Awareness value);

		auto SetProcessDPIAwareness = (SetProcessDPIAwarenessProcedure)::GetProcAddress(SHCoreDLL, "SetProcessDpiAwareness");
		auto GetProcessDPIAwareness = (GetProcessDPIAwarenessProcedure)::GetProcAddress(SHCoreDLL, "GetProcessDpiAwareness");
		GetDPIForMonitor            = (GetDPIForMonitorProcedure)      ::GetProcAddress(SHCoreDLL, "GetDpiForMonitor");

		if (!GetProcessDPIAwareness) { ::FreeLibrary(SHCoreDLL); return false; }
		if (!SetProcessDPIAwareness) { ::FreeLibrary(SHCoreDLL); return false; }

		// awarenessの設定 (DPIモードの設定: MSが推奨しているperMonitorに変更する)
		Process_DPI_Awareness currentAwareness = Process_DPI_UnAware;
		GetProcessDPIAwareness(nullptr, &currentAwareness);

		if (currentAwareness != Process_Per_Monitor_DPI_Aware)
		{
			OutputDebugString(L"Setting process to per monitor DPI aware\n");
			
			SetProcessDPIAwareness(Process_Per_Monitor_DPI_Aware);

			return ErrorLogger::Succeed();
		}
		else
		{
			return true;
		}
	}
	/*---------------------------------------------------------------
				 DPIモードの取得 :For vista, win7, and win8
	-----------------------------------------------------------------*/
	else if (userDLL)
	{
		// 関数ポインタの登録, 返り値(ポインタ)(引数)で取得出来る
		using SetProcessDPIAwareProcedure = BOOL(WINAPI*)(void);
		auto SetProcessDPIAware = (SetProcessDPIAwareProcedure)::GetProcAddress(userDLL, "SetProcessDPIAware");

		// 関数の実行
		if (SetProcessDPIAware) 
		{
			return SetProcessDPIAware();
		}
		else
		{
			::OutputDebugString(L"Failed to set dpi mode\n");
			return false;
		}
	}
	else
	{
		return false;
	}
}

/****************************************************************************
*                     GetDPIScaleFactorAtPixelPoint
*************************************************************************//**
*  @fn        float CoreApplication::GetDPIScaleFactorAtPixelPoint(const float x, const float y) const
*
*  @brief     あるピクセル位置でのDPIの拡大率を示します.
*
*  @param[in] const float x
*  @param[in] const float y
*
*  @return    float
*****************************************************************************/
float PlatformApplication::GetDPIScaleFactorAtPixelPoint(const float x, const float y) const
{
	float scale = 1.0f;

	if (GetDPIForMonitor)
	{
		POINT    position = { static_cast<LONG>(x), static_cast<LONG>(y) };
		HMONITOR monitor  = MonitorFromPoint(position, MONITOR_DEFAULTTONEAREST);

		uint32 dpiX = 0, dpiY = 0;
		if (monitor && SUCCEEDED(GetDPIForMonitor(monitor, 0, &dpiX, &dpiY)))
		{
			scale = (float)dpiX / USER_DEFAULT_SCREEN_DPI;
		}
	}
	else
	{
		auto displayContext = GetDC(nullptr);
		int32 displayDPI    = GetDeviceCaps(displayContext, LOGPIXELSX);
		scale = (float)displayDPI / USER_DEFAULT_SCREEN_DPI;
		ReleaseDC(nullptr, displayContext);
	}

	return scale;
}
#pragma endregion Monitor
#endif