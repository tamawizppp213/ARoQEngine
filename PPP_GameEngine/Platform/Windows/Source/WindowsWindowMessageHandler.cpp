//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Windows/Include/WindowsWindowsMessageHandler.hpp"
#if PLATFORM_OS_WINDOWS
#include "../../Windows/Include/WindowsWindow.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <cassert>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
CoreWindowMessageHandler::CoreWindowMessageHandler()
{

}

CoreWindowMessageHandler::~CoreWindowMessageHandler()
{

}

#pragma endregion Constructor and Destructor

#pragma region Main Function
bool CoreWindowMessageHandler::OnWindowActivationChanged(const CoreWindowPtr& window, const core::ActivationType type)
{
	return true;
}

bool CoreWindowMessageHandler::OnWindowClosed(const CoreWindowPtr& window)
{
	return window->Destroy();
}

bool CoreWindowMessageHandler::OnSizeChanged(const std::uint32_t width, const std::uint32_t height)
{
	Screen::SetScreenWidth(width);
	Screen::SetScreenHeight(height);
	return true;
}
#pragma endregion Main Function
#endif