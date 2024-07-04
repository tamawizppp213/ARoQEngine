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
#include "../../Windows/Include/WindowsMessage.hpp"
#include "../../Windows/Include/WindowsWindow.hpp"
#include "../../Windows/Include/WindowsPlatformCommand.hpp"
#include "../../Windows/Include/WindowsOS.hpp"
#include "../../Core/Include/CoreCommonState.hpp"
#include "../../Core/Include/CoreWindowMessageHandler.hpp"
#include "../../Windows/Include/WindowsError.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"

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
	GetDPIForMonitorProcedure GetDPIForMonitorFunction = nullptr;

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
	const auto oleResult = OleInitialize(NULL);
	if(oleResult != S_OK)
	{
		printf("Failed to initialize OLE\n");
	}

	windows::WindowsOS::CoInitialize();

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
	_messageHandler = gu::MakeShared<core::CoreWindowMessageHandler>();

	/*-----------------------------------------------------------------
					  高DPIが設定可能かどうか
	--------------------------------------------------------------------*/
	_enableHighDPIMode = SetHighDPIMode();

	/*-----------------------------------------------------------------
					 マウスが接続されているか
	--------------------------------------------------------------------*/
	_isConnectedMouse = GetConnectedMiceCount() > 0;
}

PlatformApplication::~PlatformApplication()
{
	if (SHCoreDLL) { ::FreeLibrary(SHCoreDLL); }
	_windows.Clear();
	_windows.ShrinkToFit();
	_messageList.Clear();
	_messageList.ShrinkToFit();

	windows::PlatformCommand::CoUnInitialize();
	OleUninitialize();

	printf_s("Windows application is destroyed\n");
}

#pragma endregion Contructor and Destructor

#pragma region Main Function

/*!***********************************************************************
*  @brief      Windowsのイベントを実行するコールバック関数
*  @param[in]  HWND  ウィンドウハンドル
*  @param[in]  UINT  メッセージ
*  @param[in]  WPARAM
*  @param[in]  LPARAM
*  @return     LRESULT
**************************************************************************/
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

/*!***********************************************************************
*  @brief      Windowアプリケーションで新しいウィンドウクラスを登録します
*  @param[in]  void
*  @return     bool : ウィンドウクラスの登録に成功したかどうか
**************************************************************************/
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
	if (!::RegisterClassW(&windowClass))
	{
		MessageBoxW(NULL, L"Failed to register class.", NULL, NULL);
		return false;
	}

	return true;
}

/*!***********************************************************************
*  @brief      新規のウィンドウインスタンスを作成します. ここではセットアップは行いません
*  @param[in]  void
*  @return     SharedPointer<core::CoreWindow> ウィンドウ
**************************************************************************/
SharedPointer<core::CoreWindow> PlatformApplication::MakeWindow()
{
	return StaticPointerCast<core::CoreWindow>(MakeShared<windows::CoreWindow>());
}

/****************************************************************************
*                     MakeCommand
****************************************************************************/
/* @fn        gu::SharedPointer<core::PlatformCommand> PlatformApplication::MakeCommand()
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

/*!***********************************************************************
*  @brief      指定のウィンドウを実際に作成し, セットアップするまで行います (初期化時に呼び出し必要)
*  @param[in]  const gu::SharedPointer<CoreWindow>& window
*  @param[in]  const CoreWindowDesc& windowの設定項目
*  @param[in]  const gu::SharedPointer<CoreWindow>& 親のウィンドウ
*  @return     SharedPointer<core::CoreWindow> ウィンドウ
**************************************************************************/
void PlatformApplication::SetUpWindow(const SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow)
{
	Checkf(window, "window is nullptr");

	_windows.Push(StaticPointerCast<windows::CoreWindow>(window));
	
	const auto windowsCoreWindow = StaticPointerCast<windows::CoreWindow>(window);
	windowsCoreWindow->Create(StaticPointerCast<core::PlatformApplication>(SharedFromThis()), desc, parentWindow);
}

