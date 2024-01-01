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

		/* @brief : resize the window*/
		virtual void Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height) = 0;
		
		// @brief: bring the window to front
		virtual void BringToFront(const bool forceFront = false) = 0;

		// @brief : move to the window at the x, y position
		virtual void Move(const gu::int32 x, const gu::int32 y) = 0;

		// @brief : enable / disable the window 
		virtual void Enable(const bool enable) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : return true if the window is in the foreground*/
		virtual bool IsFullscreenSupported() const = 0;

		virtual bool IsForegroundWindow() const = 0;

		virtual bool IsMaximized() const = 0;

		virtual bool IsMinimized() const = 0;

		virtual bool IsEnabled() = 0;

		virtual bool IsManualManageDPIChanges() const = 0;

		virtual void SetManualManageDPIChanges(const bool manualManageDPIChange) = 0;

		virtual void* GetWindowHandle() const noexcept = 0;

		virtual float GetAspectRatio() const = 0;

		WindowMode GetWindowMode() const { return _windowDesc.WindowMode; };

		virtual void GetWindowPostion(gu::int32& x, gu::int32& y) const = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : return the border size
		-----------------------------------------------------------------*/
		virtual gu::int32 GetWindowBorderSize() const = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : return the title bar size
		-----------------------------------------------------------------*/
		virtual gu::int32 GetWindowTitleBarSize() const = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : Returns the size and location of the window when it is restored 
		-----------------------------------------------------------------*/
		virtual bool GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : Return true if window exists underneath the coordinates
		-----------------------------------------------------------------*/
		virtual bool ExistPointInWindow(const gu::int32 x, const gu::int32 y) const = 0;

		/*---------------------------------------------------------------
		　　　　　@brief :  IsVisible the window
		-----------------------------------------------------------------*/
		bool IsVisible() const { return _isVisible; }

		/*---------------------------------------------------------------
		　　　　　@brief : Return the rectangle of the winow is associated with
		-----------------------------------------------------------------*/
		virtual void SetWindowMode(const WindowMode windowMode) = 0;

		virtual void SetText(const wchar_t* const text) = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : Sets the opacity of this window
		-----------------------------------------------------------------*/
		virtual void SetOpacity(const float opacity) const = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : Set this window to the keyboard focus 
		-----------------------------------------------------------------*/
		virtual void SetKeyboardFocus() = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : Return the rectangle of the winow is associated with
		-----------------------------------------------------------------*/
		virtual bool GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const = 0;

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