//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_PLATFORM_APPLICATION_HPP
#define WINDOWS_PLATFORM_APPLICATION_HPP

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformApplication.hpp"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class PlatformApplication : public platform::core::PlatformApplication
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		HINSTANCE GetInstanceHandle() const noexcept { return _instanceHandle; }


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PlatformApplication() = default;

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		HINSTANCE _instanceHandle = NULL;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		bool RegisterWindowClass(const HINSTANCE instanceHandle, const HICON icon, const HCURSOR cursor);

		
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};
}
#endif
#endif