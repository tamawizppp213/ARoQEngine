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
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#include "../../Windows/Include/WindowsWindow.hpp"
#include "../../Core/Include/CoreCommonState.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
static constexpr LPCWSTR g_ApplicationName = L"Game Window";

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
}
#pragma endregion Contructor and Destructor

#pragma region Main Function
LRESULT CALLBACK PlatformApplication::WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return WindowMessageProcedure(hwnd, message, wParam, lParam);
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
	windowClass.lpfnWndProc   = WindowProcedure;  // window procedure. event driven 
	windowClass.cbClsExtra    = 0;                // window class構造体の後に確保する領域のバイト数
	windowClass.cbWndExtra    = 0;                // windowInstanceの後に確保する領域のバイト数
	windowClass.hInstance     = _instanceHandle;  // lpfnWndProcで指定した関数が含まれるインスタンスへのハンドルを指定
	windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION); // デフォルトアイコン
	windowClass.hCursor       = NULL;             // 別途管理する
	windowClass.hbrBackground = NULL;             // 背景は透明にする
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

void PlatformApplication::InitializeWindow(const std::shared_ptr<core::CoreWindow>& window)
{
	assert(("window is nullptr", window != nullptr));

	_window = std::static_pointer_cast<windows::CoreWindow>(window);
	//_window->Create();
}

LRESULT PlatformApplication::WindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		/*-----------------------------------------------------------------
		  WM_ACTIVATE is sent when the windows is activated or deactivated
		--------------------------------------------------------------------*/
		case WM_ACTIVATE:
		{
			return 0;
		}
		/*-----------------------------------------------------------------
		  WM_SIZE is sent when the windows size changed
		--------------------------------------------------------------------*/
		case WM_SIZE:
		{
			switch (wParam)
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
			}
			return 0;
		}
		/*-----------------------------------------------------------------
			WM_CLOSE is sent when the window is closed
		--------------------------------------------------------------------*/
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}
		/*-----------------------------------------------------------------
			WM_DESTROY is sent when the window is deleted
		--------------------------------------------------------------------*/
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return 0;
}
#pragma endregion Main Function
#endif