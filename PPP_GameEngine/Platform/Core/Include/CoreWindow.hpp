//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GENERAL_WINDOW_HPP
#define GENERAL_WINDOW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <cstdint>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	enum class WindowMode
	{
		FullScreen,         
		BorderLess,
		Windowed,
		CountOf
	};

	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class CoreWindow : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : native window should make itself visible*/
		virtual void Show() = 0;

		/* @brief : native window should hide itself */
		virtual void Hide() = 0;

		/* @brief :  Native window should implement this function by performing the equivalent of the Win32 minimize-to-taskbar operation */
		virtual void Minimize() = 0;

		/* @brief : Native window should implement this function by performing the equivalent of the Win32 maximize operation */
		virtual void Maximize() = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		WindowMode GetWindowMode() const { return _windowMode; };

		bool IsVisible() const { return _isVisible; }

		void SetWindowMode(const WindowMode windowMode) { _windowMode = windowMode; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CoreWindow() = default;

		~CoreWindow() = default;

		CoreWindow(const WindowMode windowMode) : _windowMode(windowMode) {};
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		WindowMode _windowMode = WindowMode::Windowed;

		bool _isVisible = true;
	};
}
#endif