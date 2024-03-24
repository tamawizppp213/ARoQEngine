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
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Base/Include/GUString.hpp"

#include <SetupAPI.h>
#include <cfgmgr32.h>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "cfgmgr32.lib")
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
	gu::SharedPointer<windows::CoreWindow> FindWindowByHWND(const gu::DynamicArray<gu::SharedPointer<windows::CoreWindow>>& searchWindows, HWND handle)
	{
		for (int i = 0; i < searchWindows.Size(); ++i)
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
	windows::PlatformCommand::CoInitialize();

	_instanceHandle = GetModuleHandleA(NULL);
	Checkf(_instanceHandle, "_instanceHandle is nullptr.\n");

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
	_windows.Clear();
	_windows.ShrinkToFit();
	_messageList.Clear();
	_messageList.ShrinkToFit();

	windows::PlatformCommand::CoUnInitialize();

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

/****************************************************************************
*                     RegisterWindowClass
*************************************************************************//**
*  @fn        bool PlatformApplication::RegisterWindowClass()
*
*  @brief     ウィンドウクラスを登録します
* 
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool PlatformApplication::RegisterWindowClass() const
{
	/*---------------------------------------------------------------
						Register Window Class
			http://kaitei.net/winapi/creating-windows/
	-----------------------------------------------------------------*/
	// Settings
	WNDCLASS windowClass      = {};
	windowClass.style         = CS_DBLCLKS;                      // enable to use double click
	windowClass.lpfnWndProc   = StaticWindowProcedure;           // window procedure. event driven 
	windowClass.cbClsExtra    = 0;                               // window class構造体の後に確保する領域のバイト数
	windowClass.cbWndExtra    = 0;                               // windowInstanceの後に確保する領域のバイト数
	windowClass.hInstance     = _instanceHandle;                 // lpfnWndProcで指定した関数が含まれるインスタンスへのハンドルを指定
	windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION); // デフォルトアイコン
	windowClass.hCursor       = NULL;                            // 別途管理する
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
*  @param[in] const gu::SharedPointer<core::CoreWindow>& parentWindow 親のウィンドウ 
*
*  @return    void
*****************************************************************************/
void PlatformApplication::SetUpWindow(const SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow)
{
	Checkf(window, "window is nullptr");

	_windows.Push(StaticPointerCast<windows::CoreWindow>(window));
	
	const auto windowsCoreWindow = StaticPointerCast<windows::CoreWindow>(window);
	windowsCoreWindow->Create(StaticPointerCast<core::PlatformApplication>(SharedFromThis()), desc, parentWindow);
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
		_messageList.Push(DeferredMessage(hwnd, message, wParam, lParam));
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

	if (!window)          { return 0; }
	if (!_messageHandler) { return 0; }

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

			_messageHandler->OnSizeChanged(window, LOWORD(message.LParam), HIWORD(message.LParam));

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
			_windows.Remove(window);
			if (_windows.IsEmpty())
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
	for (int i = 0; i < _messageList.Size(); ++i)
	{
		ProcessDeferredWindowsMessage(_messageList[i]);
	}

	_messageList.Clear();
	_messageList.ShrinkToFit();
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
			.left   = monitorInfo.DisplayRectangle.Left,
			.top    = monitorInfo.DisplayRectangle.Top,
			.right  = monitorInfo.DisplayRectangle.Right,
			.bottom = monitorInfo.DisplayRectangle.Bottom
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

/****************************************************************************
*                     GetMonitorsInfo
*************************************************************************//**
*  @fn        void PlatformApplication::GetMonitorsInfo(gu::DynamicArray<core::MonitorInfo>& monitorInfo) const
*
*  @brief     モニターの情報を取得する
*
*  @param[out] gu::DynamicArray<core::MOnitorInfo> モニターの情報リスト
* 
*  @return    void
*****************************************************************************/
void PlatformApplication::GetMonitorsInfo(gu::DynamicArray<core::MonitorInfo>& monitorInfos) const
{
	/*---------------------------------------------------------------
				 事前準備
	-----------------------------------------------------------------*/
	// モニターの数を見つける用
	DISPLAY_DEVICE device;
	Memory::Zero(&device, sizeof(device));
	device.cb = sizeof(device);

	// モニター情報取得用
	DISPLAY_DEVICE monitor;
	Memory::Zero(&monitor, sizeof(monitor));
	monitor.cb = sizeof(monitor);

	/*---------------------------------------------------------------
				 ディスプレイアダプター取得
	-----------------------------------------------------------------*/
	for (DWORD displayIndex = 0; EnumDisplayDevices(nullptr, displayIndex, &device, 0); 
		++displayIndex, Memory::Zero(&device, sizeof(device)), device.cb = sizeof(device)) // 終了時
	{
		// デスクトップに接続されているもの以外は無視
		if ((device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) <= 0) { continue; }

		/*---------------------------------------------------------------
					 モニターの取得
		-----------------------------------------------------------------*/
		for (DWORD monitorIndex = 0; EnumDisplayDevices(device.DeviceName, monitorIndex, &monitor, 0);
			++monitorIndex, Memory::Zero(&monitor, sizeof(monitor)), monitor.cb = sizeof(monitor))
		{
			// アクティブなデバイス以外は無視 + 非表示の疑似デバイスも無視
			if (!(monitor.StateFlags & DISPLAY_DEVICE_ACTIVE         )) { continue; }
			if (  monitor.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) { continue; }

			/*---------------------------------------------------------------
						 core::MonitorInfoに情報を格納
			-----------------------------------------------------------------*/
			core::MonitorInfo monitorInfo = {};

			EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProcedure, (LPARAM)&monitorInfo);
			
			// モニターのデバイス名
			monitorInfo.Name      = device.DeviceName;

			// monitorのID
			monitorInfo.ID        = monitor.DeviceID;
			
			// Primary monitorかどうか
			monitorInfo.IsPrimary = (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) > 0;

			// モニターの物理的なサイズを取得する
			const auto hdc = CreateDC(device.DeviceName, device.DeviceName, nullptr, nullptr);
			const auto physicalWidth  = GetDeviceCaps(hdc, HORZSIZE) / 10.0f; //[cm]単位
			const auto physicalHeight = GetDeviceCaps(hdc, VERTSIZE) / 10.0f;
			const auto pixelWidth     = GetDeviceCaps(hdc, HORZRES); // ピクセル横幅
			const auto pixelHeight    = GetDeviceCaps(hdc, VERTRES); // ピクセル縦幅   
			const auto inch           = sqrtf(physicalHeight * physicalHeight + physicalWidth * physicalWidth) / 2.54f;
			
			// モニターの物理的なDPIを取得する
			const auto dpi = sqrtf((float)pixelWidth * pixelWidth + (float)pixelHeight * pixelHeight) / inch;

			// モニターのDPIを取得する
			if (dpi < 96 || dpi > 300)
			{
				monitorInfo.DPI = GetMonitorDPI(monitorInfo);
			}
			else
			{
				const auto centerX = 0.5f * float(monitorInfo.WorkArea.Right + monitorInfo.WorkArea.Left);
				const auto centerY = 0.5f * float(monitorInfo.WorkArea.Top + monitorInfo.WorkArea.Bottom);
				const auto dpiScale = GetDPIScaleFactorAtPixelPoint(centerX, centerY);
				monitorInfo.DPI = (int32)((float)monitorInfo.DPI * dpiScale);
			}

			// モニターの幅と高さを取得する
			monitorInfo.NativeWidth = pixelWidth;
			monitorInfo.NativeHeight = pixelHeight;

			monitorInfos.Push(monitorInfo);

			DeleteDC(hdc);
		}
		
	}
}


