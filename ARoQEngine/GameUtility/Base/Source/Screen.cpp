//////////////////////////////////////////////////////////////////////////////////
//              Title:  Screen.hpp
//            Content:  Screen Config
//             Author:  Toide Yutaro
//             Create:  2022_03_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/Screen.hpp"
#include <assert.h>
//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
static constexpr int DEFAULT_SCREEN_WIDTH = 1920;
static constexpr int DEFAULT_SCREEN_HEIGHT = 1080;
int Screen::_currentPixelWidth          = DEFAULT_SCREEN_WIDTH;
int Screen::_currentPixelHeight         = DEFAULT_SCREEN_HEIGHT;
int Screen::_fullScreenResolutionWidth  = DEFAULT_SCREEN_WIDTH;
int Screen::_fullScreenResolutionHeight = DEFAULT_SCREEN_HEIGHT;
float Screen::_onePixelWidth            = 1.0f / DEFAULT_SCREEN_WIDTH;
float Screen::_onePixelHeight           = 1.0f / DEFAULT_SCREEN_HEIGHT;
//////////////////////////////////////////////////////////////////////////////////
//                  Window Class (width, height static)
//                  Default Size  width: 1920 × height: 1080
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*                       GetScreenWidth
*************************************************************************//**
*  @fn        int Screen::GetScreenWidth()
*  @brief     return current screen pixel width
*  @param[in] void
*  @return 　　int 
*****************************************************************************/
int Screen::GetScreenWidth()
{
	return _currentPixelWidth;
}
/****************************************************************************
*                       GetScreenHeight
*************************************************************************//**
*  @fn        int Screen::GetScreenHeight()
*  @brief     return current screen pixel height
*  @param[in] void
*  @return 　　int
*****************************************************************************/
int Screen::GetScreenHeight()
{
	return _currentPixelHeight;
}
/****************************************************************************
*                       GetOnePixelWidth
*************************************************************************//**
*  @fn        float Screen::GetOnePixelWidth()
*  @brief     return one pixel width
*  @param[in] void
*  @return 　　float
*****************************************************************************/
float Screen::GetOnePixelWidth()
{
	return _onePixelWidth;
}
/****************************************************************************
*                       GetOnePixelHeight
*************************************************************************//**
*  @fn        float Screen::GetOnePixelHeight()
*  @brief     return one pixel height
*  @param[in] void
*  @return 　　float
*****************************************************************************/
float Screen::GetOnePixelHeight()
{
	return _onePixelHeight;
}
/****************************************************************************
*                       GetFullScreenWidth
*************************************************************************//**
*  @fn        float Screen::GetOnePixelWidth()
*  @brief     return one pixel height
*  @param[in] void
*  @return 　　int
*****************************************************************************/
int Screen::GetFullScreenWidth()
{
	return _fullScreenResolutionWidth;
}
/****************************************************************************
*                       GetFullScreenHeight
*************************************************************************//**
*  @fn        float Screen::GetOnePixelHeight()
*  @brief     return full screen pixel height
*  @param[in] void
*  @return 　　int
*****************************************************************************/
int Screen::GetFullScreenHeight()
{
	return _fullScreenResolutionHeight;
}
/****************************************************************************
*                       SetFullScreenWidth
*************************************************************************//**
*  @fn        float Screen::SetFullScreenWidth(int width)
*  @brief     Set Full screen width
*  @param[in] int width
*  @return 　　void
*****************************************************************************/
void Screen::SetFullScreenWidth(int width)
{
	_fullScreenResolutionWidth = width;
}
/****************************************************************************
*                       SetFullScreenHeight
*************************************************************************//**
*  @fn        float Screen::SetFullScreenHeight(int height)
*  @brief     Set Full screen height
*  @param[in] int height
*  @return 　　void
*****************************************************************************/
void Screen::SetFullScreenHeight(int height)
{
	_fullScreenResolutionHeight = height;
}
/****************************************************************************
*                       SetScreenWidth
*************************************************************************//**
*  @fn        float Screen::SetScreenWidth(int width)
*  @brief     Set screen width
*  @param[in] int width
*  @return 　　void
*****************************************************************************/
void Screen::SetScreenWidth(int width)
{
	_currentPixelWidth= width <= _fullScreenResolutionWidth ? width : _fullScreenResolutionWidth;
	_onePixelWidth = width >= 1.0f ? 1.0f / width : 0.0f;
}
/****************************************************************************
*                       SetScreenHeight
*************************************************************************//**
*  @fn        float Screen::SetScreenHeight(int height)
*  @brief     Set screen height
*  @param[in] int height
*  @return 　　void
*****************************************************************************/
void Screen::SetScreenHeight(int height)
{
	_currentPixelHeight = height <= _fullScreenResolutionHeight ? height : _fullScreenResolutionHeight;
	_onePixelHeight = height >= 1.0f ? 1.0f / height : 0.0f;
}
/****************************************************************************
*                       GetAspectRatio
*************************************************************************//**
*  @fn        float Screen::GetAspectRatio()
*  @brief     Get aspect ratio
*  @param[in] void
*  @return 　　float 
*****************************************************************************/
float Screen::GetAspectRatio()
{
	return (float)_currentPixelWidth / (float)_currentPixelHeight;
}