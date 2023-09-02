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
	//RegisterWindowClass();
}
#pragma endregion Contructor and Destructor

#pragma region Main Function
LRESULT CALLBACK PlatformApplication::WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

bool PlatformApplication::RegisterWindowClass(const HINSTANCE instanceHandle, const HICON icon)
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
	windowClass.hInstance     = instanceHandle;   // lpfnWndProcで指定した関数が含まれるインスタンスへのハンドルを指定
	windowClass.hIcon         = icon;             // アイコン
	windowClass.hCursor       = NULL;             // 別途管理する
	windowClass.hbrBackground = NULL;             // 背景は透明にする
	windowClass.lpszMenuName  = NULL;
	windowClass.lpszClassName = g_ApplicationName;

	// Register
	if (!::RegisterClass(&windowClass))
	{
		MessageBox(NULL, L"Failed to register class.", NULL, NULL);
		return false;
	}

	return true;
}

#pragma endregion Main Function
#endif