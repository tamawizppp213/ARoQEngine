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
#include "../../Core/Include/CoreCommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	class PlatformApplication;

	/****************************************************************************
	*				  			    CoreWindow
	*************************************************************************//**
	*  @class     CoreWindow
	*  @brief     This class is used to draw the objects
	*****************************************************************************/
	class CoreWindow : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : native window should make itself visible*/
		virtual bool Show() = 0;

		/* @brief : native window should hide itself */
		virtual bool Hide() = 0;

		/* @brief :  Native window should implement this function by performing the equivalent of the Win32 minimize-to-taskbar operation */
		virtual bool Minimize() = 0;

		/* @brief : Native window should implement this function by performing the equivalent of the Win32 maximize operation */
		virtual bool Maximize() = 0;

		/* @brief : Restore the window info when the window maximize and minimize*/
		virtual bool ReStore() = 0;

		/* @brief : Destroy the window*/
		virtual bool Destroy() = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : return true if the window is in the foreground*/
		virtual bool IsFullscreenSupported() const = 0;

		virtual bool IsForegroundWindow() const = 0;

		virtual bool IsMaximized() const = 0;

		virtual bool IsMinimized() const = 0;

		virtual void* GetWindowHandle() const noexcept = 0;

		virtual float GetAspectRatio() const = 0;

		WindowMode GetWindowMode() const { return _windowDesc.WindowMode; };

		/* @brief : IsVisible the window*/
		bool IsVisible() const { return _isVisible; }

		void SetWindowMode(const WindowMode windowMode) { _windowDesc.WindowMode = windowMode; }

		virtual void SetText(const wchar_t* const text) = 0;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CoreWindow();

		~CoreWindow();

		CoreWindow(const WindowMode windowMode);

		CoreWindow(const CoreWindowDesc& desc);

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		CoreWindowDesc _windowDesc = {};

		bool _isVisible = false;
	};
}
#endif