/****************************************************************************
*                     PumpMessage
****************************************************************************/
/* @fn        bool PlatformApplication::PumpMessage()
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
****************************************************************************/
/* @fn        LRESULT PlatformApplication::ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	return ProcessImmediateWindowsMessage(hwnd, message, wParam, lParam);
}

/*!***********************************************************************
*  @brief      ウィンドウの各メッセージを処理する実体(即時処理を行いたい処理を記述します)
*  @param[in]  HWND ウィンドウハンドル
*  @param[in]  UINT メッセージ
*  @param[in]  WPARAM
*  @param[in]  LPARAM
*  @return     LRESULT
**************************************************************************/
LRESULT PlatformApplication::PushDeferredWindowsMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_allowedToDeferredMessageProcessing)
	{
		_messageList.Push(DeferredMessage(hwnd, message, wParam, lParam));
		return 0;
	}
	else
	{
		return ProcessDeferredWindowsMessage(DeferredMessage(hwnd, message, wParam, lParam));
	}
}

/*!***********************************************************************
*  @brief      ウィンドウの各メッセージを処理する実体(即時処理を行いたい処理を記述します)
*  @param[in]  const DeferredMessage Windows専用のメッセージ
*  @return     LRESULT
**************************************************************************/
LRESULT PlatformApplication::ProcessImmediateWindowsMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ウィンドウが存在しない場合は何もしない
	if (_windows.IsEmpty()) 
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	} 

	// ウィンドウの取得
	const auto window = FindWindowByHWND(_windows, hwnd);
	
	if (!window) { return DefWindowProc(hwnd, message, wParam, lParam); }

	/*-----------------------------------------------------------------
		各メッセージの処理
	--------------------------------------------------------------------*/
	switch (message)
	{
		/*-----------------------------------------------------------------
			ユーザーがホットキーを使用するか新しい入力言語を選択したときに送信
			(wParam: 入力ロケール, lParam: 識別子)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-inputlangchangerequest
		--------------------------------------------------------------------*/
		case WM_INPUTLANGCHANGEREQUEST:
		/*-----------------------------------------------------------------
			入力言語が変更されたときに送信
			(wParam: 入力言語のBYTEフォント文字, lParam: 識別子)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-inputlangchange
		--------------------------------------------------------------------*/
		case WM_INPUTLANGCHANGE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			ウィンドウが非表示または表示されようとしている時にウィンドウに送信
			(wParam: ウィンドウが表示されているかどうかのフラグ, lParam: 表示されているウィンドウの状態)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-showwindow
		--------------------------------------------------------------------*/
		case WM_SHOWWINDOW:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			ウィンドウに対し、そのサイズが変更された後に送信されます
			(wParam: 要求されたサイズ変更の種類, lParam: 下位はクライアント領域の幅, 上位は高さ)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-size
		--------------------------------------------------------------------*/
		case WM_SIZE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);

			const bool wasMaximized = wParam == SIZE_MAXIMIZED;
			const bool wasRestored  = wParam == SIZE_RESTORED;

			if (wasMaximized || wasRestored)
			{
				_messageHandler->OnWindowAction(window, wasMaximized ? core::WindowActionType::Maximize : core::WindowActionType::Restore);
			}
			
			return 0;
		}
		/*-----------------------------------------------------------------
			ウィンドウに対し、そのサイズが変更されている最中に送信されます
			(wParam: サイズが設定されているウィンドウの端, lParam:  RECT 構造体へのポインタ)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-sizing
		--------------------------------------------------------------------*/
		case WM_SIZING:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			移動またはサイズ変更モーダル ループに入った後、ウィンドウに 1 回送信
			(wParam: 使用しない, lParam: 使用しない)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-entersizemove
		--------------------------------------------------------------------*/
		case WM_ENTERSIZEMOVE:
		{
			_inModalSizeLoop = true;
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			移動またはサイズ変更モーダル ループを終了した後、ウィンドウに 1 回送信されます
			(wParam: 使用しない, lParam: 使用しない)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-exitsizemove
		--------------------------------------------------------------------*/
		case WM_EXITSIZEMOVE:
		{
			_inModalSizeLoop = false;
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			ウィンドウが移動された後に送信されます。
			(wParam: 使用しない lParam : ウィンドウのクライアント領域の左上隅の x と y 座標 下位ワードには x 座標が含まれ、上位ワードには y 座標が含まれます)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-sizing
		--------------------------------------------------------------------*/
		case WM_MOVE:
		{
			const int32 x = LOWORD(lParam);
			const int32 y = HIWORD(lParam);

			_messageHandler->OnWindowMoved(window, x, y);
			return 0;
		}
		/*-----------------------------------------------------------------
		 特定の画面座標に対応するウィンドウの部分を決定するために、ウィンドウに送信
		 (wParam : 使用しない, lParam : カーソルのx, y座標)
		 https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-nchittest
		--------------------------------------------------------------------*/
		case WM_NCHITTEST:
		{
			break;
		}
		/*-----------------------------------------------------------------
		 カーソルが非アクティブウィンドウにあるときにマウスボタンを押すと送信
		 (wParam : アクティブ化されるウィンドウハンドル, lParam : 下位16ビットにヒットテスト値, 上位16ビットにマウスメッセージ)
		 https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-mouseactivate
		--------------------------------------------------------------------*/
		case WM_MOUSEACTIVATE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
				Windowがアクティブ化, 非アクティブ化された時に送られる	
		--------------------------------------------------------------------*/
		case WM_ACTIVATE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
		アクティブなウィンドウとは異なるアプリケーションに属するウィンドウがアクティブ化される時に送られる
		--------------------------------------------------------------------*/
		case WM_ACTIVATEAPP:
		{
			_isResizing = false;
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			WM_CLOSEはウィンドウが閉じられたときに送信されます
		--------------------------------------------------------------------*/
		case WM_CLOSE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
		  システム全体の設定を変更したとき, ポリシー設定が変更されたときに全ての最上位ウィンドウに送信
		--------------------------------------------------------------------*/
		case WM_SETTINGCHANGE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
		  対象ウィンドウの一部を塗りつぶす要求を行う場合に送信
		--------------------------------------------------------------------*/
		case WM_PAINT:
		{
			if (_isResizing)
			{
				_messageHandler->OnOSPainted(window);
			}
			break;
		}
		/*-----------------------------------------------------------------
		 ウィンドウの背景を消去する必要があるとき (たとえば、ウィンドウのサイズを変更する場合) に送信されます
		--------------------------------------------------------------------*/
		case WM_ERASEBKGND:
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-erasebkgnd
			return 1; // 自身で背景の消去を行うため, 0以外の値を返す必要がある. 
		}
		/*-----------------------------------------------------------------
		 ウィンドウの背景を消去する必要があるとき (たとえば、ウィンドウのサイズを変更する場合) に送信されます
		--------------------------------------------------------------------*/
		case WM_NCACTIVATE:
		{
			//  OSウィンドウのボーダーを使用しない限り、非クライアント領域がアクティブ化または非アクティブ化時にボーダーを描画しないようにします.
			//  標準的なアクティブ化が行われるようにtrueを返す
			if (!window->GetDesc().HasWindowOSWindowBorder)
			{
				return true;
			}
			break;
		}
		/*-----------------------------------------------------------------
		 フレームを塗りつぶす必要があるときにウィンドウに送信される
		--------------------------------------------------------------------*/
		case WM_NCPAINT:
		{
			//  OSのウィンドウ境界を使用しない限り、クライアント以外の領域を描画する呼び出しをインターセプトする
			if (!window->GetDesc().HasWindowOSWindowBorder)
			{
				return 0;
			}
			break;
		}
		/*-----------------------------------------------------------------
		 ウィンドウメニューからコマンドを選択したとき, ユーザーが最大化・最小化・復元・閉じるボタンをクリックしたときに送信
		--------------------------------------------------------------------*/
		case WM_SYSCOMMAND:
		{
			switch(wParam & 0xFFF0)
			{
				case SC_RESTORE:
				{
					if (IsIconic(hwnd))
					{
						::ShowWindow(hwnd, SW_RESTORE);
						return 0;
					}
					if (!_messageHandler->OnWindowAction(window, core::WindowActionType::Restore))
					{
						return 1;
					}
					break;
				}
				case SC_MINIMIZE:
				{
					_messageHandler->OnWindowAction(window, core::WindowActionType::Minimize);
					break;
				}
				case SC_MAXIMIZE:
				{
					_messageHandler->OnWindowAction(window, core::WindowActionType::Maximize);
					break;
				}
				case SC_CLOSE:
				{
					PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
					break;
				}
			}
			break;
		}
		/*-----------------------------------------------------------------
		 windowのサイズ変更できる最小の大きさと最大の大きさを変更する
		 (wParam : 使用しない, lParam : MINMAXINFOへのポインタ)
		 https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-getminmaxinfo
		--------------------------------------------------------------------*/
		case WM_GETMINMAXINFO:
		{
			return 0;
		}
		/*-----------------------------------------------------------------
		 非クライアント領域にカーソルがあるときにクリックされたときに送信
		 (wParam : ヒットテスト値(WM_NCHITTEST), lParam : カーソルのx, y座標を含むPOINTS構造体ではあるが, 使用するなと記載 
		 https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-nclbuttondown)
		--------------------------------------------------------------------*/
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		{
			switch (wParam)
			{
				case HTMINBUTTON:
				case HTMAXBUTTON:
				case HTCLOSE:
				case HTCAPTION:
				{
					if (!_messageHandler->OnWindowAction(window, core::WindowActionType::ClickedNonClientArea))
					{
						return 1;
					}
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		/*-----------------------------------------------------------------
			ディスプレイの解像度が変更された場合に送信される (最上位ウィンドウ)
			(wParam:イメージ深度, lParam: 画面の水平(下位), 垂直解像度(上位))
			https://learn.microsoft.com/ja-jp/windows/win32/gdi/wm-displaychange
		--------------------------------------------------------------------*/
		case WM_DISPLAYCHANGE:
		{
			break;
		}
		/*-----------------------------------------------------------------
			有効DPIが変更された場合に送信
			(wParam: dpiのX (HI), Y軸 (LO), lParam: RECT構造体のポインタ)
			https://learn.microsoft.com/ja-jp/windows/win32/hidpi/wm-dpichanged
		--------------------------------------------------------------------*/
		case WM_DPICHANGED:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			コントロールに関連付けられているキーボード入力メッセージを処理する
			https://learn.microsoft.com/ja-jp/windows/win32/dlgbox/wm-getdlgcode
		--------------------------------------------------------------------*/
		case WM_GETDLGCODE:
		{
			return DLGC_WANTALLKEYS;
		}
		/*-----------------------------------------------------------------
			CreateWindowを呼び出してウィンドウの作成を要求するときに送信
			(wParam : 使用しない, lParam : イベント固有のデータを含む構造体へのポインタ)
			https://learn.microsoft.com/ja-jp/windows/win32/devio/wm-devicechange
		--------------------------------------------------------------------*/
		case WM_CREATE:
		{
			return 0;
		}
		/*-----------------------------------------------------------------
			デバイスまたはコンピューターのハードウェア構成の変更をアプリケーションに通知します
			(wParam : 発生イベント, lParam : CreateStruct構造体へのポインタ)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-create
		--------------------------------------------------------------------*/
		case WM_DEVICECHANGE:
		{
			break;
		}
		/*-----------------------------------------------------------------
			Windowが破棄された時に送られる
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

	return DefWindowProc(hwnd, message, wParam, lParam);
}

/*!***********************************************************************
*  @brief      ウィンドウの各メッセージを処理する実体
*  @param[in]  const DeferredMessage Windows専用のメッセージ
*  @return     LRESULT
**************************************************************************/
LRESULT PlatformApplication::ProcessDeferredWindowsMessage(const DeferredMessage& message)
{
	const auto window      = FindWindowByHWND(_windows, message.WindowHandle);
	const auto messageCode = message.MessageCode;

	if (!window)          { return FALSE; }
	if (!_messageHandler) { return FALSE; }

	switch (messageCode)
	{
		/*-----------------------------------------------------------------
		  Windowがアクティブ化, 非アクティブ化された時に送られる
		--------------------------------------------------------------------*/
		case WM_ACTIVATE:
		{
			core::ActivationType activationType = core::ActivationType::InActivate;

			if (LOWORD(message.WParam) & WA_ACTIVE)
			{
				activationType = _isForceActivateByMouse ? core::ActivationType::ClickActivate : core::ActivationType::Activate;
			}
			else if (LOWORD(message.WParam) & WA_CLICKACTIVE)
			{
				activationType = core::ActivationType::ClickActivate;
			}

			_isForceActivateByMouse = false;

			return _messageHandler->OnWindowActivationChanged(window, core::ActivationType::Activate) ? 0 : 1;
		}

		/*-----------------------------------------------------------------
		  アクティブなウィンドウとは異なるアプリケーションに属するウィンドウがアクティブ化される時に送られる
		--------------------------------------------------------------------*/
		case WM_ACTIVATEAPP:
		{
			return _messageHandler->OnWindowApplicationActivationChanged(!!message.WParam) ? 0 : 1;
		}
		/*-----------------------------------------------------------------
		  システム全体の設定を変更したとき, ポリシー設定が変更されたときに全ての最上位ウィンドウに送信
		--------------------------------------------------------------------*/
		case WM_SETTINGCHANGE:
		{
			return _messageHandler->OnSettingChanged() ? 0 : 1;
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
			_messageHandler->OnWindowResizing(window);
			break;
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
			return _messageHandler->OnWindowClosed(gu::StaticPointerCast<core::CoreWindow>(window)) ? 0 : 1;
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
****************************************************************************/
/* @fn        void PlatformApplication::ProcessDeferredEvents()
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
/*!***********************************************************************
*  @brief      モニターのdot per inchを取得する.  https://zenn.dev/tenka/articles/windows_display_monitor_dpi
*  @param[in]  const core::MonitorInfo& monitor
*  @return     gu::int32 DPI
**************************************************************************/
gu::int32 PlatformApplication::GetMonitorDPI(const core::MonitorInfo& monitorInfo) const
{	
	if (!EnableHighDPIAwareness()) { return USER_DEFAULT_SCREEN_DPI; }

	int32 displayDPI = USER_DEFAULT_SCREEN_DPI;

	if (::GetDPIForMonitorFunction)
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
		if(monitor && SUCCEEDED(GetDPIForMonitorFunction(monitor, 0, &dpiX, &dpiY)))
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
****************************************************************************/
/* @fn        void CoreApplication::SetHighDPIMode()
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
		GetDPIForMonitorFunction    = (GetDPIForMonitorProcedure)      ::GetProcAddress(SHCoreDLL, "GetDpiForMonitor");

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
****************************************************************************/
/* @fn        float CoreApplication::GetDPIScaleFactorAtPixelPoint(const float x, const float y) const
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

	if (GetDPIForMonitorFunction)
	{
		POINT    position = { static_cast<LONG>(x), static_cast<LONG>(y) };
		HMONITOR monitor  = MonitorFromPoint(position, MONITOR_DEFAULTTONEAREST);

		uint32 dpiX = 0, dpiY = 0;
		if (monitor && SUCCEEDED(GetDPIForMonitorFunction(monitor, 0, &dpiX, &dpiY)))
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
****************************************************************************/
/* @fn        void PlatformApplication::GetMonitorsInfo(gu::DynamicArray<core::MonitorInfo>& monitorInfo) const
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

/*!***********************************************************************
*  @brief      Windowアプリケーションで新しいウィンドウクラスを登録します
*  @param[in]  void
*  @return     bool : ウィンドウクラスの登録に成功したかどうか
**************************************************************************/
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
		return FALSE;
	}

	return TRUE;
}

/*!***********************************************************************
*  @brief      タスクバーなどを無視した作業領域を返します.
*  @param[in]  const core::Rectangle& ウィンドウ
*  @return     core::Rectangle 作業領域
**************************************************************************/
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

/*!***********************************************************************
*  @brief      接続されているマウスの個数を取得します.
*  @param[in]  void
*  @return     gu:int32
**************************************************************************/
gu::int32 PlatformApplication::GetConnectedMiceCount()
{
	DynamicArray<RAWINPUTDEVICELIST> rawInputDevices = {};
	
	gu::uint32 deviceCount = 0;

	/*---------------------------------------------------------------
				接続されているInputデバイスを取得
	-----------------------------------------------------------------*/
	GetRawInputDeviceList(nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST));

	if (deviceCount == 0)
	{
		_isConnectedMouse = false;
		return 0;
	}

	rawInputDevices.Resize(deviceCount);
	GetRawInputDeviceList(rawInputDevices.Data(), &deviceCount, sizeof(RAWINPUTDEVICELIST));

	/*---------------------------------------------------------------
				マウスの個数の抽出
	-----------------------------------------------------------------*/
	gu::int32 mouseCount = 0;

	for (const auto& device : rawInputDevices)
	{
		// マウスデバイスのみを抽出
		if (device.dwType != RIM_TYPEMOUSE) { continue; }

		// 文字数を取得
		uint32 nameLength = 0;
		if (GetRawInputDeviceInfoA(device.hDevice, RIDI_DEVICENAME, nullptr, &nameLength) == static_cast<uint32>(-1))
		{
			continue;
		}

		// 文字情報を取得
		gu::string deviceName(nameLength, true);
		if (GetRawInputDeviceInfoA(device.hDevice, RIDI_DEVICENAME, deviceName.CString(), &nameLength) == static_cast<uint32>(-1))
		{
			continue;
		}

		if (deviceName.Contains("RDP_MOU", true))
		{
			continue;
		}

		mouseCount++;
	}

	// リモートデスクトップセッションが存在する場合には, マウスが接続されていると仮定する
	if (GetSystemMetrics(SM_REMOTESESSION))
	{
		mouseCount++;
	}

	_isConnectedMouse = mouseCount > 0;

	return mouseCount;
}

#endif