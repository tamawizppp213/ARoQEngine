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
#if PLATFORM_OS_WINDOWS
#include "../../Windows/Include/WindowsError.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Math/Include/GMColor.hpp"
#include <vector>
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

ICursor::ICursor(const gu::char8* filePath) : core::ICursor(core::CursorType::Original)
{
	const auto handle = (HCURSOR)CreateCursorFromFile(filePath);

	if (handle == nullptr)
	{
		ErrorLogger::Log(nullptr);
	}

	_cursor = handle;
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
*                     Lock
*************************************************************************//**
*  @fn        void ICursor::Lock(core::Rectangle* rectangle)
*
*  @brief     �J�[�\����̈�������ł����������Ȃ��悤�ɂ��܂�
*
*  @param[in] core::Rectangle* rectangle pointer
*
*  @return    bool
*****************************************************************************/
bool ICursor::Lock(core::Rectangle* rectangle)
{
	::ClipCursor(reinterpret_cast<RECT*>(rectangle));
	return ErrorLogger::Succeed();
}

/****************************************************************************
*                     CreateCursorFromFile
*************************************************************************//**
*  @fn        void* ICursor::CreateCursorFromFile(const gu::char8* filePath, const gm::Float2 inHotSpot)
*
*  @brief     �t�@�C����ǂݍ���ŃJ�[�\�����g�p���܂�. 
*             ����.cur�̂ݎ�t�\�ł�. 
*
*  @param[in] const gu::char8* filePath
*  @param[in] const gm::Float2 inHotSpot
*
*  @return    bool
*****************************************************************************/
void* ICursor::CreateCursorFromFile(const gu::char8* filePath)
{
	// pixel��0�S���Ă���͎̂��ۂ̃��\�[�X�̍������g�p���邽�߂ł�. 
	return (HCURSOR)LoadImageA(nullptr, filePath, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
}

/****************************************************************************
*                     CreateCursorFromRGBABuffer
*************************************************************************//**
*  @fn        void* ICursor::CreateCursorFromRGBABuffer(const gm::RGBA* pixels, const gu::int32 width, const gu::int32 height)
*
*  @brief     RGBA�̍\���̂��璼�ڃA�C�R�����쐬���܂�
*
*  @param[in] const gu::RGBA* pixels
*  @param[in] const gu::int32 pixel width
*  @param[in] const gu::int32 pixel height
*
*  @return    void*
*****************************************************************************/
void* ICursor::CreateCursorFromRGBABuffer(const gm::RGBA* pixels, const gu::int32 width, const gu::int32 height)
{
	Check(width > 0);
	Check(height > 0);

	std::vector<gm::BGRA>  bgraPixels(width * height);
	std::vector<gu::uint8> maskPixels(width * height);

	/*---------------------------------------------------------------
			BGRA���ɕϊ�
	-----------------------------------------------------------------*/
	for (int i = 0; i < bgraPixels.size(); ++i)
	{
		bgraPixels[i] = gm::BGRA(pixels[i].B, pixels[i].G, pixels[i].R, pixels[i].A);
		maskPixels[i] = 255;
	}

	/*---------------------------------------------------------------
			�r�b�g�}�b�v�̍쐬
	-----------------------------------------------------------------*/
	const auto cursorColor = ::CreateBitmap(width, height, 1, 32, bgraPixels.data());
	const auto cursorMask  = ::CreateBitmap(width, height, 1, 8, maskPixels.data());

	ICONINFO iconInfo =
	{
		.fIcon = false,
		.xHotspot = 0,
		.yHotspot = 0,
		.hbmMask  = cursorMask,
		.hbmColor = cursorColor
	};

	const auto cursorHandle = ::CreateIconIndirect(&iconInfo);

	/*---------------------------------------------------------------
			�I������
	-----------------------------------------------------------------*/
	::DeleteObject(cursorColor);
	::DeleteObject(cursorMask);

	bgraPixels.clear(); bgraPixels.shrink_to_fit();
	maskPixels.clear(); maskPixels.shrink_to_fit();
	return nullptr;
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
*                     GetSize
*************************************************************************//**
*  @fn        void ICursor::GetSize(gu::int32& width, gu::int32& height) const
*
*  @brief     Return cursor size
*
*  @param[out] gu::int32& width
*  @param[out] gu::int32& height
*
*  @return    void
*****************************************************************************/
void ICursor::GetSize(gu::int32& width, gu::int32& height) const
{
	ICONINFO iconInfo = {};

	/*---------------------------------------------------------------
			�A�C�R���̏����擾���܂�
	-----------------------------------------------------------------*/
	if (!GetIconInfo((HICON)_cursor, &iconInfo)) { width = 0; height = 0; return; }

	/*---------------------------------------------------------------
			�A�C�R���̃T�C�Y���擾���܂�
	-----------------------------------------------------------------*/
	BITMAP bitmap = {};
	if (GetObject(iconInfo.hbmMask, sizeof(bitmap), &bitmap) == sizeof(bitmap))
	{
		width  = bitmap.bmWidth;

		// ���m�N���A�C�R���̏ꍇ, �㕔��AND�}�X�N, ������XOR�}�X�N�����Ă���֌W�ŃA�C�R���̍�����2�{���������ƂɂȂ�܂�.
		// https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-iconinfo
		height = iconInfo.hbmColor ? bitmap.bmHeight : bitmap.bmHeight / 2;
	}
	if (iconInfo.hbmMask)
	{
		DeleteObject(iconInfo.hbmMask);
	}
	if (iconInfo.hbmColor)
	{
		DeleteObject(iconInfo.hbmColor);
	}
	
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
void ICursor::SetPosition(const gu::int32 x, const gu::int32 y) const 
{
	::SetCursorPos(x, y);
}

#pragma endregion Main Function
#endif