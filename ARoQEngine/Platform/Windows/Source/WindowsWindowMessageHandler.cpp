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
bool CoreWindowMessageHandler::OnWindowActivationChanged([[maybe_unused]] const CoreWindowPtr& window, [[maybe_unused]] const core::ActivationType type)
{
	return true;
}

bool CoreWindowMessageHandler::OnWindowClosed(const CoreWindowPtr& window)
{
	return window->Destroy();
}

bool CoreWindowMessageHandler::OnSizeChanged(const CoreWindowPtr& window, const gu::uint32 width, const gu::uint32 height)
{
	if (_isResizing) { return false; }

	// Resizeの::SetWindowPosによってさらにResizeが呼ばれるのを防ぎます.
	_isResizing = true;

	// 左上の位置を取得する.
	gu::int32 x = 0, y = 0;
	window->GetWindowPostion(x, y);

	// サイズ変更
	window->Resize(x, y, width, height);

	// 後々消します.
	Screen::SetScreenWidth(width);
	Screen::SetScreenHeight(height);

	_isResizing = false;
	return true;
}
#pragma endregion Main Function
#endif