BOOL CALLBACK PlatformApplication::MonitorEnumProcedure(HMONITOR monitor, [[maybe_unused]]HDC monitorDC, [[maybe_unused]]LPRECT rect, LPARAM userData)
{
	MONITORINFOEX monitorInfoExtension = {};
	monitorInfoExtension.cbSize = sizeof(monitorInfoExtension);
	GetMonitorInfo(monitor, &monitorInfoExtension);

	auto monitorInfo = (core::MonitorInfo*)userData;
	if (monitorInfo->Name == monitorInfoExtension.szDevice)
	{
		monitorInfo->DisplayRectangle.Bottom = monitorInfoExtension.rcMonitor.bottom;
		monitorInfo->DisplayRectangle.Left   = monitorInfoExtension.rcMonitor.left;
		monitorInfo->DisplayRectangle.Right  = monitorInfoExtension.rcMonitor.right;
		monitorInfo->DisplayRectangle.Top    = monitorInfoExtension.rcMonitor.top;

		monitorInfo->WorkArea.Bottom = monitorInfoExtension.rcWork.bottom;
		monitorInfo->WorkArea.Left   = monitorInfoExtension.rcWork.left;
		monitorInfo->WorkArea.Right  = monitorInfoExtension.rcWork.right;
		monitorInfo->WorkArea.Top    = monitorInfoExtension.rcWork.top;
		return false;
	}
	return true;
}

/****************************************************************************
*                     GetWorkArea
*************************************************************************//**
*  @fn        core::Rectangle PlatformApplication::GetWorkArea(const core::Rectangle& window)
*
*  @brief     タスクバーを含まない実際の作業領域を返す
*
*  @param[in] window in the all region
*
*  @return    core::rectangle
*****************************************************************************/
core::Rectangle PlatformApplication::GetWorkArea(const core::Rectangle& window)
{
	const RECT windowsDimension =
	{
		.left   = window.Left,
		.top    = window.Top,
		.right  = window.Right,
		.bottom = window.Bottom
	};

	/*---------------------------------------------------------------
		   ウィンドウサイズに対し交差領域が最も大きいディスプレイモニターを取得
	-----------------------------------------------------------------*/
	HMONITOR bestMonitor = MonitorFromRect(&windowsDimension, MONITOR_DEFAULTTONEAREST);

	// monitor Infoを取得
	MONITORINFO monitorInfo;
	Memory::Zero(&monitorInfo, sizeof(monitorInfo));
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfo(bestMonitor, &monitorInfo);

	/*---------------------------------------------------------------
		   作業領域を取得する	
	-----------------------------------------------------------------*/
	core::Rectangle workArea =
	{
		.Left   = monitorInfo.rcWork.left,
		.Top    = monitorInfo.rcWork.top,
		.Right  = monitorInfo.rcWork.right,
		.Bottom = monitorInfo.rcWork.bottom
	};

	return workArea;
}


#endif