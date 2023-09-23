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
#include "../../Windows/Include/WindowsWindowsMessageHandler.hpp"
#include "../../Windows/Include/WindowsWindow.hpp"
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
#pragma endregion Main Function
#endif