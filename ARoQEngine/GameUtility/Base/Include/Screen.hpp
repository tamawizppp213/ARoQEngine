//////////////////////////////////////////////////////////////////////////////////
//              Title:  Screen.hpp
//            Content:  Screen Config
//             Author:  Toide Yutaro
//             Create:  2022_03_13
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCREEN_HPP
#define SCREEN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                  Window Class (width, height static)
//                  Default Size  width: 1920 Å~ height: 1080
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			TemplateClass
*************************************************************************//**
*  @class     TemplateClass
*  @brief     temp
*****************************************************************************/
class Screen
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/

	/****************************************************************************
	**                Public Property
	*****************************************************************************/
	static int   GetScreenWidth();
	static int   GetScreenHeight();
	static float GetOnePixelWidth();
	static float GetOnePixelHeight();
	static float GetAspectRatio();
	static int   GetFullScreenWidth();
	static int   GetFullScreenHeight();
	
	static void SetScreenWidth(int width);
	static void SetScreenHeight(int height);
	static void SetFullScreenWidth(int width);
	static void SetFullScreenHeight(int height);
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	Screen() = default;
	~Screen() = default;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Property
	*****************************************************************************/
	static float _onePixelWidth;
	static float _onePixelHeight;
	static int   _currentPixelWidth;
	static int   _currentPixelHeight;
	static int   _fullScreenResolutionWidth;
	static int   _fullScreenResolutionHeight;
	static bool  _isFullScreen;
};
#endif