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
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Main Function
void CoreWindow::Create(const std::shared_ptr<core::PlatformApplication>& application, const core::CoreWindowDesc& desc)
{
	/*-----------------------------------------------------------------
					   Prepare 
	--------------------------------------------------------------------*/
	assert(("application is nullptr", application));

	const auto windowsApplication = std::static_pointer_cast<windows::PlatformApplication>(application);

	/*-----------------------------------------------------------------
		               Window Settings
	--------------------------------------------------------------------*/
	int windowWidth   = static_cast<int>(desc.DesiredScreenWidth);
	int windowHeight  = static_cast<int>(desc.DesiredScreenHeight);
	int windowCenterX = static_cast<int>(desc.DesiredScreenPositionX);
	int windowCenterY = static_cast<int>(desc.DesiredScreenPositionY);

	/*-----------------------------------------------------------------
	                	Create window
	--------------------------------------------------------------------*/
	_hwnd = CreateWindowEx
	(
		0, // Window style
		core::PlatformApplication::APPLICATION_NAME, // Name to register with Windows
		desc.Title,
		NULL,
		windowCenterX, // Center position_X
		windowCenterY, // Center position_Y
		windowWidth,   // window width
		windowHeight,  // window height
		NULL, // parent window
		NULL, // menu handler 
		windowsApplication->GetInstanceHandle(), // instance handler 
		NULL  // additional expansion area
	);

	if (!_hwnd)
	{
		MessageBox(NULL, L"Couldn't create window", NULL, NULL);
		return;
	}

	/*---------------------------------------------------------------
							Show Window
	-----------------------------------------------------------------*/
	ShowWindow(_hwnd, SW_SHOW);
	UpdateWindow(_hwnd);
}

void CoreWindow::Show()
{
	if (_isVisible) { return; }
	
	_isVisible = true;
	::ShowWindow(_hwnd, SW_SHOW);
}

void CoreWindow::Hide()
{
	if (_isVisible)
	{
		_isVisible = false;
		::ShowWindow(_hwnd, SW_HIDE);
	}
}

void CoreWindow::Minimize()
{
	if (_isFirstTimeVisible)
	{

	}
	else
	{
		::ShowWindow(_hwnd, SW_MINIMIZE);
	}
}

void CoreWindow::Maximize()
{
	if (_isFirstTimeVisible)
	{

	}
	else
	{
		::ShowWindow(_hwnd, SW_RESTORE);
	}
}
#pragma endregion Main Function