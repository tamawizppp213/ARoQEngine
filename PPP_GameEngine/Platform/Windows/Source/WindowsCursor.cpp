//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Windows/Include/WindowsCursor.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
ICursor::ICursor(const core::CursorType type) : core::ICursor(type)
{
	SetType(type);
}

ICursor::~ICursor()
{

}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                     Show
*************************************************************************//**
*  @fn        void ICursor::Show()
*
*  @brief     Show cursor
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void ICursor::Show()
{
	::ShowCursor(true);
}

/****************************************************************************
*                     Hide
*************************************************************************//**
*  @fn        void ICursor::Hide()
*
*  @brief     Hide cursor
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void ICursor::Hide()
{
	::ShowCursor(false);
}

/****************************************************************************
*                     GetType
*************************************************************************//**
*  @fn        core::CursorType ICursor::GetType() const 
*
*  @brief     Return cursor type
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
core::CursorType ICursor::GetType() const 
{
	return _type;
}

/****************************************************************************
*                     SetType
*************************************************************************//**
*  @fn        void ICursor::SetType(const core::CursorType type)
*
*  @brief     Set cursor type
*
*  @param[in] const core::CursorType type
*
*  @return    void
*****************************************************************************/
void ICursor::SetType(const core::CursorType type)
{
	switch (type)
	{
	case core::CursorType::None:
	{
		break;
	}
	case core::CursorType::Default:
	{
		_cursor = ::LoadCursor(NULL, IDC_ARROW);
		break;
	}
	case core::CursorType::TextEditBeam:
	{
		_cursor = ::LoadCursor(NULL, IDC_IBEAM);
		break;
	}
	case core::CursorType::ResizeLeftRight:
	{
		_cursor = ::LoadCursor(NULL, IDC_SIZEWE);
		break;
	}
	case core::CursorType::ResizeUpDown:
	{
		_cursor = ::LoadCursor(NULL, IDC_SIZEWE);
		break;
	}
	case core::CursorType::ResizeSouthEast:
	{
		_cursor = ::LoadCursor(NULL, IDC_SIZENWSE);
		break;
	}
	case core::CursorType::ResizeSouthWest:
	{
		_cursor = ::LoadCursor(NULL, IDC_SIZENESW);
		break;
	}
	case core::CursorType::CardinalCross:
	{
		_cursor = ::LoadCursor(NULL, IDC_SIZEALL);
		break;
	}
	case core::CursorType::Crosshairs:
	{
		_cursor = ::LoadCursor(NULL, IDC_CROSS);
		break;
	}
	case core::CursorType::Hand:
	{
		_cursor = ::LoadCursor(NULL, IDC_HAND);
		break;
	}
	case core::CursorType::Prohibit:
	{
		_cursor = ::LoadCursor(NULL, IDC_NO);
		break;
	}
	default:
	{
		break;
	}
	}

	::SetCursor(_cursor);
}

/****************************************************************************
*                     GetPosition
*************************************************************************//**
*  @fn        gm::Float2 ICursor::GetPosition() const
*
*  @brief     Get cursor position in the monitor
*
*  @param[in] void
*
*  @return    gm::Float2
*****************************************************************************/
gm::Float2 ICursor::GetPosition() const
{
	POINT cursorPosition;
	::GetCursorPos(&cursorPosition);

	return gm::Float2(static_cast<float>(cursorPosition.x), static_cast<float>(cursorPosition.y));
}

/****************************************************************************
*                     SetPosition
*************************************************************************//**
*  @fn        void ICursor::SetPosition() const
*
*  @brief     Set cursor position in the monitor
*
*  @param[in] std::int32_t x
*  @param[in] std::int32_t y
*
*  @return    void
*****************************************************************************/
void ICursor::SetPosition(const std::int32_t x, const std::int32_t y) const 
{
	::SetCursorPos(x, y);
}
#pragma endregion Main Function