//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Application.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "resource.h"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
static constexpr LPCWSTR CLASS_NAME = L"Game Window";
static constexpr LPCWSTR GAME_TITLE = L"PPP Engine";
static constexpr int GAME_WINDOW_WIDTH  = 1920;
static constexpr int GAME_WINDOW_HEIGHT = 1080;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
rhi::core::LowLevelGraphicsEngine test;
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Application::Instance().WindowMessageProcedure(hwnd, message, wParam, lParam);
}

bool Application::StartUp()
{
	if (!CreateMainWindow())                               { return false; }
	if (!_gameInput.Initialize(_appInstance, _mainWindow)) { return false; }
	
	test.StartUp(rhi::core::APIVersion::Vulkan, _mainWindow, _appInstance);
	return true;
}

void Application::Run()
{
	MSG message = { NULL };

	_gameTimer.Reset();
	//_gameManager.GameStart(_gameTimer, _mainWindow, _appInstance);
	/*---------------------------------------------------------------
						Main Loop
	-----------------------------------------------------------------*/
	while (WM_QUIT != message.message)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			_gameTimer.Tick();
			if (!_isApplicationPaused)
			{
				_gameTimer.AverageFrame(_mainWindow);
				_gameInput.Update();
				//_gameManager.GameMain();
				//a.BeginDrawFrame();
				//a.EndDrawFrame();
			}
		}
	}
}

void Application::ShutDown()
{
	_gameInput.Finalize();
	//_gameManager.GameEnd();
}

#pragma region Private Function
/****************************************************************************
*                       CreateMainWindow
*************************************************************************//**
*  @fn        bool Application::CreateMainWindow()
*  @brief     Create main window (init: full screen size) 
*             Reference: http://kaitei.net/winapi/creating-windows/
*  @param[in] void
*  @return Å@Å@void
*****************************************************************************/
bool Application::CreateMainWindow()
{
	/*---------------------------------------------------------------
						Register Window Class
	-----------------------------------------------------------------*/
	_appInstance = GetModuleHandle(NULL);
	WNDCLASS windowClass      = {};
	windowClass.style         = CS_VREDRAW | CS_HREDRAW; // enable to redraw (vertical and horizon)
	windowClass.lpfnWndProc   = WindowProcedure;         // Event driven function 
	windowClass.cbClsExtra    = 0;                       // No bytes are allocated in the area after this.
	windowClass.cbWndExtra    = 0;
	windowClass.hInstance     = _appInstance;
	windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION); //Load Default Icon
	windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW)    ; // Set default mouse cursor
	windowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	windowClass.lpszMenuName  = MAKEINTRESOURCE(GAME_MENU);
	windowClass.lpszClassName = CLASS_NAME;

	if (!RegisterClass(&windowClass))
	{
		MessageBox(NULL, L"Failed to register class.", NULL, NULL);
	}
	
	/*---------------------------------------------------------------
						Set Full Screen size
	-----------------------------------------------------------------*/
	RECT  desktopWindowRect    = {};
	HWND  desktopWindow        = GetDesktopWindow();
	GetWindowRect(desktopWindow, &desktopWindowRect);
	Screen::SetFullScreenHeight(desktopWindowRect.bottom - desktopWindowRect.top);
	Screen::SetFullScreenWidth (desktopWindowRect.right  - desktopWindowRect.left);

	/*---------------------------------------------------------------
						Set Game screen window size
	-----------------------------------------------------------------*/
	RECT gameWindowRect = { 0,0, Screen::GetFullScreenWidth(), Screen::GetFullScreenHeight()};
	Screen::SetScreenWidth (Screen::GetFullScreenWidth());
	Screen::SetScreenHeight(Screen::GetFullScreenHeight());

	/*---------------------------------------------------------------
						CenteringWindow
	-----------------------------------------------------------------*/
	int windowPositionX = max((Screen::GetFullScreenWidth() - Screen::GetScreenWidth()) / 2, 0);
	int windowPositionY = max((Screen::GetFullScreenHeight() - Screen::GetScreenHeight()) / 2, 0);
	/*---------------------------------------------------------------
						Create Window Object
	-----------------------------------------------------------------*/
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	_mainWindow = CreateWindow(
		CLASS_NAME,               // Name to register with Windows
		GAME_TITLE,               // Game Title
		windowStyle,              // Window style
		windowPositionX,          // Center position_x
		windowPositionY,          // Center position_y
		Screen::GetScreenWidth(), // Window width
		Screen::GetScreenHeight(),// Window height
		NULL,                     // Parent window
		NULL,                     // Menu handler
		_appInstance,             // instance handler
		NULL);                    // Additional expansion area
	
	if (!_mainWindow)
	{
		MessageBox(NULL, L"Couldn't create window", NULL, NULL);
		return false;
	}

	/*---------------------------------------------------------------
							Show Window
	-----------------------------------------------------------------*/
	ShowWindow(_mainWindow, SW_SHOW);
	UpdateWindow(_mainWindow);
	return true;
}

LRESULT Application::WindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		case WM_SIZE: 
		{
			Screen::SetScreenWidth(LOWORD(lParam));
			Screen::SetScreenHeight(HIWORD(lParam));
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
		/*-----------------------------------------------------------------
			WM_DESTROY is sent when the window is deleted
		--------------------------------------------------------------------*/
		case WM_COMMAND:
		{
			ExecuteWindowsCommand(wParam);
			return 0;
		}
		/*-----------------------------------------------------------------
			WM_DisplayChange is sent when the window is changed
		--------------------------------------------------------------------*/
		case WM_DISPLAYCHANGE:
		{
			_gameManager.GameEnd();
			_gameManager.GameStart(_gameTimer, _mainWindow, _appInstance);
			return 0;
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT Application::ExecuteWindowsCommand(WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
		case ID_FILE_OPEN:
		{
			break;
		}
	}
	return 0;
}
#pragma endregion Private Function