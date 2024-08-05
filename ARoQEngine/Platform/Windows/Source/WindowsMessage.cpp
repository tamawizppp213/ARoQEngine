//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Windows/Include/WindowsMessage.hpp"
#if PLATFORM_OS_WINDOWS
#include "../../Windows/Include/WindowsWindow.hpp"


//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
WindowsMessage::WindowsMessage()
{

}

WindowsMessage::~WindowsMessage()
{

}

#pragma endregion Constructor and Destructor

#pragma region Main Function
/*!***********************************************************************
*  @brief      キーボードによる入力メッセージかどうか
*  @param[in]  const uint32 メッセージ
*  @return     bool
**************************************************************************/
bool WindowsMessage::IsKeyboardMessage(const gu::uint32 message)
{
	switch (message)
	{
		case WM_CHAR:
		case WM_SYSCHAR:
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYUP:
		case WM_SYSCOMMAND:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}

/*!***********************************************************************
*  @brief      マウスによる入力メッセージかどうか
*  @param[in]  const uint32 メッセージ
*  @return     bool
**************************************************************************/
bool WindowsMessage::IsMouseMessage(const gu::uint32 message)
{
	switch(message)
	{
		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHOVER:
		case WM_MOUSELEAVE:
		case WM_MOUSEMOVE:
		case WM_NCMOUSEHOVER:
		case WM_NCMOUSELEAVE:
		case WM_NCMOUSEMOVE:
		case WM_NCMBUTTONDBLCLK:
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
		case WM_NCXBUTTONDBLCLK:
		case WM_NCXBUTTONDOWN:
		case WM_NCXBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_XBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}

/*!***********************************************************************
*  @brief      uint32であるため, 正しい入力メッセージに変換可能か
*  @param[in]  const uint32 メッセージ
*  @return     bool
**************************************************************************/
bool WindowsMessage::IsInputMessage(const gu::uint32 message)
{
	if (IsKeyboardMessage(message) || IsMouseMessage(message))
	{
		return true;
	}

	switch (message)
	{
		case WM_INPUT:
		case WM_INPUT_DEVICE_CHANGE:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}

#pragma endregion Main Function
#endif